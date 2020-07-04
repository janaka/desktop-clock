#include <SPI.h>
#include <WiFi.h>
#include "time.h"
//#include <WiFiUdp.h>

#include "LedMatrix.h" // https://github.com/janaka/esp32-led-matrix
#include <NTPClient.h> // https://github.com/taranais/NTPClient

// to configure wifi follow instruction in `1.wifi_config.tmp`

#define NUMBER_OF_DEVICES 4 //number of led matrix connect in series
#define CS_PIN 15           // GPIO 15
#define CLK_PIN 14
#define MISO_PIN 2 //we do not use this pin just fill to match constructor
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;        //adjust this to you timezone
const int daylightOffset_sec = 3600; // +1hr
int currentMin = 60;

//WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);

void setup()
{
  Serial.begin(19200);
  delay(5000); // wait sometime for the serial port to open.
  Serial.println("Boot start...");

  wifiConnect();

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Set time from internet time server.");
  printLocalTime();

  wifiDisconnect();
  Serial.println("Disconenct Wifi.");

  ledMatrix.init();
  ledMatrix.setIntensity(1); // range is 0-15
  ledMatrix.setCharWidth(8);
  ledMatrix.setTextAlignment(TEXT_ALIGN_LEFT);

  //ledMatrix.setText("0000");
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
  strftime(strSec, sizeof strSec, "%S", &timeinfo);
  Serial.println(strTime);

  int thisMin = (int)strMin;
  if (currentMin!=thisMin)
  {
    currentMin=thisMin;
    ledMatrix.setText(strTime);
    ledMatrix.clear();
    ledMatrix.drawText();
    ledMatrix.commit();
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
  Serial.println((unsigned int)&timeinfo.tm_sec);

  //tm_min

  //tm_hour
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
}