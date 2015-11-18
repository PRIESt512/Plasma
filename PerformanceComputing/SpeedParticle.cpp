#include "pch.h"
#include "SpeedParticle.h"
#include "MaxwellParticleDistribution.h"
#include <random>
#include "Constant.h"

namespace PerformanceComputing
{
	SpeedParticle::SpeedParticle()
	{

	}
	Windows::Foundation::IAsyncAction^ SpeedParticle::SpeedCompare()
	{
		return concurrency::create_async([]() {
			MaxwellParticleDistribution^ distribution = ref new MaxwellParticleDistribution(0, 7000);
			distribution->DistributionCarbons();
			auto carbons = distribution->Carbons;

			auto mu = 0;
			auto sigma = M_PI / 30;

			std::mt19937 gen;
			std::normal_distribution<> distr(mu, sigma);
			std::vector<Particle> speedCarbons;

			for (size_t i = 0; i < 9801; i++)
			{
				Particle p;
				auto speed = carbons->GetAt(i) / CARBON_SPEED_DIMENSIONLESS_UNIT;
				auto angle = distr(gen);
				auto angle2 = distr(gen) + M_PI / 2;
				auto x_speed = speed * cos(angle);
				auto y_speed = speed * cos(angle2);
				auto z_speed = abs(pow(x_speed / cos(angle), 2) - pow(x_speed, 2) - pow(y_speed, 2));
				p.Vx = x_speed;
				p.Vy = y_speed;
				p.Vz = z_speed;

				speedCarbons.push_back(p);
			}
		});
	}
}
