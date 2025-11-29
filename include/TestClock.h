#pragma once

#include "IClockSource.h"
class Stepper;
class TestClock : public IClockSource
{
public:

	// Inherited via IClockSource
	virtual void Init() override;
	void SetMinsStepper(Stepper* stepper);
	void SetHoursStepper(Stepper* stepper);
	virtual void Pump() override;
	virtual bool Running() override;
	virtual uint8_t GetHours() override;
	virtual uint8_t GetMins() override;
	virtual uint8_t GetSecs() override;
	virtual uint8_t GetDate() override;
	virtual uint8_t GetMonth() override;
	virtual uint8_t GetYear() override;
	virtual uint8_t GetYearDay() override;
	virtual uint8_t GetDayOfWeek() override;
	virtual void SetHour(int hour) override;
	virtual void SetMinute(int minute) override;
private:
	uint8_t m_Minutes;
	uint8_t m_Hours;
	Stepper* m_MinsStepper;
	Stepper* m_HoursStepper;
	unsigned long m_Millis;

};