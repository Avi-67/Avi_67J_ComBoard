#include <Arduino.h>
#include <SPICREATE.h>
#include <SPIflash_67com.h>
#include <SoftwareSerial.h>
#include <67com_pin.h>

#define DATASETSIZE 256

SPICREATE::SPICreate SPIC;
Flash flash1;

int maxpage = 65535;

void readAllFlash()
{
  for (int j = 0; j < 65536; j++)
  {
    unsigned char readbuffer[DATASETSIZE];
    flash1.read(j * DATASETSIZE, readbuffer);
    for (int i = 0; i < DATASETSIZE; i++)
    {
      Serial.print(readbuffer[i]);
      Serial.print(',');
    }
  }
}

void setup() {
  Serial.begin(9600, SERIAL_8N1, RX3, TX3);

  SPIC.begin(VSPI, SCK, MISO, MOSI);
  flash1.begin(&SPIC, flashCS, 60000);

  pinMode(GPS_SW,OUTPUT);
  digitalWrite(GPS_SW, LOW);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  while (Serial.available())
  {
    unsigned char cmdFromPC = Serial.read();
    Serial.print("recieved cmd from Serial0 is '");
    Serial.write(cmdFromPC);
    Serial.println("'");

    if (cmdFromPC == 'r')
    {
      for (int j = 0; j < maxpage; j++)
      {
        unsigned char readbuffer[DATASETSIZE];
        flash1.read(j * DATASETSIZE, readbuffer);
        for (int i = 0; i < DATASETSIZE; i++)
        {
          if(readbuffer[i] != 0xFE){
            Serial.write(readbuffer[i]);
          }
        }
      }
    }
  }
}
 