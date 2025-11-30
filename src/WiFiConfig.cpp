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

void(*resetFunc) (void) = 0; //declare reset function @ address 0

static void notFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not found");
}


// HTML web page to handle 3 input fields (input1, input2, input3)
static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>WIFI set up</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    SSID: <input type="text" name="ssid">
    Password: <input type="text" name="password">
    NTP: <input type="text" name="ntp">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";

WiFiConfig::WiFiConfig()
{
    m_Server = nullptr;
}

void WiFiConfig::DoWifiConfig()
{
    m_Server = new AsyncWebServer(80);
    // Send web page with input fields to client
    m_Server->on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", index_html);
        });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    m_Server->on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
        String ssid;
        String password;
        String ntp;
        // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
        if (request->hasParam("ssid")) {
            ssid = request->getParam("ssid")->value();

        }
        // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
        if (request->hasParam("password")) {
            password = request->getParam("password")->value();

        }

         // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
        if (request->hasParam("ntp")) {
            ntp = request->getParam("ntp")->value();
            if(ntp.length() != 0)
            {
                Serial.print("NTP URL set to ");
                Serial.println(ntp.c_str());
                Settings::Instance()->SetNTP(ntp.c_str());
            }

        }

        Settings::Instance()->SetWifi(ssid.c_str(), password.c_str());
        resetFunc();
        });
    m_Server->onNotFound(notFound);
    m_Server->begin();
}
