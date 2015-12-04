#pragma once
#include "Particle.h"
#include "MaxwellParticleDistribution.h"
#include <random>
#include <memory>

namespace Native
{
	class SpeedParticle : protected MaxwellParticleDistribution
	{
	public:
		SpeedParticle(int largest, int smallest);
		SpeedParticle(int largest, int smallest, int processor_count);

		void DecompositionElectrons();
		void DecompositionCarbons();
		void DecompositionHeliums();

		std::vector<PerformanceComputing::Particle*> _speed_electrons;
		std::vector<PerformanceComputing::Particle*> _speed_carbons;
		std::vector<PerformanceComputing::Particle*> _speed_heliums;
	};

}

namespace PerformanceComputing
{
	public ref class SpeedParticle sealed
	{
	public:
		SpeedParticle(int largest, int smallest);
		SpeedParticle(int largest, int smallest, int processor_count);

		/*property Windows::Foundation::Collections::IVectorView<Particle>^ Speed {
			Windows::Foundation::Collections::IVectorView<Particle>^ get() { return _speedParticle.GetView(); }
		}*/

		Windows::Foundation::IAsyncAction^ DecompositionSpeedAsync();
		Windows::Foundation::IAsyncAction^ DecompositionSpeedElectronsAsync();
		Windows::Foundation::IAsyncAction^ DecompositionSpeedCarbonsAsync();
		Windows::Foundation::IAsyncAction^ DecompositionSpeedHeliumsAsync();

	private:
		Native::SpeedParticle* _speed;

		//Platform::Collections::Vector<Particle, equal_particle> _speedParticle;
	};
}