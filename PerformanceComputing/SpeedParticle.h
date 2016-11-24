#pragma once
#include "Particle.h"
#include "MaxwellParticleDistribution.h"
#include <random>
#include <memory>

namespace Native
{
	//Нативный класс для расчета распределения скоростей
	class SpeedParticle : protected MaxwellParticleDistribution
	{
	public:
		SpeedParticle(int smallest, int largest);
		SpeedParticle(int smallest, int largest, int processor_count);

		void DecompositionElectrons(bool DIMENSIONLESS);
		void DecompositionCarbons(bool DIMENSIONLESS);
		void DecompositionHeliums(bool DIMENSIONLESS);

		void FullSpeedElectrons(bool DIMENSIONLESS);
		void FullSpeedCarbons(bool DIMENSIONLESS);
		void FullSpeedHeliums(bool DIMENSIONLESS);

		void DecompositionElectronsOne();

		std::vector<PerformanceComputing::Particle> _speed_electrons;
		std::vector<PerformanceComputing::Particle> _speed_carbons;
		std::vector<PerformanceComputing::Particle> _speed_heliums;

		std::vector<double> _full_speed_electrons;
		std::vector<double> _full_speed_carbons;
		std::vector<double> _full_speed_heliums;
	};

}

namespace PerformanceComputing
{
	//Управляемая оболочка С++/СХ для нативного класса C++, позволяет взаимодействовать C# с нативным кодом в управляемой среде 
	public ref class SpeedParticle sealed
	{
	private:
		Native::SpeedParticle* _speed;

		Windows::Foundation::Collections::IVector<Particle>^ _speedElectrons;
		Windows::Foundation::Collections::IVector<Particle>^ _speedCarbons;
		Windows::Foundation::Collections::IVector<Particle>^ _speedHeliums;

		Windows::Foundation::Collections::IVector<double>^ _full_speedElectrons;
		Windows::Foundation::Collections::IVector<double>^ _full_speedCarbons;
		Windows::Foundation::Collections::IVector<double>^ _full_speedHeliums;

	public:

		/// <summary>
		/// Экземпляр класса, для расчета общей скорости или скорости по проекциям частиц
		/// <param name="smallest">Минимальное значение, с которого необходимо генерировать распределение</param>
		/// <param name="largest">Максимальное значение, до которого необходимо генерировать распределение</param>
		/// </summary>
		SpeedParticle(int smallest, int largest);

		/// <summary>
		/// Экземпляр класса, для расчета общей скорости или скорости по проекциям частиц
		/// <param name="smallest">Минимальное значение, с которого необходимо генерировать распределение</param>
		/// <param name="largest">Максимальное значение, до которого необходимо генерировать распределение</param>
		/// <param name="processor_count">Число ядер процессора для многопоточной генерации данных</param>
		/// </summary>
		SpeedParticle(int smallest, int largest, int processor_count);

		/// <summary>
		/// Расчет проекций скорости частиц электрона, углерода и гелия
		/// <param name="DIMENSIONLESS">Параметр перехода к безразмерным величинам</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет общей скорости частиц электрона, углерода и гелия
		/// <param name="DIMENSIONLESS">Параметр перехода к безразмерным величинам</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ FullSpeedAsync(bool DIMENSIONLESS);

		Windows::Foundation::IAsyncAction^ DecompositionSpeedElectronsAsyncOne();


		/// <summary>
		/// Расчет скорости частиц электрона с разложением по проекциям
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedElectronsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет скорости частиц углерода с разложением по проекциям
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedCarbonsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет скорости частиц гелия с разложением по проекциям
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedHeliumsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет общей скорости частиц электрона, без разложения по проекциям
		/// <param name="DIMENSIONLESS">Параметр перехода к безразмерным величинам</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedElectronsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет общей скорости частиц углерода, без разложения по проекциям
		/// <param name="DIMENSIONLESS">Параметр перехода к безразмерным величинам</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedCarbonsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Расчет общей скорости частиц гелия, без разложения по проекциям
		/// <param name="DIMENSIONLESS">Параметр перехода к безразмерным величинам</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedHeliumsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// Скорость по проекциям частиц электрона
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ ElectronsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedElectrons; }
		}

		/// <summary>
		/// Скорость по проекциям частиц углерода
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ CarbonsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedCarbons; }
		}

		/// <summary>
		/// Скорость по проекциям частиц гелия
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ HeliumsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedHeliums; }
		}

		/// <summary>
		/// Общая скорость частиц электрона
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ ElectronsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedElectrons; }
		}

		/// <summary>
		/// Общая скорость частиц углерода
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ CarbonsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedCarbons; }
		}

		/// <summary>
		/// Общая скорость частиц гелия
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ HeliumsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedHeliums; }
		}
	};
}