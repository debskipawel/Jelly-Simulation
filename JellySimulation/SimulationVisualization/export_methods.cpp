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

extern void __cdecl MoveOnLeftClick(float dx, float dy)
{
	if (!g_app)
		return;

	g_app->MoveSteeringCube(dx, -dy);
}

extern void __cdecl MoveOnMiddleClick(float dx, float dy)
{
	if (!g_app)
		return;

	g_app->MoveCamera(dx, dy);
}

extern void __cdecl MoveOnRightClick(float dx, float dy)
{
}

extern void __cdecl Scroll(float dd)
{
	if (!g_app)
		return;

	g_app->Zoom(dd);
}

void __cdecl UpdateVisualizationParameters(bool drawControlPoints, bool drawSteeringCube, bool drawShadedCube, bool drawBoundingCuboid)
{
	if (!g_app)
		return;
	// TODO: uncomment
	// g_app->UpdateVisualizationParameters(drawControlPoints, drawSteeringCube, drawShadedCube);
}

void __cdecl RestartSimulation(float pointMass, float stickiness, float massesElasticity, float steeringSpringsElasticity, float steeringElasticyOnCollisions, float maxImbalance)
{
	if (!g_app)
		return;

	g_app->RestartSimulation(pointMass, stickiness, massesElasticity, steeringSpringsElasticity, steeringElasticyOnCollisions, maxImbalance);
}
