#include "pch.h"
#include "MaxwellParticleDistribution.h"
#include "Constant.h"
#include <random>
#include <ppltasks.h>
#include <functional>
#include <ppl.h>
#include <vector>
#include <utility>

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
	return 4 * M_PI * pow((m / (2 * M_PI * BOLTZMANN_CONSTANT * T)), 3 / 2) * pow(v, 2) * exp(-m * v * v / (2 * BOLTZMANN_CONSTANT * T));
}

Native::MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest)
	:_smallest(smallest), _largest(largest), _processor_count(2)
{
	_count = ceil((pow(99, 3) / this->_processor_count));
}

Native::MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest, int processor_count)
	: _smallest(smallest), _largest(largest), _processor_count(processor_count)
{
	_count = ceil((pow(99, 3) / this->_processor_count));
}

void Native::MaxwellParticleDistribution::DistributionElectrons()
{
	std::function<double(const double)> electron_pdf(std::bind(pdf, std::placeholders::_1, ELECTRONS_MASS, ELECTRONS_TEMPERATURE));
	std::vector<concurrency::task<std::vector<int>>> tasks_electrons(_processor_count);
	std::discrete_distribution<> discrete_temp(this->_largest, 0, this->_largest, electron_pdf);
	concurrency::combinable<std::discrete_distribution<>> discrete = concurrency::combinable<std::discrete_distribution<>>([&]()->std::discrete_distribution<>
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

void Native::MaxwellParticleDistribution::DistributionCarbons()
{
	std::function<double(const double)> carbon_pdf(std::bind(pdf, std::placeholders::_1, CARBONS_MASS, CARBONS_TEMPERATURE));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> discrete(this->_largest, 0, this->_largest, carbon_pdf);
	int count = 99 * 99;
	for (size_t i = 0; i < count; i++)
		_carbons.push_back(discrete(gen));
}

void Native::MaxwellParticleDistribution::DistributionHeliums()
{
	std::function<double(const double)> helium_pdf(std::bind(pdf, std::placeholders::_1, HELIUMS_MASS, HELIUMS_TEMPERATURE));
	std::vector<concurrency::task<std::vector<int>>> tasks_heliums(_processor_count);
	std::discrete_distribution<> discrete_temp(this->_largest, 0, this->_largest, helium_pdf);
	concurrency::combinable<std::discrete_distribution<>> discrete = concurrency::combinable<std::discrete_distribution<>>([&]()->std::discrete_distribution<>
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------


PerformanceComputing::MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest)
	: particle_distribution(std::make_unique <Native::MaxwellParticleDistribution>(smallest, largest))
{
}

PerformanceComputing::MaxwellParticleDistribution::MaxwellParticleDistribution(int smallest, int largest, int processor_count)
	: particle_distribution(std::make_unique <Native::MaxwellParticleDistribution>(smallest, largest, processor_count))
{
}

Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionParticleAsync()
{
	return concurrency::create_async([this]() { concurrency::parallel_invoke(
		[this] {this->particle_distribution->DistributionCarbons(); },
		[this] {this->particle_distribution->DistributionHeliums(); },
		[this] {this->particle_distribution->DistributionElectrons(); }
	);
	});
}

Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionElectronsAsync()
{
	return concurrency::create_async([this]
	{
		this->particle_distribution->DistributionElectrons();
	});
}

Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionCarbonsAsync()
{
	return concurrency::create_async([this]
	{
		this->particle_distribution->DistributionCarbons();
	});
}

Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributioHeliumsAsync()
{
	return concurrency::create_async([this]
	{
		this->particle_distribution->DistributionHeliums();
	});
}

//void PerformanceComputing::MaxwellParticleDistribution::DistributionParticle()
//{
//	concurrency::parallel_invoke(
//		[this] {this->particle_distribution ->DistributionCarbons(); },
//		[this] {this->DistributionHeliums(); },
//		[this] {this->DistributionElectrons(); }
//	);
//}
//
//void PerformanceComputing::MaxwellParticleDistribution::DistributionElectrons()
//{
//	std::function<double(const double)> electron_pdf(std::bind(pdf, std::placeholders::_1, ELECTRONS_MASS, ELECTRONS_TEMPERATURE));
//	std::vector<concurrency::task<std::vector<int>>> tasks_electrons(_processor_count);
//	std::discrete_distribution<> discrete_temp(this->_largest, 0, this->_largest, electron_pdf);
//	concurrency::combinable<std::discrete_distribution<>> discrete = concurrency::combinable<std::discrete_distribution<>>([&]()->std::discrete_distribution<>
//	{return discrete_temp; });
//	red = ref new Platform::Collections::Vector<Particle, equal_particle>();
//
//	for (size_t i = 0; i < _processor_count; i++)
//	{
//		tasks_electrons[i] = concurrency::create_task([&, this]() -> std::vector<int>
//		{
//			std::random_device rd;
//			std::mt19937 gen(rd());
//			std::vector<int> discrete_electrons(_count);
//			std::generate(discrete_electrons.begin(), discrete_electrons.end(), [&]() {return discrete.local()(gen); });
//			return discrete_electrons;
//		});
//	}
//	concurrency::when_all(begin(tasks_electrons), end(tasks_electrons)).then([this](std::vector<int> result)
//	{
//		this->_electrons = ref new Platform::Collections::Vector<int>(std::move(result));
//	}).wait();
//
//	/*concurrency::combinable<std::vector<int>> vect = concurrency::combinable<std::vector<int>>([&temp_v]()->std::vector<int>
//	{return temp_v; });
//
//	concurrency::parallel_for(1, 5, [&](int i)
//	{
//		std::random_device rd;
//		std::mt19937 gen(rd());
//		std::generate(vect.local().begin(), vect.local().end(), [&]() {return discrete.local()(gen); });
//	}, concurrency::static_partitioner());
//
//
//	vect.combine_each([&](std::vector<int>& local) -> void
//	{
//		distribution_electrons.insert(distribution_electrons.end(), local.begin(), local.end());
//	});
//
//	this->electrons = ref new Platform::Collections::Vector<int>(std::move(distribution_electrons));*/
//}
//
//void PerformanceComputing::MaxwellParticleDistribution::DistributionCarbons()
//{
//	std::function<double(const double)> carbon_pdf(std::bind(pdf, std::placeholders::_1, CARBONS_MASS, CARBONS_TEMPERATURE));
//	std::vector<int> distribution;
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::discrete_distribution<> discrete(this->_largest, 0, this->_largest, carbon_pdf);
//	int count = 99 * 99;
//	for (size_t i = 0; i < count; i++)
//		distribution.push_back(discrete(gen));
//
//	this->_carbons = ref new Platform::Collections::Vector<int>(std::move(distribution));
//}
//
//void PerformanceComputing::MaxwellParticleDistribution::DistributionHeliums()
//{
//	std::function<double(const double)> helium_pdf(std::bind(pdf, std::placeholders::_1, HELIUMS_MASS, HELIUMS_TEMPERATURE));
//	std::vector<concurrency::task<std::vector<int>>> tasks_heliums(_processor_count);
//	std::discrete_distribution<> discrete_temp(this->_largest, 0, this->_largest, helium_pdf);
//	concurrency::combinable<std::discrete_distribution<>> discrete = concurrency::combinable<std::discrete_distribution<>>([&]()->std::discrete_distribution<>
//	{return discrete_temp; });
//
//	for (size_t i = 0; i < _processor_count; i++)
//	{
//		tasks_heliums[i] = concurrency::create_task([&, this]() -> std::vector<int>
//		{
//			std::random_device rd;
//			std::mt19937 gen(rd());
//			std::vector<int> discrete_heliums(_count);
//			std::generate(discrete_heliums.begin(), discrete_heliums.end(), [&]() {return discrete.local()(gen); });
//			return discrete_heliums;
//		});
//	}
//	concurrency::when_all(begin(tasks_heliums), end(tasks_heliums)).then([this](std::vector<int> result)
//	{
//		this->_heliums = ref new Platform::Collections::Vector<int>(std::move(result));
//	}).wait();
//}
//
//Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionParticleAsync()
//{
//	return concurrency::create_async([this]
//	{
//		this->DistributionParticle();
//	});
//}
//
//Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionElectronsAsync()
//{
//	return concurrency::create_async([this]
//	{
//		this->DistributionElectrons();
//	});
//}
//
//Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributionCarbonsAsync()
//{
//	return concurrency::create_async([this]
//	{
//		this->DistributionCarbons();
//	});
//}
//
//Windows::Foundation::IAsyncAction^ PerformanceComputing::MaxwellParticleDistribution::DistributioHeliumsAsync()
//{
//	return concurrency::create_async([this]
//	{
//		this->DistributionHeliums();
//	});
//}
