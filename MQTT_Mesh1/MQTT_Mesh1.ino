#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "vats";
const char* password = "31238069";
const char* mqtt_server = "broker.mqtt-dashboard.com";

int l=1;
int q;
#define N 2 // no of motors
int k;
int b1,b2;
int dir[N],pwr[N],umax=255;
// pid constants
float kp=0.3,kd=0.025*1000000,ki=0;
long pos[N],target[N],eprev[N],prevT[N];
long posi[N];
float eintg[N],deltaT[N];

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
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

void pid(long value, long tar, int dt, int i)
{
  long e = tar - value;
  float ederv = (e - eprev[i]) / dt;
  eintg[i] = eintg[i] + e * dt;
  eprev[i] = e;
  float u = kp * e + kd * ederv + ki * eintg[i];
  pwr[i] = fabs(u);
  if (pwr[i] > umax)
  {
    pwr[i] = umax;
  }
  dir[i] = 1;
  if (u < 0)
  {
    dir[i] = -1;
  }
  if (e < 500 && e > -500)
  {
    for (q = 0; q < N; q++) {
      target[q] = 0;
      posi[q] = 0;
      pos[q] = 0;
    }
    //dir =0;
    
  }
}
// func to drive
void setMotor(int di, int pwm, int in1, int in2)
{
  if (di == 1)
  {
    analogWrite(in1, pwm);
    digitalWrite(in2, 0);
  }
  else if (di == -1)
  {
    digitalWrite(in1, 0);
    analogWrite(in2, pwm);
  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

}
void setTarget(int dis)
{

  for (k = 0; k < N; k++)
  {
    target[k] = (dis * 14 * 300) / (PI * 4.4);
  }
  //target= 200;
}
const int enca[] = {D2, D1};
const int encb[] = {D3, D4};
//const int pwm[] = {9,13};
const int in1[] = {D5, D7};
const int in2[] = {D6, D8};

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
  
  setTarget(str.toInt());

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
      client.subscribe("esp2");
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


  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  for (int k = 0; k < 2; k++) {
    pinMode(enca[k], INPUT);
    pinMode(encb[k], INPUT);

    pinMode(in1[k], OUTPUT);
    pinMode(in2[k], OUTPUT);
    dir[k] = 0, pwr[k] = 0;
    pos[k] = 0, target[k] = 0, eprev[k] = 0, prevT[k] = 0;
    posi[k] = 0;
    eintg[k] = 0, deltaT[k] = 0;
  }
  attachInterrupt(digitalPinToInterrupt(enca[0]), readEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(enca[1]), readEncoder2, RISING);
  //     setTarget(50);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (k = 0; k < 2; k++)
  {
    long currT = micros();
    // deltaT[k]=(currT-prevT[k])/(1.0e6);
    /*Serial.print(" deltaT ");
      Serial.print(k+1);
      Serial.print(" ");*/

    deltaT[k] = (currT - prevT[k]);

    // Serial.print(deltaT[k]);
    prevT[k] = currT;
    noInterrupts();
    pos[k] = posi[k];
    interrupts();
    //pid(long value,long tar,int dt,int i)
    pid(pos[k], target[k], deltaT[k], k);
    //setMotor(int di,int pwm,int in1,int in2)

    setMotor(dir[k], pwr[k], in1[k], in2[k]);
  }
}

IRAM_ATTR void readEncoder1()
{
   int b1 = digitalRead(encb[0]);
  if (b1>0)
  {
    posi[0]++;
  }
  else
  {posi[0]--;}
  
}
IRAM_ATTR void readEncoder2()
{
   b2 = digitalRead(encb[1]);
  if (b2>0)
  {
    posi[1]++;
  }
  else
  {posi[1]--;}
  
}
