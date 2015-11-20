#pragma once
#include "Particle.h"
#include "MaxwellParticleDistribution.h"
#include <random>
#include <memory>

namespace Native
{
	class SpeedParticle
	{
	public:
		SpeedParticle(int largest, int smallest);
		SpeedParticle(int largest, int smallest, int processor_count);

		void DecompositionSpeed();
		void DecompositionElectrons();
		void DecompositionCarbons();
		void DecompositionHeliums();

	private:
		std::unique_ptr<MaxwellParticleDistribution> _maxwell;
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

	private:
		std::unique_ptr<Native::SpeedParticle> _speed;

		//Platform::Collections::Vector<Particle, equal_particle> _speedParticle;
	};
}