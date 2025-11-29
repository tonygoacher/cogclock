#pragma once

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>




class WebNtp
{
public:
	WebNtp();
	void Begin();

private:
	AsyncWebServer* m_Server;
};