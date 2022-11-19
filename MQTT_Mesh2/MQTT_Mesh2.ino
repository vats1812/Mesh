#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "vats";
const char* password = "31238069";
const char* mqtt_server = "5.196.95.208";
#define m1 D5
#define m2 D6
//#define m3 D2
//#define m4 D8
#define en D7

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void forward() {
  digitalWrite(m1, HIGH);
//  digitalWrite(m3, HIGH);
  digitalWrite(m2, LOW);
//  digitalWrite(m4, LOW);
  Serial.print("Forward Initated");
}

void backward() {
  digitalWrite(m1, LOW);
//  digitalWrite(m3, LOW);
  digitalWrite(m2, HIGH);
//  digitalWrite(m4, HIGH);
  Serial.print("Backward Initated");
}


void stopmtr() {
  digitalWrite(m1, LOW);
//  digitalWrite(m3, LOW);
  digitalWrite(m2, LOW);
//  digitalWrite(m4, LOW);
  Serial.print("Stop Initated");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String str;
  for (int i = 0; i < length; i++) {
    str += (char)payload[i];
    Serial.print((char)payload[i]);
  }

  Serial.println();
  if (str == "fo") {
    forward();
//    delay(5000);
//    stopmtr();
  } if (str == "b") {
    backward();
//    delay(5000);
//    stopmtr();
  }
  if (str == "stop") {
    stopmtr();
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("message", "hello world");
      // ... and resubscribe
      client.subscribe("esp1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
//  pinMode(m3, OUTPUT);
//  pinMode(m4, OUTPUT);
  pinMode(en, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
