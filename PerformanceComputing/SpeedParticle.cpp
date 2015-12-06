#include "pch.h"
#include "SpeedParticle.h"


namespace Native
{
	SpeedParticle::SpeedParticle(int largest, int smallest) : MaxwellParticleDistribution(largest, smallest)
	{
	}

	SpeedParticle::SpeedParticle(int largest, int smallest, int processor_count) : MaxwellParticleDistribution(largest, smallest, processor_count)
	{
	}

	void SpeedParticle::DecompositionElectrons()
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<PerformanceComputing::Particle*>>> tasks_decomposition_electrons(processor_count);

		auto discrete_temp = this->get_generator_distribution_electron();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_electrons[i] = concurrency::create_task([&, this]() -> std::vector<PerformanceComputing::Particle*>
			{
				std::vector<PerformanceComputing::Particle*> speed_particle(count);
				std::random_device rd;
				std::mt19937 gen(rd());
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> PerformanceComputing::Particle*
				{
					auto p = new PerformanceComputing::Particle();
					auto speed = discrete.local()(gen) / ELECTRON_SPEED_DIMENSIONLESS_UNIT;
					auto angle = normal.local()(gen);
					auto angle2 = normal.local()(gen) + M_PI / 2;
					auto temp_cos = cos(angle);

					p->Vx = speed * temp_cos;
					p->Vy = speed * cos(angle2);
					p->Vz = sqrt(abs(pow(p->Vx / temp_cos, 2) - p->Vx*p->Vx - p->Vy*p->Vy));
					return p;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_electrons), end(tasks_decomposition_electrons)).then([this](std::vector<PerformanceComputing::Particle*>& result)
		{
			_speed_electrons = result;
		}).wait();
	};

	void SpeedParticle::DecompositionCarbons()
	{
		auto discrete = this->get_generator_distribution_carbon();

		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> normal(mu, sigma);

		auto count = SIZE_MOD * SIZE_MOD;

		for (size_t i = 0; i < count; i++)
		{
			auto p = new PerformanceComputing::Particle();
			auto speed = (*discrete)(gen) / CARBON_SPEED_DIMENSIONLESS_UNIT;
			auto angle = normal(gen);
			auto angle2 = normal(gen) + M_PI / 2;
			auto temp_cos = cos(angle);

			p->Vx = speed * temp_cos;
			p->Vy = speed * cos(angle2);
			p->Vz = sqrt(abs(pow(p->Vx / temp_cos, 2) - p->Vx*p->Vx - p->Vy*p->Vy));
			_speed_carbons.push_back(p);
		}
	}

	void SpeedParticle::DecompositionHeliums()
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<PerformanceComputing::Particle*>>> tasks_decomposition_heliums(processor_count);

		auto discrete_temp = this->get_generator_distribution_helium();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_heliums[i] = concurrency::create_task([&, this]() -> const std::vector<PerformanceComputing::Particle*>
			{
				std::vector<PerformanceComputing::Particle*> speed_particle(count);
				std::mt19937 gen;
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> PerformanceComputing::Particle*
				{
					auto p = new PerformanceComputing::Particle();
					auto speed = discrete.local()(gen) / ELECTRON_SPEED_DIMENSIONLESS_UNIT;
					auto angle = normal.local()(gen);
					auto angle2 = normal.local()(gen) + M_PI / 2;
					auto temp_cos = cos(angle);

					p->Vx = speed * temp_cos;
					p->Vy = speed * cos(angle2);
					p->Vz = sqrt(abs(pow(p->Vx / temp_cos, 2) - p->Vx*p->Vx - p->Vy*p->Vy));
					return p;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_heliums), end(tasks_decomposition_heliums)).then([this](const std::vector<PerformanceComputing::Particle*> &result)
		{
			_speed_heliums = result;
		}).wait();
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace PerformanceComputing
{

	SpeedParticle::SpeedParticle(int largest, int smallest) : _speed(new Native::SpeedParticle(largest, smallest))
	{
	}

	SpeedParticle::SpeedParticle(int largest, int smallest, int processor_count) : _speed(new Native::SpeedParticle(largest, smallest, processor_count))
	{
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedAsync()
	{
		return concurrency::create_async([this]()
		{ concurrency::parallel_invoke(
			[this] {this->_speed->DecompositionElectrons(); },
			[this] {this->_speed->DecompositionCarbons(); },
			[this] {this->_speed->DecompositionHeliums(); }
		);
		});
	}

	Windows::Foundation::IAsyncAction ^ SpeedParticle::DecompositionSpeedElectronsAsync()
	{
		return concurrency::create_async([this]() {
			this->_speed->DecompositionElectrons();
		});
	}

	Windows::Foundation::IAsyncAction ^ SpeedParticle::DecompositionSpeedCarbonsAsync()
	{
		return concurrency::create_async([this]() {
			this->_speed->DecompositionCarbons();
		});
	}

	Windows::Foundation::IAsyncAction ^ SpeedParticle::DecompositionSpeedHeliumsAsync()
	{
		return concurrency::create_async([this]() {
			this->_speed->DecompositionHeliums();
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