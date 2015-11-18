#pragma once
#include "Particle.h"

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

		void DistributionParticle();
		void DistributionElectrons();
		void DistributionCarbons();
		void DistributionHeliums();

		property Windows::Foundation::Collections::IVector<int>^ Electrons {
			Windows::Foundation::Collections::IVector<int>^ get() { return _electrons; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Heliums {
			Windows::Foundation::Collections::IVector<int>^ get() { return _heliums; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Carbons {
			Windows::Foundation::Collections::IVector<int>^ get() { return _carbons; }
		}

	private:
		Platform::Collections::Vector<int>^ _electrons;
		Platform::Collections::Vector<int>^ _heliums;
		Platform::Collections::Vector<int>^ _carbons;
		Platform::Collections::Vector<Particle, equal_particle>^ red;

		int _largest;
		int _smallest;
		int _processor_count;
		int _count;
		const int _size = 99;
	};
}
