#include <Arduino.h>
#include <SPICREATE.h>
#include <SPIflash_67com.h>
#include <SoftwareSerial.h>
#include <67com_pin.h>

#define DATASETSIZE 256

SoftwareSerial Serial3;
SoftwareSerial Serial4;
SoftwareSerial Serial5;
SPICREATE::SPICreate SPIC;
Flash flash1;

class uart {
  
}

void setup() {
  Serial.begin(9600, SERIAL_8N1, RX3, TX3)
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1); //twelite
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);//GPS RX:17 TX:16
  Serial4.begin(9600, SWSERIAL_8N1, RX4, TX4, false, 256);//log
  Serial4.enableIntTx(false);
  Serial5.begin(9600, SWSERIAL_8N1, RX5, TX5, false, 256);//LED
  Serial5.enableIntTx(false);
}

void loop() {
  if(Serial1.available()){
    char cmd = Serial1.read();
    Serial1.print(cmd);
    Serial1.println(" from twelite");
    Serial3.print(cmd);
    Serial4.print(cmd);
    Serial5.print(cmd);
  }

  if(Serial.available()){
    char cmd3 = Serial.read();
    Serial.write(cmd3);
    Serial.println(" from para")
  }

  Serial4.listen();
  if(Serial4.available()){
  char cmd4 = Serial4.read();
  Serial4.print('j');
  Serial1.write(cmd4);
  Serial1.println(" from log");
  }

  Serial5.listen();
  if(Serial5.available()){
    Serial5.print('j');
    char cmd5 = Serial5.read();
    Serial1.write(cmd5);
    Serial1.println(" from led");
  }
}   