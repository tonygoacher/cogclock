#include <ESPDateTime.h>

#include "WebNTP.h"
#include "Settings.h"



extern void(*resetFunc) (void); //declare reset function @ address 0

static void notFound(AsyncWebServerRequest* request) {
	request->send(404, "text/plain", "Not found");
}

char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>NTP Set Up</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <br>
  <b>Server: %s</b>
  <br>
  <b>State: %s</b>
  <br>
  <b>Time: %s</b>
  <br>
  <form action="/get">
    NTP: <input type="text" name="NTP">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";

WebNtp::WebNtp()
{

}

void Send(AsyncWebServerRequest* request)
{
    char htmlOutput[1024];
    if (DateTime.isTimeValid())
    {
        sprintf(htmlOutput, index_html, Settings::Instance()->GetNtp(), "Connected", DateTime.toString().c_str());
    }
    else
    {
        sprintf(htmlOutput, index_html, Settings::Instance()->GetNtp(), "Not Connected", "");
    }
  
    request->send_P(200, "text/html", htmlOutput);
}

void WebNtp::Begin()
{
  


    DateTime.setTimeZone(0);
 
    Serial.print("Setting NTP URL to");
    Serial.println(Settings::Instance()->GetNtp());
    DateTime.setServer(Settings::Instance()->GetNtp());

    DateTime.begin(15 * 1000);
}
