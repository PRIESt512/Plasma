﻿#pragma once
#include "Constant.h"
#include <random>
#include <ppltasks.h>
#include <functional>
#include <ppl.h>
#include <vector>
#include <utility>

namespace Native
{
	//Нативный класс для расчета распределения Максвелла
	class MaxwellParticleDistribution
	{
	public:
		MaxwellParticleDistribution(int largest, int smallest);
		MaxwellParticleDistribution(int largest, int smallest, int processor_count);

		void DistributionElectrons();
		void DistributionCarbons();
		void DistributionHeliums();

		std::vector<int> _electrons;
		std::vector<int> _heliums;
		std::vector<int> _carbons;

		int get_processor_count() const;
		int get_count() const;

	protected:
		std::shared_ptr<std::discrete_distribution<>> get_generator_distribution_electron();
		std::shared_ptr<std::discrete_distribution<>> get_generator_distribution_helium();
		std::shared_ptr<std::discrete_distribution<>> get_generator_distribution_carbon();

	private:
		std::function<double(const double)> _electron_pdf;
		std::function<double(const double)> _carbon_pdf;
		std::function<double(const double)> _helium_pdf;

		int _largest;
		int _smallest;
		int _processor_count;
		int _count;
	};
}

namespace PerformanceComputing
{
	//Управляемая оболочка для нативного класса С++/СХ, позволяет взаимодействовать C# с неуправляемым кодом 
	public ref class MaxwellParticleDistribution sealed
	{
	public:
		MaxwellParticleDistribution(int largest, int smallest);
		MaxwellParticleDistribution(int largest, int smallest, int processor_count);

		Windows::Foundation::IAsyncAction^ DistributionParticleAsync();
		Windows::Foundation::IAsyncAction^ DistributionElectronsAsync();
		Windows::Foundation::IAsyncAction^ DistributionCarbonsAsync();
		Windows::Foundation::IAsyncAction^ DistributioHeliumsAsync();

		property Windows::Foundation::Collections::IVector<int>^ Electrons {
			Windows::Foundation::Collections::IVector<int>^ get() { return _electrons; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Heliums {
			Windows::Foundation::Collections::IVector<int>^ get() { return _carbons; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Carbons {
			Windows::Foundation::Collections::IVector<int>^ get() { return _heliums; }
		}

	private:
		std::unique_ptr<Native::MaxwellParticleDistribution> particle_distribution;
		Platform::Collections::Vector<int>^ _electrons;
		Platform::Collections::Vector<int>^ _heliums;
		Platform::Collections::Vector<int>^ _carbons;
	};
}
