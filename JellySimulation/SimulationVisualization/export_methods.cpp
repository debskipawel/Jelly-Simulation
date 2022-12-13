#include "pch.h"
#include "export_methods.h"

#include <Core/App.h>

std::shared_ptr<App> g_app;

extern bool __cdecl Init()
{
	auto hr = S_OK;

	try
	{
		g_app = std::make_shared<App>();
		Clock::Init();
	}
	catch (std::exception e)
	{
		hr = S_FALSE;
	}

	if (!g_app)
	{
		hr = S_FALSE;
	}

	return hr == S_OK;
}

extern void __cdecl Render(void* resource, bool isNewResource)
{
	g_app->Render(resource, isNewResource);
}

extern bool __cdecl Cleanup()
{
	g_app.reset();

	return g_app == nullptr;
}

extern void __cdecl OnSimulationRestart(float edge, float density, float deviation, float angularVelocity, float integrationStep, bool gravityOn)
{
	g_app->RestartSimulation(edge, density, deviation, angularVelocity, integrationStep, gravityOn);
}

extern void __cdecl OnSimulationPause()
{
	g_app->PauseSimulation();
}

extern void __cdecl OnSimulationResume()
{
	g_app->ResumeSimulation();
}

extern void __cdecl MoveCamera(float dx, float dy)
{
	if (!g_app)
		return;

	g_app->MoveCamera(dx, dy);
}

extern void __cdecl Zoom(float dd)
{
	if (!g_app)
		return;

	g_app->Zoom(dd);
}

void __cdecl DisplayCubeChanged(bool display)
{
	if (g_app)
	{
		g_app->DisplayCubeChanged(display);
	}
}

void __cdecl DisplayDiagonalChanged(bool display)
{
	if (g_app)
	{
		g_app->DisplayDiagonalChanged(display);
	}
}

void __cdecl DisplayTraceChanged(bool display, uint32_t traceLength)
{
	if (g_app)
	{
		g_app->DisplayTraceChanged(display, traceLength);
	}
}

extern void __cdecl ToggleGravity(bool gravityOn)
{
	g_app->ToggleGravity(gravityOn);
}
