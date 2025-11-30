/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <TimeLib.h>
#include <MultiStepper.h>
#include <AccelStepper.h>
#include <TimeElapsed.h>
#include <ESPDateTime.h>
#include <DateTime.h>
#include <EEPROM.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "WiFiConfig.h"
#include "Settings.h"
#include "WebNTP.h"
#include "Stepper.h"
#include "CogClock.h"
#include "NTPClockSource.h"

#include "TestClock.h"
#include "Output.h"


#define D0 16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15


WebNtp webNtp;
//Stepper stepper(5, 4);
//CogClock( minuteStep,  minutDir,  hourStep,  hourDir,  minuteSensorPort,  hourSensorPort);

CogClock cogClock(D8, D2, D6, D5, D7, D1);
//TestClock clockSource;
NTPClockSource clockSource;
ArduinoOutput enableDrive;
ArduinoOutput ledDrive;
///
void setup() {
    enableDrive.Init(D4, IOutput::PortOn);
    ledDrive.Init(D3, IOutput::PortOn);
    cogClock.SetEnablePort(&enableDrive);
    Serial.begin(115200);
    delay(2000);
    Serial.println("Startup...");
    
    WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s",Settings::Instance()->GetSSID());
    WiFi.begin(Settings::Instance()->GetSSID(), Settings::Instance()->GetPassword());

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("WiFi Failed!");
        WiFi.mode(WIFI_OFF);
        delay(1000);
        WiFi.mode(WIFI_AP);
        Serial.println(WiFi.softAP("CogClock", "password") ? "Ready" : "Failed!");
        WiFiConfig wifiConfig;
        wifiConfig.DoWifiConfig();
        while (1)
        {
            delay(500);
        }
    }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    webNtp.Begin();

    clockSource.Init();   
}

bool initDone = false;

void loop() {
    if (!initDone)
    {
        if (clockSource.Running())
        {
            initDone = true;
            Serial.println("Init clock");
            cogClock.Init(&clockSource);
        }
    }
    else
    {
        cogClock.Pump();
    }     
 }