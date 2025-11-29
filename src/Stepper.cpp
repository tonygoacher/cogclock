#include "Stepper.h"

Stepper::Stepper(uint8_t stepPin, uint8_t dirPin)
{

	m_Stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
	m_Stepper->setAcceleration(900.0f);
	m_Stepper->setMaxSpeed(1.0f);
}

Stepper::Stepper(uint8_t stepPin, uint8_t dirPin, float speed, float acceleration)
{
	m_Stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
	m_Stepper->setAcceleration(acceleration);
	m_Stepper->setMaxSpeed(speed);
	//m_Stepper->setSpeed(speed);
}

void Stepper::Goto(long position)
{
	m_Stepper->move(position);
}

void Stepper::MoveTo(long position)
{
	m_Stepper->moveTo(position);
}

void Stepper::Run()
{
	m_Stepper->run();
}

bool Stepper::Running()
{
	return m_Stepper->isRunning();
}


void Stepper::Step()
{
	m_Stepper->runSpeed();
}


void Stepper::Stop()
{
	m_Stepper->stop();
}


long Stepper::GetPos()
{
	return m_Stepper->currentPosition();
}

void Stepper::SetCurrentPos(long newPos)
{
	m_Stepper->setCurrentPosition(newPos);
}

float Stepper::GetCurrentSpeed()
{
	return m_Stepper->speed();
}



void Stepper::SetAcceleration(float acceleration)
{
	m_Stepper->setAcceleration(acceleration);
}

void Stepper::SetMaxSpeed(float maxSpeed)
{
	m_Stepper->setMaxSpeed(maxSpeed);
}