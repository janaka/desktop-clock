#include <SPI.h>
#include <WiFi.h>
#include "time.h"

#include "LedMatrix.h" // https://github.com/janaka/esp32-led-matrix

// to configure wifi follow instruction in `1.wifi_config.tmp`

#define NUMBER_OF_DEVICES 4 //number of led matrix connected in series
#define CS_PIN 15   // CS -> GPIO 15
#define CLK_PIN 14  // CLK -> GPIO 14
#define MISO_PIN 2  // we do not use this pin just fill to match constructor
#define MOSI_PIN 12 // DIN -> GPIO 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

unsigned int currentMin = 60;

void setup()
{
  Serial.begin(19200);
  delay(5000); // wait sometime for the serial port to open.
  Serial.println("Boot start...");

  syncInternetTime();

  ledMatrix.init();
  ledMatrix.setIntensity(1); // range is 0-15
  ledMatrix.setCharWidth(8);
  ledMatrix.setTextAlignment(TEXT_ALIGN_LEFT);
}

void loop()
{
  // Serial.print(".");
  struct tm timeinfo;
  char strTime[5];
  char strHr[3];
  char strMin[3];
  char strSec[3];
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(strTime, sizeof strTime, "%H%M", &timeinfo);
  strftime(strHr, sizeof strHr, "%H", &timeinfo);
  strftime(strMin, sizeof strMin, "%M", &timeinfo);
  // strftime(strSec, sizeof strSec, "%S", &timeinfo);
  Serial.println(strTime);

  unsigned int thisHr = (unsigned int)timeinfo.tm_hour;
  unsigned int thisMin = (unsigned int)timeinfo.tm_min;
  unsigned int thisSec = (unsigned int)timeinfo.tm_sec;
  
  Serial.println(thisSec);
  
  // Avoid drift. Sync time with the internet server once per day.
  if ((thisHr==23) && (thisMin==59)) { 
    syncInternetTime();
  }

  Serial.println(thisMin);
  Serial.println(currentMin);

  // Only refresh display when the minute changes.
  if (thisMin!=currentMin) {
    currentMin = (unsigned int)timeinfo.tm_min;
    ledMatrix.setText(strTime);
    ledMatrix.clear();
    ledMatrix.drawText();
    ledMatrix.commit();
    Serial.println("Time refreshed on display.");
  }

  printLocalTime();
  delay(5000);
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //Serial.println((unsigned int)&timeinfo.tm_sec);

  //tm_min

  //tm_hour
}

void syncInternetTime()
{
  const char *ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;     //adjust this to you timezone. 0 is GMT/UTC
  const int daylightOffset_sec = 0; //3600; // +1hr for BST

  wifiConnect();

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Set time from internet time server.");
  printLocalTime();

  wifiDisconnect();
  
}

void wifiConnect()
{
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int c = 0;
  while ((c < 5) && (WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    c++;
    Serial.print(".");
    Serial.print(c);
  }

  Serial.println("");
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());
  /*
WiFi.status() return codes:
WL_NO_SHIELD	255
WL_IDLE_STATUS	0
WL_NO_SSID_AVAIL	1
WL_SCAN_COMPLETED	2
WL_CONNECTED	3
WL_CONNECT_FAILED	4
WL_CONNECTION_LOST	5
WL_DISCONNECTED	6
*/
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiDisconnect()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Disconenct Wifi.");
}