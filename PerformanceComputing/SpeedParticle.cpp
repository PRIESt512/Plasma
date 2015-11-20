#include "pch.h"
#include "SpeedParticle.h"


namespace Native
{
	SpeedParticle::SpeedParticle(int largest, int smallest) : _maxwell(std::make_unique<MaxwellParticleDistribution>(largest, smallest))
	{
	}

	SpeedParticle::SpeedParticle(int largest, int smallest, int processor_count) : _maxwell(std::make_unique<MaxwellParticleDistribution>(largest, smallest, processor_count))
	{
	}

	void SpeedParticle::DecompositionSpeed()
	{
		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> distr(mu, sigma);
		std::vector<PerformanceComputing::Particle> speedCarbons;

		auto count = SIZE_MOD * SIZE_MOD * SIZE_MOD;

		for (size_t i = 0; i < count; i++)
		{

		}

	}

	void SpeedParticle::DecompositionElectrons()
	{

	}

	void SpeedParticle::DecompositionCarbons()
	{
		auto discrete = *_maxwell->get_generator_distribution_carbon();

		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> normal(mu, sigma);
		std::vector<PerformanceComputing::Particle> speedCarbons;

		auto count = SIZE_MOD * SIZE_MOD;

		for (size_t i = 0; i < 9801; i++)
		{
			PerformanceComputing::Particle p;
			auto speed = discrete(gen) / CARBON_SPEED_DIMENSIONLESS_UNIT;
			auto angle = normal(gen);
			auto angle2 = normal(gen) + M_PI / 2;
			p.Vx = speed * cos(angle);
			p.Vy = speed * cos(angle2);
			p.Vz = abs(pow(p.Vx / cos(angle), 2) - pow(p.Vx, 2) - pow(p.Vy, 2));

			speedCarbons.push_back(p);
		}

	}

	void SpeedParticle::DecompositionHeliums()
	{
	}
}

namespace PerformanceComputing
{

	SpeedParticle::SpeedParticle(int largest, int smallest) : _speed(std::make_unique<Native::SpeedParticle>(largest, smallest))
	{
	}

	SpeedParticle::SpeedParticle(int largest, int smallest, int processor_count) : _speed(std::make_unique<Native::SpeedParticle>(largest, smallest, processor_count))
	{
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedAsync()
	{
		return concurrency::create_async([this]() {
			this->_speed->DecompositionCarbons();
		});
	}
}

//MaxwellParticleDistribution^ distribution = ref new MaxwellParticleDistribution(0, 7000);
//distribution->DistributionCarbons();
//auto carbons = distribution->Carbons;
//
//auto mu = 0;
//auto sigma = M_PI / 30;
//
//std::mt19937 gen;
//std::normal_distribution<> distr(mu, sigma);
//std::vector<Particle> speedCarbons;
//
//for (size_t i = 0; i < 9801; i++)
//{
//	Particle p;
//	auto speed = carbons->GetAt(i) / CARBON_SPEED_DIMENSIONLESS_UNIT;
//	auto angle = distr(gen);
//	auto angle2 = distr(gen) + M_PI / 2;
//	auto x_speed = speed * cos(angle);
//	auto y_speed = speed * cos(angle2);
//	auto z_speed = abs(pow(x_speed / cos(angle), 2) - pow(x_speed, 2) - pow(y_speed, 2));
//	p.Vx = x_speed;
//	p.Vy = y_speed;
//	p.Vz = z_speed;
//
//	speedCarbons.push_back(p);