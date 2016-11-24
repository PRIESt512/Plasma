#pragma once
#include "Particle.h"
#include "MaxwellParticleDistribution.h"
#include <random>
#include <memory>

namespace Native
{
	//�������� ����� ��� ������� ������������� ���������
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
	//����������� �������� �++/�� ��� ��������� ������ C++, ��������� ����������������� C# � �������� ����� � ����������� ����� 
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
		/// ��������� ������, ��� ������� ����� �������� ��� �������� �� ��������� ������
		/// <param name="smallest">����������� ��������, � �������� ���������� ������������ �������������</param>
		/// <param name="largest">������������ ��������, �� �������� ���������� ������������ �������������</param>
		/// </summary>
		SpeedParticle(int smallest, int largest);

		/// <summary>
		/// ��������� ������, ��� ������� ����� �������� ��� �������� �� ��������� ������
		/// <param name="smallest">����������� ��������, � �������� ���������� ������������ �������������</param>
		/// <param name="largest">������������ ��������, �� �������� ���������� ������������ �������������</param>
		/// <param name="processor_count">����� ���� ���������� ��� ������������� ��������� ������</param>
		/// </summary>
		SpeedParticle(int smallest, int largest, int processor_count);

		/// <summary>
		/// ������ �������� �������� ������ ���������, �������� � �����
		/// <param name="DIMENSIONLESS">�������� �������� � ������������ ���������</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ ����� �������� ������ ���������, �������� � �����
		/// <param name="DIMENSIONLESS">�������� �������� � ������������ ���������</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ FullSpeedAsync(bool DIMENSIONLESS);

		Windows::Foundation::IAsyncAction^ DecompositionSpeedElectronsAsyncOne();


		/// <summary>
		/// ������ �������� ������ ��������� � ����������� �� ���������
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedElectronsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ �������� ������ �������� � ����������� �� ���������
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedCarbonsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ �������� ������ ����� � ����������� �� ���������
		/// </summary>
		Windows::Foundation::IAsyncAction^ DecompositionSpeedHeliumsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ ����� �������� ������ ���������, ��� ���������� �� ���������
		/// <param name="DIMENSIONLESS">�������� �������� � ������������ ���������</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedElectronsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ ����� �������� ������ ��������, ��� ���������� �� ���������
		/// <param name="DIMENSIONLESS">�������� �������� � ������������ ���������</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedCarbonsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// ������ ����� �������� ������ �����, ��� ���������� �� ���������
		/// <param name="DIMENSIONLESS">�������� �������� � ������������ ���������</param>
		/// </summary>
		Windows::Foundation::IAsyncAction^ SpeedHeliumsAsync(bool DIMENSIONLESS);

		/// <summary>
		/// �������� �� ��������� ������ ���������
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ ElectronsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedElectrons; }
		}

		/// <summary>
		/// �������� �� ��������� ������ ��������
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ CarbonsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedCarbons; }
		}

		/// <summary>
		/// �������� �� ��������� ������ �����
		/// </summary>
		property Windows::Foundation::Collections::IVector<Particle>^ HeliumsDecompositionSpeed {
			Windows::Foundation::Collections::IVector<Particle>^ get() { return _speedHeliums; }
		}

		/// <summary>
		/// ����� �������� ������ ���������
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ ElectronsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedElectrons; }
		}

		/// <summary>
		/// ����� �������� ������ ��������
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ CarbonsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedCarbons; }
		}

		/// <summary>
		/// ����� �������� ������ �����
		/// </summary>
		property Windows::Foundation::Collections::IVector<double>^ HeliumsFullSpeed {
			Windows::Foundation::Collections::IVector<double>^ get() { return _full_speedHeliums; }
		}
	};
}