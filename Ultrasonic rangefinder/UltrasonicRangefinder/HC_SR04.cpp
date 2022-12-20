#include "HC_SR04.h"

// Sensor initializing
HC_SR04::HC_SR04(byte trig, byte echo, unsigned int measurements)
{
	distance = 0;
	this->measurements = measurements;
	this->trig = trig;
	this->echo = echo;
	mode = 1;
	maxMode = 2;
	panelSize = 3;
	delayMic = micros();
	delayMs = millis();

	ConnectSensor();
}

float HC_SR04::GetDistance()
{
	float tempDistance = distance;

	switch (mode)
	{
	case 1:
		break;
	case 2:
		tempDistance += panelSize;
		break;
	default:
		break;
	}

	return tempDistance;
}

unsigned int HC_SR04::GetMeasurements()
{
	return measurements;
}

byte HC_SR04::GetMode()
{
	return mode;
}

void HC_SR04::SetMeasurements(unsigned int measurements)
{
	this->measurements = measurements;
}

void HC_SR04::ChangeMode(byte switcher)
{
	if (mode + switcher > maxMode)
	{
		mode = 1;
		ChangeMode(switcher - 1);
	}
	else
	{
		mode += switcher;
	}
}

void HC_SR04::ConnectSensor()
{
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
}

void HC_SR04::SignalGeneration()
{
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
}

float HC_SR04::CalcRawDistance()
{
	unsigned long duration = 0;

	ConnectSensor();

	SignalGeneration();

	// Waiting for a signal
	duration = pulseIn(echo, HIGH, 23200);

	// Distance calculation
	distance = duration / 58;

	return distance;
}

void HC_SR04::CalcDistance()
{
	float distanceArr[measurements];

	// Calculation of n measurements
	for (int i = 0; i < measurements; i++)
	{
		distanceArr[i] = CalcRawDistance();
		delay(50);
	}
	delay(1);
	distance -= MsdRandom(distanceArr);
}

// The quadratic mean
float HC_SR04::Msd(float* array)
{
	float summ = 0;
	float arith = 0;

	arith = ArithMean(array);

	for (int i = 0; i < measurements; i++)
    {
		  summ += square(array[i] - arith);
    }
	summ = sqrt(summ / (measurements - 1));

	return summ;
}

// Arithmetic mean
float HC_SR04::ArithMean(float* array)
{
	float summ = 0;

	for (int i = 0; i < measurements; i++)
    {
		summ += array[i];
    }

	summ /= measurements;

	return summ;
}

// Averaging
float HC_SR04::MsdRandom(float* array)
{
	return (Msd(array) / sqrt(measurements));
}
