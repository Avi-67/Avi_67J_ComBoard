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

int mode;
int outputnum = 5;
char sign[6];
bool torb;
int maxpage = 65535;
int flashaddr = 0x00;
int flashfull = 0;
uint8_t tx_buf[256];

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
  Serial.begin(115200);
  Serial.println("ESP launched");
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1); //twelite
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);//GPS RX:17 TX:16
  Serial3.begin(9600, SWSERIAL_8N1, RX3, TX3, false, 256);//para
  Serial3.enableIntTx(false);
  Serial4.begin(9600, SWSERIAL_8N1, RX4, TX4, false, 256);//log
  Serial4.enableIntTx(false);
  Serial5.begin(9600, SWSERIAL_8N1, RX5, TX5, false, 256);//LED
  Serial5.enableIntTx(false);

  //SPIFlash
  SPIC.begin(VSPI, SCK, MISO, MOSI);
  flash1.begin(&SPIC, flashCS, 60000);
  

  pinMode(GPS_SW,OUTPUT);
  digitalWrite(GPS_SW, LOW);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  //honbann code
  if (Serial1.available())
  {
    char cmd = Serial1.read();

    if(mode == 0)
    {
      switch (cmd)
      {
      case 'b'://open servo(mode 0)
        Serial3.print('b');
        torb = 0;
        break;
      
      case 't'://close servo(mode 0)
        Serial3.print('t');
        torb = 1;
        break;

      case 'o'://select above servo(mode 0)
        Serial3.print('o');
        break;

      case 'c'://select bottom servo(mode 0)
        Serial3.print('c');
        break;

      case 'y'://turn on LED(mode 0)
        Serial5.print('y');
        break;

      case 'n'://turn off LED(mode 0)
        Serial5.print('n');
        break;

      case 'g'://stop gps(mode 0)
        digitalWrite(GPS_SW, LOW);
        for (int i =0; i < outputnum; i++)
        {
          Serial1.println("GPS stop");
        }
        break;

      case 'p'://preaparetion (mode 0 to mode1)
        Serial3.print('p');
        Serial4.print('p');
        Serial5.print('p');
        digitalWrite(GPS_SW, HIGH);
        mode = 1;  
        Serial1.println("preparation mode");
        break;
      }
    }else if (mode == 1)
    {
      switch (cmd)
      {
      case 'l':
        Serial3.print('l');
        Serial4.print('l');
        Serial5.print('l');
        mode = 2;
        Serial1.println("Logging mode");
        break;
      
      case 'd':
        Serial3.print('d');
        Serial4.print('d');
        Serial5.print('d');
        mode = 0;
        Serial1.println("delete mode");
        flash1.erase();
        Serial1.println("del finish");
        break;
      }
    }
    
    if (cmd == 's')//ストップモード(any mode to mode 0)
    {
      Serial3.print('s');
      Serial4.print('s');
      Serial5.print('s');
      Serial1.println("Stop mode");
      mode = 0;
    }
  }

if (Serial3.available())
{
  sign[0] = Serial3.read();
  switch (sign[0])
  {
    case 'o':
    if (torb)
    {
      for(int i =0; i< outputnum;i++)
      {
        Serial1.println("top servo open");
        delay(50);
      }
    }else{
      for(int i =0; i< outputnum;i++)
      {
        Serial1.println("bottom servo open");
        delay(50);
      }    
    }
    break;

    case 'c':
    if (torb)
    {
      for(int i =0; i< outputnum;i++)
      {
        Serial1.println("top servo close");
        delay(50);
      }
    }else{
      for(int i =0; i< outputnum;i++)
      {
        Serial1.println("bottom servo close");
        delay(50);
      }    
    }
    break;

    case 'p':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("pre mode from para");
      delay(50);
    }
    break;

    case 'l':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("log mode from para");
      delay(50);
    }
    break;

    case 'd':
    for (int i = 0; i< outputnum; i++)
    {
      Serial1.println("del mode from para");
      delay(50);
    }
    break;

    case 'f':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("del finish from para");
      delay(50);
    }
    break;

    case 'a':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("para phese1");
      delay(50);
    }
    break;

    case 'b':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("para phase2");
      delay(50);
    }
    break;
    
    case 's':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("stop mode from para");
      delay(50);
    }
    break;
  }  
}
  
if (Serial4.available())
{
  sign[1] = Serial4.read();
  switch (sign[1])
  {
    case 'p':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("pre mode from log");
      delay(50);
    }
    break;

    case 'l':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("log mode from log");
      delay(50);
    }
    break;

    case 'd':
    for (int i = 0; i< outputnum; i++)
    {
      Serial1.println("del mode from log");
      delay(50);
    }
    break;

    case 'f':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("del finish from log");
      delay(50);
    }
    break;

    case 's':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("stop mode from log");
      delay(50);
    }    
    break;
  }
}

if (Serial5.available())
{
  sign[2] = Serial5.read();

  switch (sign[2])
  {
    case 'p':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("pre mode from led");
      delay(50);
    }
    break;

    case 'l':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("log mode from led");
      delay(50);
    }
    break;

    case 'd':
    for (int i = 0; i< outputnum; i++)
    {
      Serial1.println("del mode from led");
      delay(50);
    }
    break;

    case 'f':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("del finish from led");
      delay(50);
    }
    break;

    case 's':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("stop mode from led");
      delay(50);
    }  
    break;

    case 'y':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("led on");
      delay(50);
    }
    break;

    case 'n':
    for (int i = 0; i < outputnum; i++)
    {
      Serial1.println("led off");
      delay(50);
    }
    break;    
  }
}
if(mode == 1||mode ==2){
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

  if (mode == 2 )
  {
    for (int i = 0; i < count; i++)
    {
      tx_buf[i] =data[i]; 
    }
    for (int i = count; i < 256; i++)
    {
      tx_buf[i] = 0xFE;
    }
    flash1.write(flashaddr, tx_buf);
    flashaddr += 0x100;
    flashfull +=1;
    if (flashfull == maxpage)
    {
      mode = 1;
    }
  }
}

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