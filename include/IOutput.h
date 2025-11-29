#pragma once



class IOutput
{
public:	
	enum PortState
	{
		PortOn,
		PortOff,
	};
	virtual void SetState(IOutput::PortState portState) = 0;
};