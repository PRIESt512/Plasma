#pragma once
#include "Particle.h"

namespace Native
{
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

	private:
		int _largest;
		int _smallest;
		int _processor_count;
		int _count;
		const int _size = 99;
	};
}

namespace PerformanceComputing
{
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
			Windows::Foundation::Collections::IVector<int>^ get() { return ref new Platform::Collections::Vector<int>(std::move(particle_distribution->_electrons)); }
		}
		property Windows::Foundation::Collections::IVector<int>^ Heliums {
			Windows::Foundation::Collections::IVector<int>^ get() { return ref new Platform::Collections::Vector<int>(std::move(particle_distribution->_heliums)); }
		}
		property Windows::Foundation::Collections::IVector<int>^ Carbons {
			Windows::Foundation::Collections::IVector<int>^ get() { return ref new Platform::Collections::Vector<int>(std::move(particle_distribution->_carbons)); }
		}

	private:
		std::unique_ptr<Native::MaxwellParticleDistribution> particle_distribution;
		Platform::Collections::Vector<int>^ _electrons;
		Platform::Collections::Vector<int>^ _heliums;
		Platform::Collections::Vector<int>^ _carbons;
		Platform::Collections::Vector<Particle, equal_particle>^ red;
	};
}
