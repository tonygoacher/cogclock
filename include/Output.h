#pragma once


#include "IOutput.h"

class ArduinoOutput : public IOutput
{
public:
	// Inherited via IOutput
	void Init(int portNumber, IOutput::PortState initialState);
	virtual void SetState(IOutput::PortState portState);
private:
	int m_Port;
};