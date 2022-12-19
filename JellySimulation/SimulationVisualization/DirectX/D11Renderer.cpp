#include "pch.h"
#include "D11Renderer.h"

#include <Scene/Scene.h>
#include <Scene/SceneIterator.h>

#include <DirectX/D11ShaderLoader.h>

#include <Components/Rendering/RenderingComponent.h>
#include <Components/Rendering/DepthStateComponent.h>
#include <Components/Rendering/TessellationComponent.h>
#include <Components/Rendering/BlendStateComponent.h>

D11Renderer::D11Renderer()
{
    m_device = std::make_shared<D11Device>();
    SetUpStates();
}

D11Renderer::~D11Renderer()
{
    D11ShaderLoader::Clear();

    Context()->OMSetRenderTargets(0, nullptr, nullptr);
    m_device.reset();
}

HRESULT D11Renderer::ResetRenderTarget(void* resource)
{
    this->Context()->OMSetRenderTargets(0, nullptr, nullptr);
    return this->InitRenderTarget(resource);
}

HRESULT D11Renderer::InitRenderTarget(void* pResource)
{
    HRESULT hr = S_OK;

    IUnknown* pUnk = (IUnknown*)pResource;

    IDXGIResource* pDXGIResource;
    hr = pUnk->QueryInterface(__uuidof(IDXGIResource), (void**)&pDXGIResource);
    if (FAILED(hr))
    {
        return hr;
    }

    HANDLE sharedHandle;
    hr = pDXGIResource->GetSharedHandle(&sharedHandle);
    if (FAILED(hr))
    {
        return hr;
    }

    pDXGIResource->Release();

    IUnknown* tempResource11;
    hr = Device()->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&tempResource11));
    if (FAILED(hr))
    {
        return hr;
    }

    ID3D11Texture2D* pOutputResource;
    hr = tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&pOutputResource));
    if (FAILED(hr))
    {
        return hr;
    }
    tempResource11->Release();

    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    rtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;

    hr = Device()->CreateRenderTargetView(pOutputResource, &rtDesc, m_renderTargetView.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    D3D11_TEXTURE2D_DESC outputResourceDesc;
    pOutputResource->GetDesc(&outputResourceDesc);
    if (outputResourceDesc.Width != m_width || outputResourceDesc.Height != m_height)
    {
        m_width = outputResourceDesc.Width;
        m_height = outputResourceDesc.Height;

        SetUpViewport();
    }

    m_depthStencilView.Reset();

    m_depthStencilView = m_device->CreateDepthStencilBuffer(m_width, m_height);

    Context()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    if (NULL != pOutputResource)
    {
        pOutputResource->Release();
    }

    return hr;
}

Matrix D11Renderer::GetProjectionMatrix() const
{
    auto aspectRatio = static_cast<float>(m_width) / m_height;

    return Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(90.0f), aspectRatio, 0.1f, 100.0f);
}

void D11Renderer::Clear(float r, float g, float b, float a)
{
    float rgba[] = { r, g, b, a };

    Context()->ClearRenderTargetView(m_renderTargetView.Get(), rgba);
    Context()->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D11Renderer::StartFrame()
{
    Context()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void D11Renderer::Render(Scene& scene)
{
    for (auto it = scene.Begin<RenderingComponent>(); it != scene.End<RenderingComponent>(); ++it)
    {
        auto entity = it.Get();
        auto& render = entity.GetComponent<RenderingComponent>();

        if (!render.ShouldRender)
        {
            continue;
        }

        render.OnRenderingHandle(*this, entity);

        auto& vb = render.VertexBuffer;
        auto& ib = render.IndexBuffer;

        auto& vs = render.VertexShader;
        auto& ps = render.PixelShader;

        ID3D11Buffer* buffers[] = { vb->Buffer() };
        UINT strides[] = { vb->Stride() };
        UINT offsets[] = { vb->Offset() };

        m_device->Context()->IASetInputLayout(vs->GetLayout());
        m_device->Context()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
        m_device->Context()->IASetIndexBuffer(ib->Buffer(), ib->Format(), 0);
        m_device->Context()->IASetPrimitiveTopology(ib->Topology());

        auto vsConstantBuffers = vs->RawConstantBuffers();
        auto psConstantBuffers = ps->RawConstantBuffers();

        m_device->Context()->VSSetShader(vs->Shader(), nullptr, 0);
        m_device->Context()->VSSetConstantBuffers(0, vsConstantBuffers.size(), vsConstantBuffers.data());

        m_device->Context()->PSSetShader(ps->Shader(), nullptr, 0);
        m_device->Context()->PSSetConstantBuffers(0, psConstantBuffers.size(), psConstantBuffers.data());

        if (!ps->Textures().empty())
        {
            auto textures = ps->RawTextures();
            m_device->Context()->PSSetShaderResources(0, textures.size(), textures.data());
        }
        else
        {
            m_device->Context()->PSSetShaderResources(0, 0, nullptr);
        }

        if (entity.HasComponent<TessellationComponent>())
        {
            auto& tessellation = entity.GetComponent<TessellationComponent>();

            auto& hs = tessellation.HullShader;
            auto& ds = tessellation.DomainShader;
            auto hsConstantBuffers = hs->RawConstantBuffers();
            auto dsConstantBuffers = ds->RawConstantBuffers();

            m_device->Context()->HSSetShader(hs->Shader(), nullptr, 0);
            m_device->Context()->HSSetConstantBuffers(0, hsConstantBuffers.size(), hsConstantBuffers.data());

            m_device->Context()->DSSetShader(ds->Shader(), nullptr, 0);
            m_device->Context()->DSSetConstantBuffers(0, dsConstantBuffers.size(), dsConstantBuffers.data());
        }
        else
        {
            m_device->Context()->HSSetShader(nullptr, nullptr, 0);
            m_device->Context()->DSSetShader(nullptr, nullptr, 0);
        }

        m_device->Context()->DrawIndexed(ib->Count(), 0, 0);
    }
}

void D11Renderer::EndFrame()
{
    Context()->Flush();
}

void D11Renderer::SetUpViewport()
{
    D3D11_VIEWPORT vp{};
    vp.Width = (float)m_width;
    vp.Height = (float)m_height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    Context()->RSSetViewports(1, &vp);
}

void D11Renderer::SetUpStates()
{
    D3D11_RASTERIZER_DESC rsDesc = {};

    rsDesc.FillMode = D3D11_FILL_SOLID; 
    rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; 
    rsDesc.FrontCounterClockwise = false; 
    rsDesc.DepthBias = 0;
    rsDesc.DepthBiasClamp = 0.0f;
    rsDesc.SlopeScaledDepthBias = 0.0f;
    rsDesc.DepthClipEnable = true;
    rsDesc.ScissorEnable = false;
    rsDesc.MultisampleEnable = false;
    rsDesc.AntialiasedLineEnable = false;

    m_device->Raw()->CreateRasterizerState(&rsDesc, &m_rasterizerState);
    m_device->Context()->RSSetState(m_rasterizerState.Get());
}
