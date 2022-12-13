#pragma once
#include <functional>
#include "SimpleMath.h"

template <typename T>
T RungeKutty(double x0, T y0, double h, std::function<T(double,T)> fun)
{
	T y1;
	T k1, k2, k3, k4;
	k1 = h * fun(x0, y0);
	k2 = h * fun(x0 + h / 2., y0 + k1 * (1 / 2.));
	k3 = h * fun(x0 + h / 2., y0 + k2 * (1 / 2.));
	k4 = h * fun(x0 + h, y0 + k3);
	y1 = y0 + (k1 + 2. * k2 + 2. * k3 + k4) * (1 / 6.);
	return y1;
}

