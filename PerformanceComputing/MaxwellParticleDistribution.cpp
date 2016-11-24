#include "pch.h"
#include "MaxwellParticleDistribution.h"


//double carbon_pdf(double v)
//{
//	auto k = BOLTZMANN_CONSTANT;
//	auto m = CARBONS_MASS;
//	auto T = CARBONS_TEMPERATURE;
//	return 4 * M_PI * pow((m / (2 * M_PI * k * T)), 3 / 2) * pow(v, 2) * exp(-m * v * v / (2 * k * T));
//}
//
//double helium_pdf(double v)
//{
//	auto k = BOLTZMANN_CONSTANT;
//	auto m = HELIUMS_MASS;
//	auto T = HELIUMS_TEMPERATURE;
//	return 4 * M_PI * pow((m / (2 * M_PI * k * T)), 3 / 2) * pow(v, 2) * exp(-m * v * v / (2 * k * T));
//}
//
//double electron_pdf(double v)
//{
//	auto k = BOLTZMANN_CONSTANT;
//	auto m = ELECTRONS_MASS;
//	auto T = ELECTRONS_TEMPERATURE;
//	return 4 * M_PI * pow((m / (2 * M_PI * k * T)), 3 / 2) * pow(v, 2) * exp(-m * v * v / (2 * k * T));
//}

inline double pdf(const double v, const double m, const double T)
{
	return 4 * M_PI * pow((m / (2 * M_PI * BOLTZMANN_CONSTANT * T)), 3 / 2) * v*v * exp(-m * v * v / (2 * BOLTZMANN_CONSTANT * T));
}

namespace Native
{
	MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest)
		:_smallest(smallest), _largest(largest), _processor_count(2)
	{
		_count = ceil((pow(SIZE_MOD, 3) / this->_processor_count));

		_electron_pdf = std::bind(pdf, std::placeholders::_1, ELECTRONS_MASS, ELECTRONS_TEMPERATURE);
		_carbon_pdf = std::bind(pdf, std::placeholders::_1, CARBONS_MASS, CARBONS_TEMPERATURE);
		_helium_pdf = std::bind(pdf, std::placeholders::_1, HELIUMS_MASS, HELIUMS_TEMPERATURE);
	}

	MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest, int processor_count)
		: _smallest(smallest), _largest(largest), _processor_count(processor_count)
	{
		_count = ceil((pow(SIZE_MOD, 3) / this->_processor_count));

		_electron_pdf = std::bind(pdf, std::placeholders::_1, ELECTRONS_MASS, ELECTRONS_TEMPERATURE);
		_carbon_pdf = std::bind(pdf, std::placeholders::_1, CARBONS_MASS, CARBONS_TEMPERATURE);
		_helium_pdf = std::bind(pdf, std::placeholders::_1, HELIUMS_MASS, HELIUMS_TEMPERATURE);
	}

	void MaxwellParticleDistribution::DistributionElectrons()
	{
		std::vector<concurrency::task<std::vector<int>>> tasks_electrons(_processor_count);
		std::discrete_distribution<> discrete_temp(_largest, 0, _largest, _electron_pdf);
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()-> std::discrete_distribution<>
		{return discrete_temp; });

		for (size_t i = 0; i < _processor_count; i++)
		{
			tasks_electrons[i] = concurrency::create_task([&, this]() -> std::vector<int>
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::vector<int> discrete_electrons(_count);
				std::generate(discrete_electrons.begin(), discrete_electrons.end(), [&]() {return discrete.local()(gen); });
				return discrete_electrons;
			});
		}
		concurrency::when_all(begin(tasks_electrons), end(tasks_electrons)).then([this](std::vector<int> result)
		{
			_electrons = result;
		}).wait();
	}

	void MaxwellParticleDistribution::DistributionCarbons()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> discrete(_largest, 0, _largest, _carbon_pdf);
		int count = SIZE_MOD * SIZE_MOD;
		for (size_t i = 0; i < count; i++)
			_carbons.push_back(discrete(gen));
	}

	void MaxwellParticleDistribution::DistributionHeliums()
	{
		std::vector<concurrency::task<std::vector<int>>> tasks_heliums(_processor_count);
		std::discrete_distribution<> discrete_temp(_largest, 0, _largest, _helium_pdf);
		auto discrete = concurrency::combinable<std::discrete_distribution<>>([&]()->std::discrete_distribution<>
		{return discrete_temp; });

		for (size_t i = 0; i < _processor_count; i++)
		{
			tasks_heliums[i] = concurrency::create_task([&, this]() -> std::vector<int>
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::vector<int> discrete_heliums(_count);
				std::generate(discrete_heliums.begin(), discrete_heliums.end(), [&]() {return discrete.local()(gen); });
				return discrete_heliums;
			});
		}
		concurrency::when_all(begin(tasks_heliums), end(tasks_heliums)).then([this](std::vector<int> result)
		{
			_heliums = result;
		}).wait();
	}

	std::vector<double> MaxwellParticleDistribution::pdf_electrons()
	{
		auto temp = *get_generator_distribution_electron();
		return temp.probabilities();
	}

	std::vector<double> MaxwellParticleDistribution::pdf_carbons()
	{
		auto temp = *get_generator_distribution_carbon();
		return temp.probabilities();
	}

	std::vector<double> MaxwellParticleDistribution::pdf_heliums()
	{
		auto temp = *get_generator_distribution_helium();
		return temp.probabilities();
	}

	std::shared_ptr<std::discrete_distribution<>> MaxwellParticleDistribution::get_generator_distribution_electron()
	{
		return std::make_shared<std::discrete_distribution<>>(_largest, 0, _largest, _electron_pdf);
	}

	std::shared_ptr<std::discrete_distribution<>> MaxwellParticleDistribution::get_generator_distribution_carbon()
	{
		return std::make_shared<std::discrete_distribution<>>(_largest, 0, _largest, _carbon_pdf);
	}

	std::shared_ptr<std::discrete_distribution<>> MaxwellParticleDistribution::get_generator_distribution_helium()
	{
		return std::make_shared<std::discrete_distribution<>>(_largest, 0, _largest, _helium_pdf);
	}

	int MaxwellParticleDistribution::get_count() const
	{
		return _count;
	}

	int MaxwellParticleDistribution::get_processor_count() const
	{
		return _processor_count;
	}
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace PerformanceComputing
{
	MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest)
		: particle_distribution(Native::MaxwellParticleDistribution(smallest, largest))
	{
	}

	MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest, int processor_count)
		: particle_distribution(Native::MaxwellParticleDistribution(smallest, largest, processor_count))
	{
	}

	Windows::Foundation::IAsyncAction^ MaxwellParticleDistribution::DistributionParticleAsync()
	{
		return concurrency::create_async([this]()
		{ concurrency::parallel_invoke(
			[this]
		{
			this->particle_distribution.DistributionHeliums();
			this->_heliums = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._heliums));
		},
			[this]
		{
			this->particle_distribution.DistributionCarbons();
			this->_carbons = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._carbons));
		},
			[this]
		{
			this->particle_distribution.DistributionElectrons();
			this->_electrons = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._electrons));
		}); });
	}

	Windows::Foundation::IAsyncAction^ MaxwellParticleDistribution::DistributionElectronsAsync()
	{
		return concurrency::create_async([this]
		{
			this->particle_distribution.DistributionElectrons();
			this->_electrons = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._electrons));
		});
	}

	Windows::Foundation::IAsyncAction^ MaxwellParticleDistribution::DistributionCarbonsAsync()
	{
		return concurrency::create_async([this]
		{
			this->particle_distribution.DistributionCarbons();
			this->_carbons = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._carbons));
		});
	}

	Windows::Foundation::IAsyncAction^ MaxwellParticleDistribution::DistributioHeliumsAsync()
	{
		return concurrency::create_async([this]
		{
			this->particle_distribution.DistributionHeliums();
			this->_heliums = ref new Platform::Collections::Vector<int>(std::move(particle_distribution._heliums));
		});
	}

	Windows::Foundation::Collections::IVector<double>^ MaxwellParticleDistribution::ElectronsPdf()
	{
		return ref new Platform::Collections::Vector<double>(std::move(particle_distribution.pdf_electrons()));
	}

	Windows::Foundation::Collections::IVector<double>^ MaxwellParticleDistribution::CarbonsPdf()
	{
		return ref new Platform::Collections::Vector<double>(std::move(particle_distribution.pdf_carbons()));
	}

	Windows::Foundation::Collections::IVector<double>^ MaxwellParticleDistribution::HeliumsPdf()
	{
		return ref new Platform::Collections::Vector<double>(std::move(particle_distribution.pdf_heliums()));
	}
}