#include <ESPDateTime.h>
#include <Time.h>
#include <Arduino.h>
#include <Timezone.h>



#include "NTPClockSource.h"

NTPClockSource::NTPClockSource()
{
	m_InitDone = false;
	m_LastHour = m_InvalidHour;

}

static long int getTimeFromNtp()
{
	return DateTime.getTime();
}

uint8_t NTPClockSource::GetHours()
{

	time_t actualTime, utc;
	

	utc = now();	//current time from the Time Library
	uint8_t localHour = hour();

	if (localHour != m_LastHour)
	{
		TimeChangeRule* tcr;
		TimeChangeRule gmtStartBST = { "GMT", Last, Sun, Mar, 1, 60 };  //GMT + 1 hour
		TimeChangeRule gmtEndtBST = { "GMT", Last, Sun, Oct, 2, 0 };   //GMT
		Timezone timeInTheUK(gmtStartBST, gmtEndtBST);	// This is expensive so only do if hour changes

		m_LastHour = localHour;
		bool isdt = timeInTheUK.locIsDST(utc);
		if (isdt)
		{
			localHour++;
		}

		if (localHour >= 12)
		{
			localHour -= 12;
		}
		m_LastLocalHour = localHour;
	}

	return m_LastLocalHour;
}


uint8_t NTPClockSource::GetMins()
{
	return minute();
}

uint8_t NTPClockSource::GetSecs()
{
	return second();
}

uint8_t NTPClockSource::GetDate()
{
	return day();
}

uint8_t NTPClockSource::GetMonth()
{
	return month();
}

uint8_t NTPClockSource::GetYear()
{
	return year();
}

uint8_t NTPClockSource::GetYearDay()
{
	return 0;
}

uint8_t NTPClockSource::GetDayOfWeek()
{
	return weekday();
}

void NTPClockSource::Init()
{

}

void NTPClockSource::Pump()
{
	time_t timeRightNow = now();
	if (timeRightNow != m_Now)
	{
		m_Now = timeRightNow;
	}
}


bool NTPClockSource::Running()
{
	bool running = false;
	if (DateTime.isTimeValid() && !m_InitDone)
	{
		Serial.println("Getting timefrom NTP");
		if (timeStatus() != timeSet)
		{
			setSyncProvider((long long int (*)())getTimeFromNtp);
			setSyncInterval(300);
		}
		m_InitDone = true;

		Serial.println("Time is: ");
		Serial.println(DateTime.toString().c_str());
	}

	return m_InitDone;
}