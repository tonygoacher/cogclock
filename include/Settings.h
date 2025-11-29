#pragma once


// Settings for clock class
#include "WiFiConfig.h"


class Settings 
{
public:
	static Settings* Instance();
	// Inherited via IWifiSettings
	void SetWifi(const char* ssid, const char* password);
	void SetNTP(const char* ntp);
	void Write();
	const char* const GetSSID();
	const char* const GetPassword();
	const char* const GetNtp();
private:
	Settings();
	bool CheckSettings();
	bool ReadSettings();
	uint8_t GetChecksum();
	struct SystemSettings
	{
		uint8_t version;
		uint8_t checksum;
		char ssid[128];
		char password[128];
		char ntp[250];
	};
	SystemSettings m_Settings;
	static Settings* m_TheInstance;
	static const uint8_t sm_Version = 1;
};

