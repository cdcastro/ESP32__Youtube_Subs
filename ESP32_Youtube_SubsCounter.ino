// A Simple program to show Youtube subscribers

// Libraries included
#include <MD_Parola.h>
#include <Esp32WifiManager.h>
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include "theSwedishMaker.h"

const uint16_t WAIT_TIME = 10000;  //Time between fecthing data.

// Define hardware type and pins used
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// Hardware SPI connection
#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 5

// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Your Youtube ID
#define API_KEY "<Your API Key>"
#define CHANNEL_ID "<Your Channel ID>"

WiFiClientSecure client;

YoutubeApi api(API_KEY, client);

// Your Wifi Settings
const char* ssid = "<Your Wifi SSID>";
const char* password = "<Your Wifi Password>";

void setup() {
  P.begin();
  P.setFont(fontSubs);

  // Set the intensity (brightness) of the display (0-15)
  P.setIntensity(5);

  // Connecting to wifi
  WiFi.begin(ssid, password);
  P.print(" WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setInsecure();
}

void loop(void) {
  P.print("fetching");
  
  int cnt = 0;
  long yt1, yt2;
  while (1) {
    cnt = 3;
    if (api.getChannelStatistics(CHANNEL_ID)) {
      yt1 = api.channelStats.subscriberCount;
      yt2 = api.channelStats.viewCount;

      // Format results
      String subsCount = num_format(yt1);
      String viewCount = num_format(yt2);

      while (cnt > 0) {
        P.print("*" + subsCount);
        delay(1500);
        P.print("*" + viewCount);
        delay(1500);
        cnt--;
      }
    }
  }
}

String num_format(long num) {
  String num_s;
  long num_original = num;
  if (num > 99999 && num <= 999999) {
    num = num / 1000;
    long fraction = num_original % 1000;
    String num_fraction = String(fraction);
    String decimal = num_fraction.substring(0, 1);
    num_s = String(num) + "." + decimal + "K";
  } else if (num > 999999) {
    num = num / 1000000;
    long fraction = num_original % 1000000;
    String num_fraction = String(fraction);
    String decimal = num_fraction.substring(0, 1);
    if (num_original < 100000000) {
      num_s = " " + String(num) + "." + decimal + "M";
    } else {
      num_s = String(num) + "." + decimal + "M";
    }
  } else {
    int num_l = String(num).length();
    char num_f[15];
    int blankDigits = 6 - num_l;
    for (int i = 0; i < blankDigits; i++) {
      num_f[i] = ' ';
    }
    num_f[blankDigits] = '\0';
    num_s = num_f + String(num);
  }
  return num_s;
}