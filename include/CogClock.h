#pragma once



#include "IClock.h"
#include "IClockSource.h"

class Stepper;
class IOutput;
class CogClock : public IClock
{
public:
	CogClock(uint8_t minuteStep, uint8_t minutDir, uint8_t hourStep, uint8_t hourDir, uint8_t minuteSensorPort, uint8_t hourSensorPort);
	// Inherited via IClock
	virtual void Init(IClockSource* clock) override;
	void SetEnablePort(IOutput* enablePort);
	virtual void Pump() override;
	Stepper* GetMinsStepper() { return m_MinuteStepper; }
	Stepper* GetHoursStepper() { return m_HourStepper; }
protected:
	virtual long GetStepsToNextHour(uint8_t nextMinute);
private:


	enum CalibrationState
	{
		NotCalibrated,
		LoseInitialSensing,
		SensorHysteresis,
		SeekHome,
		MovingToHome,
		Stabilise1,
		Stabilise2,
		WaitStop1,
		WaitStop2,
		Calibrated,
	};

	struct RingControl
	{
		int stabTimer;
		uint8_t port;
		long homeDistance;
		long homePos;
		CalibrationState currentState;
		void (CogClock::* pumpFunction)();
		void(CogClock::* initFunction)();
		char name[32];
	};

	void InitMinutes();
	void InitHours();
	void PumpRing(Stepper* stepper, CogClock::RingControl* control);

	void TickHours();

	void TickMinutes();

	time_t	m_LastTime;
	Stepper* m_MinuteStepper;
	Stepper* m_HourStepper;
	IOutput* m_EnablePort;
	IClockSource* m_ClockSource;
	uint8_t m_LastMinute;
	uint8_t m_LastHour;
	int m_DisableDelay;

	RingControl	m_MinsControl;
	RingControl m_HourControl;
};
