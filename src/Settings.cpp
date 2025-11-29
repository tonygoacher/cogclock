#include <Arduino.h>
#include <EEPROM.h>

#include <stddef.h>
#include "Settings.h"



Settings* Settings::m_TheInstance = nullptr;

Settings* Settings::Instance()
{
	if (m_TheInstance == nullptr)
	{
		m_TheInstance = new Settings();
	}
	return m_TheInstance;
}

Settings::Settings()
{
	EEPROM.begin(512);  //Initialize EEPROM
	if (!ReadSettings() || m_Settings.version != sm_Version)
	{
		memset(&m_Settings, 0, sizeof(m_Settings));
		m_Settings.version = sm_Version;
		Write();
	}
}

bool Settings::ReadSettings()
{
	uint8_t* pData = (uint8_t*)&m_Settings;
	for (uint16_t address = 0; address < sizeof(m_Settings); address++)
	{
		*pData++ = EEPROM.read(address);
	}
	return CheckSettings();
}


uint8_t Settings::GetChecksum()
{
	uint8_t chk = 0;
	uint8_t* pData = (uint8_t*)&m_Settings;
	pData += offsetof(SystemSettings, ssid);
	for (int i = 0; i < sizeof(Settings) - offsetof(SystemSettings, ssid); i++)
	{
		chk += *pData++;
	}
	return chk;
}

bool Settings::CheckSettings()
{
	uint8_t calcChk = GetChecksum();

	return GetChecksum() == m_Settings.checksum;
}

void Settings::Write()
{
	uint8_t chk = GetChecksum();
	m_Settings.checksum = chk;
	uint8_t* pData = (uint8_t*)&m_Settings;
	for (uint16_t address = 0; address < sizeof(m_Settings); address++)
	{
		EEPROM.write(address, *pData++);
	}
	EEPROM.commit();
}

const char* const Settings::GetSSID()
{
	return m_Settings.ssid;
}

const char* const Settings::GetPassword()
{
	return m_Settings.password;
}

const char* const Settings::GetNtp()
{
	return m_Settings.ntp;
}

void Settings::SetWifi(const char* const ssid, const char* const password)
{
	if (strlen(ssid) + 1 < sizeof(m_Settings.ssid))
	{
		strcpy(m_Settings.ssid, ssid);
	}

	if (strlen(password) + 1 < sizeof(m_Settings.password))
	{
		strcpy(m_Settings.password, password);
	}
	Write();
}

void Settings::SetNTP(const char* ntp)
{
	if (strlen(ntp) + 1 < sizeof(m_Settings.ntp))
	{
		strcpy(m_Settings.ntp, ntp);
		Write();
	}
}