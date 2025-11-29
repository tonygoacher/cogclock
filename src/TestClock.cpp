#include "TestClock.h"
#include "Stepper.h"

void TestClock::Init()
{
	m_Minutes = -1;
	m_Hours = -1;
	m_Millis = millis();
	Serial.println("TestClock Init");
	Serial.println(m_Minutes);
}

void TestClock::SetHour(int hour)
{
	m_Hours = hour - 1;
}
void TestClock::SetMinute(int minute)
{
	Serial.print("Setting test clock to");
	Serial.println(minute);

	m_Minutes = minute - 1;
}

void TestClock::SetMinsStepper(Stepper* stepper)
{
	m_MinsStepper = stepper;
}

void TestClock::SetHoursStepper(Stepper* stepper)
{
	m_HoursStepper = stepper;
}


void TestClock::Pump()
{
}

bool TestClock::Running()
{
	return true;
}

uint8_t TestClock::GetHours()
{
	if (!m_HoursStepper->Running())
	{
		m_Hours++;

		if (m_Hours >= 12)
		{
			m_Hours = 0;
		}
		Serial.print("New hours ");
		Serial.println(m_Hours);
	}
	return m_Hours;
}

uint8_t TestClock::GetMins()
{
	if (!m_MinsStepper->Running())
	{
		m_Minutes++;

		if (m_Minutes >= 60)
		{
			m_Minutes = 0;
		}
		Serial.print("New minutes ");
		Serial.println(m_Minutes);
	}
	return m_Minutes;
}

uint8_t TestClock::GetSecs()
{
	return uint8_t();
}

uint8_t TestClock::GetDate()
{
	return uint8_t();
}

uint8_t TestClock::GetMonth()
{
	return uint8_t();
}

uint8_t TestClock::GetYear()
{
	return uint8_t();
}

uint8_t TestClock::GetYearDay()
{
	return uint8_t();
}

uint8_t TestClock::GetDayOfWeek()
{
	return uint8_t();
}
