#pragma once

#include <AccelStepper.h>


class Stepper
{
public:
	Stepper(uint8_t stepPin, uint8_t dirPin);
	Stepper(uint8_t stepPin, uint8_t dirPin, float speed, float acceleration);
	void Goto(long position);
	void MoveTo(long position);
	void Run();
	void Step();
	void Stop();
	long GetPos();
	void SetCurrentPos(long newPos);
	float GetCurrentSpeed();
	void SetCurrentSpeed(float newSpeed);
	void SetAcceleration(float acceleration);
	void SetMaxSpeed(float maxSpeed);
	bool Running();
private:
	AccelStepper* m_Stepper;

};

