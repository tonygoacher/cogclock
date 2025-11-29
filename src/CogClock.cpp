#include "CogClock.h"
#include "Stepper.h"
#include "IOutput.h"

//Minute ratio is 60:6 = 10:1
// 32 x mircro stepping for small cog = 6400 steps/rev
// Total steps for minute ring one revolution = 10 x 6400 = 64000 steps
// Steps per minute = 64000 / 60 = 1066.66666 steps/min


// Each minute is 533.3 pulses
static const long minutePositions[] =
{
	-28265, // 59 > 0
	2667,	// Pulses from zero to 1
	6933,	// Pulses to go from 1 to 2
	-3733,	// Pulse to go from 2 to 3
	-4267,	// Pulses to go from 3 to 4
	-10127,	// Pulses to go from 4 to 5
	-4802,	// Pulses to go from 5 to 6
	13858,	// Pulses to go from 6 to 7
	6933,	// Pulses to go from 7 to 8
	533,	// Pulses to go from 8 to 9
	3733,	// Pulses to go from 9  to 10
	-19721, // Pulses to go from 10 to 11
	-11726, // Pulses to go from 11 to 12
	7470, // 12 > 13
	-10666, // 13> 14
	5866, // 14 > 15
	-18142, // 15 > 16
	2133, // 16 > 17
	-14407, // 17 > 18
	5870, // 18 > 19
	14407, // 19 >20
	-18676, // 20 > 21
	-4269, // 21 > 22
	13874, // 22 > 23
	6403, // 23 > 24
	-9071, // 24 > 25
	-5870, // 25 > 26
	21344, // 26 > 27
	-4802, // 27 > 28
	-4269, // 28 > 29
	26146, // 29 > 30
	3735, // 30 > 31
	-18132, // 31 > 32
	1600, // 32 > 33
	9066, // 33> 34
	-15466, // 34 > 35
	-3202, // 35 > 36
	-8538, // 36 > 37
	-5870, // 37 > 38
	15474, // 38 > 39
	11739, // 39 > 40
	10138, // 40 > 41
	-3202, // 41 > 42
	-9605, // 42 > 43
	6937, // 43 > 44
	9605, // 44 > 45
	-2668, // 45 > 46
	-12806, // 46 > 47
	18667, // 47 > 48
	-15466, // 48 > 49
	12799, // 49 > 50
	-14932, // 50 > 51
	-4800, // 51 > 52
	-5333, // 52 > 53
	4800, // 53 >> 54
	14399, // 54 > 55
	14399, // 55 > 56
	-17599, // 56 > 57
	-4266, // 57 > 58
	11199, // 58 > 59
};


//Hour ratio is 24:8 = 3:1
// 32 x mircro stepping for hour cog = 6400 steps/rev
// Total steps for hour ring one revolution = 3 x 6400 = 19200 steps
// Steps per hour = 19200 / 12 = 1600 
static const long hourPositions[] =
{
	-1600, // 11>>12
	-8000,	// 12->1
	6400, // 1>2
	-8000, // 2>>3
	-4800, // 3>>4
	8000, // 4>>5
	1600, // 5>>6
	-8000, // 6>>7
	1600, // 7>>8
	-4800, // 8>>9
	-6400, //9 >>10
	4800, //10>>11

};

float minsAcc = 5000.0f;
float minsMaxSpeed = 10000.0f;

CogClock::CogClock(uint8_t minuteStep, uint8_t minutDir, uint8_t hourStep, uint8_t hourDir, uint8_t minuteSensorPort, uint8_t hourSensorPort)
{
	//m_MinuteStepper = new Stepper(minuteStep, minutDir, 4800.0f, 1200.0f);
	m_MinuteStepper = new Stepper(minuteStep, minutDir, minsMaxSpeed, minsAcc);
	m_HourStepper = new Stepper(hourStep, hourDir, 3000.0f, 4000.0f);

	m_MinsControl.port = minuteSensorPort;
	m_MinsControl.homeDistance = -800;
	m_MinsControl.initFunction = &CogClock::InitMinutes;
	m_MinsControl.pumpFunction = &CogClock::TickMinutes;
	m_MinsControl.currentState = CogClock::NotCalibrated;;

	m_HourControl.port = hourSensorPort;

	m_HourControl.homeDistance = -850;
	m_HourControl.initFunction = &CogClock::InitHours;
	m_HourControl.pumpFunction = &CogClock::TickHours;
	m_HourControl.currentState = CogClock::NotCalibrated;

	pinMode(m_HourControl.port, INPUT);
	pinMode(m_MinsControl.port, INPUT);


	strcpy(m_HourControl.name, "Hours");
	strcpy(m_MinsControl.name, "Mins");
}

void CogClock::SetEnablePort(IOutput* enablePort)
{
	m_EnablePort = enablePort;
	m_EnablePort->SetState(IOutput::PortOn);
}

void CogClock::InitHours()
{
	m_LastHour = m_ClockSource->GetHours();
	long firstPos;
	firstPos = 0;
	if (m_LastHour != 0)
	{
		for (int i = 1; i <= m_LastHour; i++)
		{
			firstPos += hourPositions[i];
		}
	}
	Serial.print("CogClock init hour is ");
	Serial.println(m_LastHour);
	Serial.print("CogClock target ");
	Serial.println(firstPos);
	m_EnablePort->SetState(IOutput::PortOff);
	m_HourStepper->Goto(firstPos);
}

void CogClock::Init(IClockSource* clock)
{
	m_ClockSource = clock;
}

void CogClock::InitMinutes()
{
	m_LastMinute = m_ClockSource->GetMins();	// DEBUG

	long firstPos = 0;
	if (m_LastMinute != 0)
	{
		for (int i = 1; i <= m_LastMinute; i++)
		{
			firstPos += (minutePositions[i] * 2);
		}
	}
	Serial.print("CogClock init minute is ");
	Serial.println(m_LastMinute);
	Serial.print("CogClock target ");
	Serial.println(firstPos);
	m_EnablePort->SetState(IOutput::PortOff);
	m_MinuteStepper->Goto(firstPos);
}

void CogClock::TickMinutes()
{
	m_MinuteStepper->Run();
	if (!m_MinuteStepper->Running())
	{
		uint8_t mins = m_ClockSource->GetMins();
		if (m_LastMinute != mins)
		{
			m_LastMinute = mins;
			if (mins == 0)
			{
				m_MinsControl.currentState = CogClock::NotCalibrated;
			}
			else
			{
				m_EnablePort->SetState(IOutput::PortOff);
				m_MinuteStepper->Goto(minutePositions[m_LastMinute]*2);

				Serial.println(mins);
				Serial.print("Mins CogClock target ");
				Serial.println(minutePositions[m_LastMinute]*2);
			}	
		}
	}
}

void CogClock::PumpRing(Stepper* stepper, CogClock::RingControl* control)
{
	
		switch (control->currentState)
		{
			case CogClock::NotCalibrated:
			{
			
				if (digitalRead(control->port) )
				{
					control->currentState = CogClock::SeekHome;
					m_EnablePort->SetState(IOutput::PortOff);
					stepper->Goto(-100000);
				}
				else
				{
					m_EnablePort->SetState(IOutput::PortOff);
					stepper->Goto(-6000);
					control->currentState = LoseInitialSensing;
				}
				
			}
			break;

			case CogClock::LoseInitialSensing:
			{
				stepper->Run();
				if (digitalRead(control->port) == HIGH)
				{
					control->currentState = CogClock::SeekHome;
					m_EnablePort->SetState(IOutput::PortOff);
					stepper->Goto(-100000);
				}
			}
			break;

			case CogClock::SeekHome:
			{
				stepper->Run();
				if (digitalRead(control->port) == LOW)
				{
					control->stabTimer = 20000;
					control->currentState = CogClock::WaitStop1;
					stepper->Stop();
					control->homePos = stepper->GetPos();
					Serial.printf("%s Position is %\dn", control->name, control->homePos);
				}
			}
			break;

			case CogClock::WaitStop1:
			{
				stepper->Run();
				if (!stepper->Running())
				{
					Serial.printf("%s WS Position is %d\n", control->name, stepper->GetPos());
					Serial.printf("%s WS home Position is %d\n", control->name, control->homePos + control->homeDistance);
					stepper->MoveTo(control->homePos + control->homeDistance);
					control->currentState = CogClock::MovingToHome;
				}
			}
			break;

			
			case CogClock::MovingToHome:
			{
				stepper->Run();
				if (!stepper->Running())
				{
					stepper->SetCurrentPos(0);
					stepper->SetAcceleration(5000.0f);
					stepper->SetMaxSpeed(10000.0f);
		
					Serial.printf("%s Calling init function\n", control->name);
					control->currentState = CogClock::Calibrated;
					(this->*control->initFunction)();
					Serial.println("Done\n");
				}
			}
			break;

			case Calibrated:
			{
				(this->*control->pumpFunction)();		
			}
			break;
		}
}

void CogClock::TickHours()
{

	m_HourStepper->Run();
	

	if (!m_HourStepper->Running())
	{
		uint8_t hours = m_ClockSource->GetHours();

		if (m_LastHour != hours)
		{
			if (hours == 0)
			{
				m_HourControl.currentState = CogClock::NotCalibrated;
			}
			else
			{
				long newPos = GetStepsToNextHour(hours);
				m_EnablePort->SetState(IOutput::PortOff);
		
				m_HourStepper->Goto(newPos);

				Serial.print("New hours is ");
				Serial.println(hours);
				Serial.print("Hours CogClock target ");
				Serial.println(newPos);
				m_LastHour = hours; 
			}
		} 
	}
	
}

void CogClock::Pump()
{
	PumpRing(m_MinuteStepper, &m_MinsControl);// , -770, m_MinuteStabTimer, m_MinutesCalibrationState, & CogClock::TickMinutes, & CogClock::InitMinutes);
	PumpRing(m_HourStepper, &m_HourControl); // , -800, m_HourStabTimer, m_HoursCalibrationState, & CogClock::TickHours, & CogClock::InitHours);

	if (m_MinuteStepper->Running() == false && m_HourStepper->Running() == false)
	{
		if (m_DisableDelay)
		{
			m_DisableDelay--;
		}
		else
		{
			m_EnablePort->SetState(IOutput::PortOn);
		}
	}
	else
	{
		m_DisableDelay = 10000;
	}
}

long CogClock::GetStepsToNextHour(uint8_t nextHour)
{
	long distance = 0;
	if (nextHour != m_LastHour)
	{
		char text[512];
		sprintf(text,"New Hour: %d Last Hour %d", nextHour, m_LastHour);
		Serial.print(text);
		Serial.println();
		if (nextHour > m_LastHour || nextHour == 0)
		{
			m_LastHour = nextHour;
			distance =  hourPositions[m_LastHour];
		}
		else
		{
			// BST has ended
			
			distance = -hourPositions[m_LastHour];
			m_LastHour = nextHour;
			
		}
	}
	return distance;
}
