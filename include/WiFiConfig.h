#pragma once

#include <Arduino.h>
class AsyncWebServer;

// This class returns the SSID and password of the configured connection
// Also implements a web page to allow it to be set
class WiFiConfig
{
public:
	WiFiConfig();
	void DoWifiConfig();
private:
	AsyncWebServer* m_Server;
};
