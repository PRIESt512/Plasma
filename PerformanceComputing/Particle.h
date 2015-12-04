#pragma once
#include "pch.h"

namespace PerformanceComputing
{
	value struct Particle
	{
		int x;
		int y;
		int z;
		double Vx;
		double Vy;
		double Vz;
	};

	struct equal_particle
	{
		bool operator()(const Particle& p1, const Particle& p2) const
		{
			return p1.x == p2.x;
		}
	};
}

