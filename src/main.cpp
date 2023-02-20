#include <Arduino.h>
#include <SPICREATE.h>
#include <SPIflash_67com.h>
#include <SoftwareSerial.h>
#include <67com_pin.h>

TaskHandle_t task0;
SemaphoreHandle_t xMutex;

int mode = 0;

void commandtask(char cmd){
  switch (cmd)
  {
  case 'p':
    mode = 1;
    digitalWrite(GPS_SW, HIGH);
    Serial1.println("preparation mode");
    break;
  
  case 's':
    mode = 0;
    Serial.println("stop mode");

  case 'g':
    digitalWrite(GPS_SW, LOW);
    Serial.println("gps stopped");

  default:
    Serial1.println("wrong command");
    break;
  }
}

void GPSTask(void *pvparamater){
  for(;;){
    while(Serial1.available()){
      char cmd = Serial1.read();
      commandtask(cmd);
    }

    while(mode = 1){
      char data[80];
      for (int i = 0; i < 80; i++)
      {
        data[i] = 0;
      }

      int count = 0;
      do
      {
        if (Serial2.available())
        {
          data[count] = Serial2.read();
          count++;
        }
        if (count > 78)
        {
          break;
        }
      } while (data[count-1] != 0x0A); 

      Serial1.print(data);
    }
  }
}


void setup() {
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1); //twelite
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);//GPS RX:17 TX:16

  xMutex = xSemaphoreCreateMutex();//ミューテックス
  xTaskCreatePinnedToCore(GPSTask, "GPSTask", 1000, NULL, 1, NULL, 0);//execute core 0

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(GPS_SW,OUTPUT);

  digitalWrite(GPS_SW, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}


void loop() {
 }   