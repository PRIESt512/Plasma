#pragma once
#include <functional>
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
			Windows::Foundation::Collections::IVector<int>^ get() { return electrons; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Heliums {
			Windows::Foundation::Collections::IVector<int>^ get() { return heliums; }
		}
		property Windows::Foundation::Collections::IVector<int>^ Carbons {
			Windows::Foundation::Collections::IVector<int>^ get() { return carbons; }
		}

	private:
		Platform::Collections::Vector<int>^ electrons;
		Platform::Collections::Vector<int>^ heliums;
		Platform::Collections::Vector<int>^ carbons;

		int largest;
		int smallest;
		int processor_count;
		int count;
		const int size = 99;
	};
}
