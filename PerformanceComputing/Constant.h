#pragma once
// ������������������ ����������(epsilon0)
constexpr double ELECTROSTATIC_CONSTANT = 8.8541878E-12;

// ���������� ��������� (kb)
constexpr double BOLTZMANN_CONSTANT = 1.380648813E-23;

// ���������� ��������� (kb)
constexpr double AVOGADRO_CONSTANT = 6.0221412927E+23;

// ����� ������ � ������
constexpr double ELECTRONS_NUMBER = 1E+12;
constexpr double CARBONS_NUMBER = 1E+12;
constexpr double HELIUMS_NUMBER = 1E+6;

// ����� ������
constexpr double ELECTRONS_MASS = 9.11E-31;
constexpr double CARBONS_MASS = 12.011 * 1.67E-27 - 9.11E-31;
constexpr double HELIUMS_MASS = 4.002 * 1.67E-27 - 9.11E-31;

// ������ ������
constexpr double ELECTRONS_CHARGE = -1.602176565E-19;
constexpr double CARBONS_CHARGE = 1.602176565E-19;
constexpr double HELIUMS_CHARGE = 1.602176565E-19;

// ������� ������
constexpr double ELECTRONS_RADIUS = 2.81794E-15;
constexpr double CARBONS_RADIUS = 91E-12;
constexpr double HELIUMS_RADIUS = 31E-12;

// �������� ��������� �����
constexpr double ANODE_BURNING_SPEED = 0.44E-5;

// ����������� �������� � ������
constexpr int TEMPERATURE = 10000;
constexpr int ELECTRONS_TEMPERATURE = 4200;
constexpr int CARBONS_TEMPERATURE = 4200;
constexpr int HELIUMS_TEMPERATURE = 293;

// ������� �����
constexpr float X_DIMENSION_GRID = 1.0E-3;
constexpr float Y_DIMENSION_GRID = 1.0E-2;
constexpr float Z_DIMENSION_GRID = 1.0E-2;

// ����� ����� �� ����� ��� ������� ������
constexpr int X_STEP_NUMBER_GRID = 10;
constexpr int Y_STEP_NUMBER_GRID = 10;
constexpr int Z_STEP_NUMBER_GRID = 10;

// ��� �� �����
constexpr double X_STEP = X_DIMENSION_GRID / X_STEP_NUMBER_GRID;
constexpr double Y_STEP = Y_DIMENSION_GRID / Y_STEP_NUMBER_GRID;
constexpr double Z_STEP = Z_DIMENSION_GRID / Z_STEP_NUMBER_GRID;

// ��������� �������� ����������
constexpr int POTENTIAL_BOUND_VALUE = 50;

// ����� ������������ (epsilon)
constexpr float ESTABLISHING_METHOD_ACCURACY = 0.01;