#pragma once
#include "pch.h"

namespace PerformanceComputing
{
	public value struct Particle
	{
		int x;
		int y;
		int z;
		double Vx;
		double Vy;
		double Vz;
	};

	struct MyEqual : public std::binary_function<const Particle, const Particle, bool>
	{
		bool operator()(const Particle& _Left, const Particle& _Right)
		{
			return _Left.x == _Right.x;
		};
	};
}

