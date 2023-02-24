#include <Arduino.h>
#include <SPICREATE.h>
#include <SPIflash_67com.h>
#include <SoftwareSerial.h>
#include <67com_pin.h>

TaskHandle_t task0;
SemaphoreHandle_t xMutex;

SoftwareSerial Serial3;
SoftwareSerial Serial4;
SoftwareSerial Serial5;

int mode = 0;
int share_flag;
char share_cmd;

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

void btobTask(void *pvparamater){
}

void setup() {
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1); //twelite
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);//GPS RX:17 TX:16

  Serial3.begin(9600, SWSERIAL_8N1, RX3, TX3, false, 256);//para
  Serial3.enableIntTx(false);
  Serial4.begin(9600, SWSERIAL_8N1, RX4, TX4, false, 256);//log
  Serial4.enableIntTx(false);
  Serial5.begin(9600, SWSERIAL_8N1, RX5, TX5, false, 256);//LED
  Serial5.enableIntTx(false);

  xMutex = xSemaphoreCreateMutex();//ミューテックス
  xTaskCreatePinnedToCore(GPSTask, "GPSTask", 8192, NULL, 1, NULL, 0);//execute core 0
  xTaskCreatePinnedToCore(btobTask, "btobTask", 8192, NULL, 1, NULL, 1);

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