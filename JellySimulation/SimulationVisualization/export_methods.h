#pragma once

extern "C"
{
	__declspec(dllexport) bool __cdecl Init();
	__declspec(dllexport) void __cdecl Render(void* resource, bool isNewResource);
	__declspec(dllexport) bool __cdecl Cleanup();

	__declspec(dllexport) void __cdecl MoveOnLeftClick(float dx, float dy);
	__declspec(dllexport) void __cdecl MoveOnMiddleClick(float dx, float dy);
	__declspec(dllexport) void __cdecl MoveOnRightClick(float dx, float dy);
	__declspec(dllexport) void __cdecl Scroll(float dd);
	
	__declspec(dllexport) void __cdecl UpdateVisualizationParameters(bool drawControlPoints, bool drawSteeringCube, bool drawShadedCube, bool drawBoundingCuboid);
	__declspec(dllexport) void __cdecl RestartSimulation(float pointMass, float stickiness, float massesElasticity, float steeringSpringsElasticity, float steeringElasticyOnCollisions, float maxImbalance);
}
