/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL9v6u-6Ci"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "oiuLRiMxY3azKHeGuIVR_VQQoxTictWr"

#include <SoftwareSerial.h>

// Comment this out to disable prints and save space


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "oiuLRiMxY3azKHeGuIVR_VQQoxTictWr";


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "vats";
char pass[] = "31238069";
int board;
int pin;
int pin_status;
String message = "";
bool messageReady = false;
#define RX D7
#define TX D8
SoftwareSerial meshBlynk(RX,TX);
// Data Coming fro Blynk App
BLYNK_WRITE(V1)
{
  board = 1;
  pin = 22;
  pin_status = param.asInt();
}




void setup()
{
  // Debug console
   // For Debugging purpose
  Serial.begin(115200);
  meshBlynk.begin(115200);
  
  // For sending data to another ESP32

  Blynk.begin(auth, ssid, pass); // Establishing Communication with Blynk Server
}

void loop()
{
  DynamicJsonDocument doc(1024);
  doc["board_status"] = board;
      doc["led"] = pin;
      doc["status"] = pin_status;
  serializeJson(doc,Serial);
  delay(1000);

 
  

  Blynk.run(); // Handeling Blynk Services
}
