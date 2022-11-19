/*************************************************************
 * 
 *   This is the code for Internet to Mesh Project
 *   
 *   This code needs to be uploaded on the board from which 
 *   we will be receiving the data from another ESP32 board 
 *   via UART (TX2,RX2)
 * 

    Code written by team techiesms 

    To watch the full tutorial video of 57this project, click the link below
    https://youtu.be/yZvhygHA_sw

 *************************************************************/
// Necessary Libraries
#include "painlessMesh.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>


// Serial pins of ESP32
#define RX D7
#define TX D8
SoftwareSerial meshBlynk(RX,TX);
// WiFi Credentials for Mesh Networking
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


// Variables
int led;
int led_status;
int board_status;
int board;
int pin;
int pin_status;
bool message_ready = true;
bool message_received = false;
String message = "";


Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void send_request() ;

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
Task taskSendRequest( TASK_SECOND * 1 , TASK_FOREVER, &send_request );

void sendMessage()
{

  DynamicJsonDocument doc(1024);
  doc["board"] = board_status;
  doc["pin"] = led;
  doc["status"] =  led_status;
  //doc["status_1"] = led_status_1;
  String msg ;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
  Serial.print("Mesh Broadcast - "); Serial.println(msg);

 // taskSendMessage.setInterval((TASK_SECOND * 1));
}


// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);
  meshBlynk.begin(115200);
  


void loop()
{




  // it will run the user scheduler as well
  mesh.update();
  //timer.run();
}
