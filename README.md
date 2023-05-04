# JellySimulation

A soft body physics simulation, made by approximating the jelly cube with a Bezier cube made of 64 points, connected with springs.
It was implemented using WPF & C# for GUI, and C++ with DirectX 11 for the simulation loop and graphics.

The graphics engine is ECS-based, and was my first try of implementing this pattern. 

## Build

Make sure to clone the repo with the `--recursive` flag to make sure all GitHub Modules dependencies are cloned also.

The project ships as a Visual Studio solution with all dependencies already attached and linked. Just launch the project in Visual Studio 2022 and build it.

Naturally, this means it's meant only for Windows.

___
## Video


https://user-images.githubusercontent.com/52053209/236257940-5b1a84d1-c729-4406-8269-b06aa21afe0d.mp4


