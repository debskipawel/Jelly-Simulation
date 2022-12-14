// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
// -- windows header files --
#include <windows.h>

// -- error handling --
#include <exception>

// -- io --
#include <iostream>
#include <fstream>

#include <filesystem>

// -- utils --
#include <Utils/Clock.h>
#include <Utils/Random.h>

// -- directX --
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <algorithm>
#include <execution>
#include <numeric>
#include <limits>

// -- smart pointers --
#include <memory>

// -- data structures --
#include <vector>
#include <map>
#include <set>
#include <array>

#endif //PCH_H
