#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>
#include <NTPClient.h>          // include NTPClient library
#include <TimeLib.h>            // include Arduino time library

// set Wi-Fi SSID and password
const char *ssid     = "Sayantan";
const char *password = "sayantan";

WiFiUDP ntpUDP;
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time.nist.gov", 19800, 60000); //GMT+5:30 : 5*3600+30*60=19800

#define PIN        5
#define NUMPIXELS  12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int Second, Minute, Hour, hour_;

int period = 1000;
unsigned long time_now = 0;

void setup(void)
{
  Serial.begin(115200);
  delay(10);
  pixels.begin();
  pixels.clear();
  for (int i = 0; i < 12; i++) {
    pixels.setPixelColor(i, pixels.Color(  0,   0,   0)); //Black
  }
  pixels.show();
  WiFi.begin(ssid, password);

  Serial.print("Connecting.");

  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("connected");
  timeClient.begin();
  delay(10);
}


void loop()
{
  if (WiFi.status() == WL_CONNECTED)  // check WiFi connection status
  {
    timeClient.update();
    unsigned long unix_epoch = timeClient.getEpochTime();   // get UNIX Epoch time
    Second = second(unix_epoch) / 5;      // get seconds from the UNIX Epoch time
    Minute = minute(unix_epoch) / 5;    // get minutes (0 - 59)
    hour_   = hour(unix_epoch);        // get hours   (0 - 23)

    if (hour_ > 12) {
      Hour = hour_ - 12;
    }
    else
      Hour = hour_;

    while (millis() > time_now + period) {
      time_now = millis();
      pixels.clear();
      pixels.setPixelColor(Second, pixels.Color(  0,   0, 32)); // Blue
      pixels.setPixelColor(Second - 1, pixels.Color(  0,   0, 16)); // Bluee
      pixels.setPixelColor(Second - 2, pixels.Color(  0,   0, 8)); // Blueee
      pixels.setPixelColor(Minute, pixels.Color(  0, 128,   0)); // Green
      pixels.setPixelColor(Hour, pixels.Color(128,   0,   0)); // Red
      pixels.show();
    }
  }
}
