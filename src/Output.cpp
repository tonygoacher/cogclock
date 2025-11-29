#include <Arduino.h>
#include "Output.h"


void ArduinoOutput::Init(int port, IOutput::PortState initialState)
{
	m_Port = port;
	pinMode(port, OUTPUT);
	digitalWrite(m_Port, initialState == IOutput::PortOn ? HIGH : LOW);
}

void ArduinoOutput::SetState(IOutput::PortState portState)
{
	digitalWrite(m_Port, portState == IOutput::PortOn ? HIGH : LOW);
}