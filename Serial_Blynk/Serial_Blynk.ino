#include <SoftwareSerial.h>

SoftwareSerial Serial_Blynk(D2, D3); //RX,TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial_Blynk.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("Hello From Blynk \n");
  Serial_Blynk.print("Hello From Blynk Serial \n");
  
  delay(1000);
}
