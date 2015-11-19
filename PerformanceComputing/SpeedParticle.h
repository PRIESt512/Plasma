#pragma once
#include "Particle.h"

namespace PerformanceComputing
{
	public ref class SpeedParticle sealed
	{
	public:
		SpeedParticle();
		/*property Windows::Foundation::Collections::IVectorView<Particle>^ Speed {
			Windows::Foundation::Collections::IVectorView<Particle>^ get() { return _speedParticle.GetView(); }
		}*/

		//Windows::Foundation::IAsyncAction^ SpeedCompare();

	private:
		//Platform::Collections::Vector<Particle, equal_particle> _speedParticle;
	};
}