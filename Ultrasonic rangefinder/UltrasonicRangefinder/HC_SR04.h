#pragma once
#include <Arduino.h>

class HC_SR04
{
private:
	float distance;
	unsigned int measurements; // Количество измерений для точного расчета расстояния
	byte trig;
	byte echo;
	byte mode;
	byte maxMode;
	unsigned int panelSize;
	unsigned long delayMic;
	unsigned long delayMs;

public:
	HC_SR04(byte trig, byte echo, unsigned int measurements = 10);

	// Getters
	float GetDistance();
	unsigned int GetMeasurements();
	byte GetMode();

	// Setters
	void SetMeasurements(unsigned int measurements);

	// Working with the sensor
	void ConnectSensor();
	void SignalGeneration();
	void CalcDistance();

	// Extra functions
	void ChangeMode(byte switcher); // Режим расчета

private:
	// Internal functions
	float CalcRawDistance();
	// Mathematics
	float Msd(float* array);
	float ArithMean(float* array);
	float MsdRandom(float* array);
};
