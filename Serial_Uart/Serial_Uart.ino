#include <SoftwareSerial.h>

SoftwareSerial Serial_Uart(D2, D3); //RX,TX

char c;
String dataIn;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial_Uart.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial_Uart.available() > 0)
  {
    c = Serial_Uart.read();
    if (c == '\n') {
      break;
    }
    else {
      dataIn += c;
    }
  }
  if (c == '\n') {
    Serial.println(dataIn);
    c = 0;
    dataIn = "";
  }
}
