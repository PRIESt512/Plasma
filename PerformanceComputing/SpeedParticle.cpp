#include "pch.h"
#include "SpeedParticle.h"


namespace Native
{
	SpeedParticle::SpeedParticle(int smallest, int largest) : MaxwellParticleDistribution(smallest, largest)
	{
	}

	SpeedParticle::SpeedParticle(int smallest, int largest, int processor_count) : MaxwellParticleDistribution(smallest, largest, processor_count)
	{
	}

	void SpeedParticle::DecompositionElectrons(bool DIMENSIONLESS)
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = ELECTRON_SPEED_DIMENSIONLESS_UNIT;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<PerformanceComputing::Particle>>> tasks_decomposition_electrons(processor_count);

		auto discrete_temp = this->get_generator_distribution_electron();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_electrons[i] = concurrency::create_task([&, this]() -> std::vector<PerformanceComputing::Particle>
			{
				std::vector<PerformanceComputing::Particle> speed_particle(count);
				std::random_device rd;
				std::mt19937 gen(rd());
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> PerformanceComputing::Particle
				{
					PerformanceComputing::Particle p;
					auto speed = discrete.local()(gen) / dimensionless;
					auto angle = normal.local()(gen);
					auto angle2 = normal.local()(gen) + M_PI / 2;
					auto temp_cos = cos(angle);

					p.Vx = speed * temp_cos;
					p.Vy = speed * cos(angle2);
					p.Vz = sqrt(abs(pow(p.Vx / temp_cos, 2) - p.Vx*p.Vx - p.Vy*p.Vy));
					return p;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_electrons), end(tasks_decomposition_electrons)).then([this](std::vector<PerformanceComputing::Particle>& result)
		{
			_speed_electrons = result;
		}).wait();
	};

	void SpeedParticle::DecompositionElectronsOne()
	{
		auto discrete = this->get_generator_distribution_electron();
		auto count = this->get_count();

		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> normal(mu, sigma);

		for (size_t i = 0; i < 1100000; i++)
		{
			PerformanceComputing::Particle p;
			auto speed = (*discrete)(gen) / ELECTRON_SPEED_DIMENSIONLESS_UNIT;
			auto angle = normal(gen);
			auto angle2 = normal(gen) + M_PI / 2;
			auto temp_cos = cos(angle);

			p.Vx = speed * temp_cos;
			p.Vy = speed * cos(angle2);
			p.Vz = sqrt(abs(pow(p.Vx / temp_cos, 2) - p.Vx*p.Vx - p.Vy*p.Vy));
			_speed_electrons.push_back(p);
		}

	}

	void SpeedParticle::DecompositionCarbons(bool DIMENSIONLESS)
	{
		auto discrete = this->get_generator_distribution_carbon();

		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> normal(mu, sigma);

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = CARBON_SPEED_DIMENSIONLESS_UNIT;

		auto count = SIZE_MOD * SIZE_MOD;

		for (size_t i = 0; i < count; i++)
		{
			PerformanceComputing::Particle p;
			auto speed = (*discrete)(gen) / dimensionless;
			auto angle = normal(gen);
			auto angle2 = normal(gen) + M_PI / 2;
			auto temp_cos = cos(angle);

			p.Vx = speed * temp_cos;
			p.Vy = speed * cos(angle2);
			p.Vz = sqrt(abs(pow(p.Vx / temp_cos, 2) - p.Vx*p.Vx - p.Vy*p.Vy));

			p.x = normal(gen);
			p.y = normal(gen);
			p.z = normal(gen);


			_speed_carbons.push_back(p);
		}
	}

	void SpeedParticle::DecompositionHeliums(bool DIMENSIONLESS)
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = HELLIUM_SPEED_DIMENSIONLESS_UNIT;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<PerformanceComputing::Particle>>> tasks_decomposition_heliums(processor_count);

		auto discrete_temp = this->get_generator_distribution_helium();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_heliums[i] = concurrency::create_task([&, this]() -> const std::vector<PerformanceComputing::Particle>
			{
				std::vector<PerformanceComputing::Particle> speed_particle(count);
				std::mt19937 gen;
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> PerformanceComputing::Particle
				{
					PerformanceComputing::Particle p;
					auto speed = discrete.local()(gen) / dimensionless;
					auto angle = normal.local()(gen);
					auto angle2 = normal.local()(gen) + M_PI / 2;
					auto temp_cos = cos(angle);

					p.Vx = speed * temp_cos;
					p.Vy = speed * cos(angle2);
					p.Vz = sqrt(abs(pow(p.Vx / temp_cos, 2) - p.Vx*p.Vx - p.Vy*p.Vy));
					return p;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_heliums), end(tasks_decomposition_heliums)).then([this](const std::vector<PerformanceComputing::Particle> &result)
		{
			_speed_heliums = result;
		}).wait();
	}


	void SpeedParticle::FullSpeedElectrons(bool DIMENSIONLESS)
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = ELECTRON_SPEED_DIMENSIONLESS_UNIT;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<double>>> tasks_decomposition_electrons(processor_count);

		auto discrete_temp = this->get_generator_distribution_electron();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_electrons[i] = concurrency::create_task([&, this]() -> std::vector<double>
			{
				std::vector<double> speed_particle(count);
				std::random_device rd;
				std::mt19937 gen(rd());
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> double
				{
					auto speed = discrete.local()(gen) / dimensionless;
					return speed;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_electrons), end(tasks_decomposition_electrons)).then([this](std::vector<double>& result)
		{
			_full_speed_electrons = result;
		}).wait();
	}

	void SpeedParticle::FullSpeedCarbons(bool DIMENSIONLESS)
	{

		auto discrete = this->get_generator_distribution_carbon();

		auto mu = 0;
		auto sigma = M_PI / 30;
		std::mt19937 gen;
		std::normal_distribution<> normal(mu, sigma);

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = CARBON_SPEED_DIMENSIONLESS_UNIT;

		auto count = SIZE_MOD * SIZE_MOD;

		for (size_t i = 0; i < count; i++)
		{
			auto speed = (*discrete)(gen) / dimensionless;
			_full_speed_carbons.push_back(speed);
		}
	}

	void SpeedParticle::FullSpeedHeliums(bool DIMENSIONLESS)
	{
		auto count = this->get_count();
		auto processor_count = this->get_processor_count();
		auto mu = 0;
		auto sigma = M_PI / 30;

		auto dimensionless = 1.0;
		if (DIMENSIONLESS)
			dimensionless = HELLIUM_SPEED_DIMENSIONLESS_UNIT;

		std::normal_distribution<> normal_temp(mu, sigma);
		std::vector<concurrency::task<std::vector<double>>> tasks_decomposition_heliums(processor_count);

		auto discrete_temp = this->get_generator_distribution_helium();
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<> {return *discrete_temp; });
		auto normal = concurrency::combinable<std::normal_distribution<>>([&]() -> std::normal_distribution<> {return normal_temp; });

		for (size_t i = 0; i < processor_count; i++)
		{
			tasks_decomposition_heliums[i] = concurrency::create_task([&, this]() -> const std::vector<double>
			{
				std::vector<double> speed_particle(count);
				std::mt19937 gen;
				std::generate(speed_particle.begin(), speed_particle.end(), [&]() -> double
				{
					auto speed = discrete.local()(gen) / dimensionless;
					return speed;
				});
				return speed_particle;
			});
		}
		concurrency::when_all(begin(tasks_decomposition_heliums), end(tasks_decomposition_heliums)).then([this](const std::vector<double> &result)
		{
			_full_speed_heliums = result;
		}).wait();
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace PerformanceComputing
{
	SpeedParticle::SpeedParticle(int smallest, int largest) : _speed(new Native::SpeedParticle(smallest, largest))
	{
	}

	SpeedParticle::SpeedParticle(int smallest, int largest, int processor_count) : _speed(new Native::SpeedParticle(smallest, largest, processor_count))
	{
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]()
		{ concurrency::parallel_invoke(
			[this, DIMENSIONLESS]
		{
			this->_speed->DecompositionElectrons(DIMENSIONLESS);
			this->_speedElectrons = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_electrons));
		},
			[this, DIMENSIONLESS]
		{
			this->_speed->DecompositionCarbons(DIMENSIONLESS);
			this->_speedCarbons = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_carbons));
		},
			[this, DIMENSIONLESS] {this->_speed->DecompositionHeliums(DIMENSIONLESS);
		});
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedElectronsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->DecompositionElectrons(DIMENSIONLESS);
			this->_speedElectrons = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_electrons));
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedElectronsAsyncOne()
	{
		return concurrency::create_async([this]() {
			this->_speed->DecompositionElectronsOne();
			this->_speedElectrons = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_electrons));
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedCarbonsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->DecompositionCarbons(DIMENSIONLESS);
			this->_speedCarbons = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_carbons));
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::DecompositionSpeedHeliumsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->DecompositionHeliums(DIMENSIONLESS);
			this->_speedHeliums = ref new Platform::Collections::Vector<Particle, MyEqual>(move(this->_speed->_speed_heliums));
		});
	}


	Windows::Foundation::IAsyncAction^ SpeedParticle::FullSpeedAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]()
		{ concurrency::parallel_invoke(
			[this, DIMENSIONLESS]()->void
		{
			this->_speed->FullSpeedElectrons(DIMENSIONLESS);
			this->_full_speedElectrons = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_electrons));
		},
			[this, DIMENSIONLESS]()->void
		{
			this->_speed->FullSpeedCarbons(DIMENSIONLESS);
			this->_full_speedCarbons = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_carbons));
		},
			[this, DIMENSIONLESS]()->void
		{
			this->_speed->FullSpeedHeliums(DIMENSIONLESS);
			this->_full_speedHeliums = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_heliums));
		});
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::SpeedElectronsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->DecompositionElectrons(DIMENSIONLESS);
			this->_full_speedElectrons = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_electrons));
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::SpeedCarbonsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->FullSpeedCarbons(DIMENSIONLESS);
			this->_full_speedCarbons = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_carbons));
		});
	}

	Windows::Foundation::IAsyncAction^ SpeedParticle::SpeedHeliumsAsync(bool DIMENSIONLESS)
	{
		return concurrency::create_async([this, DIMENSIONLESS]() {
			this->_speed->FullSpeedHeliums(DIMENSIONLESS);
			this->_full_speedHeliums = ref new Platform::Collections::Vector<double>(move(this->_speed->_full_speed_heliums));
		});
	}
}