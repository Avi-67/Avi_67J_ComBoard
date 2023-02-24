#include <Arduino.h>
#include <SPICREATE.h>
#include <SPIflash_67com.h>
#include <SoftwareSerial.h>
#include <67com_pin.h>

#define DATASETSIZE 256

SemaphoreHandle_t xSemaphore;
SemaphoreHandle_t xSemaphore1;
char share_cmd = ' ';
char recmd = ' ';

SoftwareSerial Serial3;
SoftwareSerial Serial4;
SPICREATE::SPICreate SPIC;
Flash flash1;

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

void sharecommand(char cmd){
  if(xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE){
    share_cmd = cmd;
    xSemaphoreGive(xSemaphore);
  }
}

void commandtask(char cmd){
  switch (cmd)
  {
  case 'p':
    //mode = 1;
    //digitalWrite(GPS_SW, HIGH);
    Serial.print(cmd);
    Serial3.print(cmd);
    //share_cmd(cmd)
    Serial1.println("preparation mode");
    break;
  
  case 's':
    //mode = 0;
    Serial.print(cmd);
    Serial3.print(cmd);    
    //share_cmd(cmd);
    Serial1.println("stop mode");
    break;

  case 'l':
    Serial.print(cmd);
    Serial3.print(cmd);
    Serial1.println("log mode");
    break;

  case 'g':
    digitalWrite(GPS_SW, LOW);
    Serial1.println("gps stopped");
    break;

  case 'd':
    Serial.print(cmd);
    Serial3.print(cmd);
    Serial1.println("delete mode");
    break;

  default:
    Serial1.println("wrong command");
    break;
  }
}


void mainTask(void *pvparamater){
  for(;;){
    while(Serial1.available()){
      char cmd = Serial1.read();
      commandtask(cmd);
    }
    if(Serial.available()){
      char rcmd = Serial.read();
      Serial1.print(rcmd);
      Serial1.println(" from para");
    }
    if(Serial3.available()){
      char rcmd =Serial3.read();
      Serial1.print(rcmd);
      Serial1.println(" from log");
    }
    if(xSemaphoreTake(xSemaphore1, (TickType_t)10) == pdTRUE){
      Serial1.print(recmd);
      Serial1.println(" from led");
    }
    delay(1);//WDT    
  }
}

void Serial4Task(void *pvparamater){
  for(;;){
    char cmd;
    if(xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE){
      cmd = share_cmd;
      xSemaphoreGive(xSemaphore);
      Serial4.print(share_cmd);
    }
    if (Serial4.available())
    {
      char cmd4 = Serial4.read();
    if(xSemaphoreTake(xSemaphore1, (TickType_t)10) == pdTRUE){
      recmd = cmd4; 
      xSemaphoreGive(xSemaphore1);
      }
    }
  }
  delay(1);//WDT 
}

void setup() {
  Serial.begin(9600, SERIAL_8N1, RX3, TX3);
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1); //twelite
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);//GPS RX:17 TX:16
  Serial3.begin(9600, SWSERIAL_8N1, RX4, TX4, false, 256);
  Serial4.begin(9600, SWSERIAL_8N1, RX5, TX5, false, 256);

  pinMode(GPS_SW, OUTPUT);
  digitalWrite(GPS_SW, LOW);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(mainTask, "mainTask", 8192, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(Serial4Task, "Serial4Task", 8192, NULL, 1, NULL, 1);
}

void loop() {
}   