#pragma once

extern "C"
{
	__declspec(dllexport) bool __cdecl Init();
	__declspec(dllexport) void __cdecl Render(void* resource, bool isNewResource);
	__declspec(dllexport) bool __cdecl Cleanup();

	__declspec(dllexport) void __cdecl OnSimulationRestart(float, float, float, float, float, bool);
	__declspec(dllexport) void __cdecl OnSimulationPause();
	__declspec(dllexport) void __cdecl OnSimulationResume();

	__declspec(dllexport) void __cdecl MoveCamera(float dx, float dy);
	__declspec(dllexport) void __cdecl Zoom(float dd);

	__declspec(dllexport) void __cdecl DisplayCubeChanged(bool display);
	__declspec(dllexport) void __cdecl DisplayDiagonalChanged(bool display);
	__declspec(dllexport) void __cdecl DisplayTraceChanged(bool display, uint32_t traceLength);
	__declspec(dllexport) void __cdecl ToggleGravity(bool gravityOn);
}
