// Master Code (master.ino)
#include <Wire.h>
#define SLAVE1_ADDR 0x0A // Example addresses for slaves (adjust as needed)
#define SLAVE2_ADDR 0x0B
#define SLAVE3_ADDR 0x0C
#define SLAVE4_ADDR 0x0D
#define SLAVE5_ADDR 0x0E
#define SLAVE6_ADDR 0x0F
int sleep_num = 7;
int next_recv;
int d = 1;
char data[5];// Array to store received data
void setup() {
  Wire.begin();  // Initialize I2C as Master
  Serial.begin(9600); 
  Serial.println("I2C Master Demonstration");
}

void loop() {
  while(d)
  {
    //next_recv = requestData(SLAVE1_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    next_recv = requestData(SLAVE2_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    //next_recv = requestData(SLAVE3_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    //next_recv = requestData(SLAVE4_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    //next_recv = requestData(SLAVE5_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    //next_recv = requestData(SLAVE6_ADDR);
    if(next_recv != 0)
    {
      d=0;
      break;
    }
    Serial.println(next_recv);
    delay(500);
  }
  Wire.end();
  Serial.print("Soldier ID : ");
  Serial.print(data[0]);
  Serial.println(data[1]);
  Serial.print("Weapon ID : ");
  Serial.println(data[2]);
  Serial.print("Firing Mode : ");
  Serial.println(data[3]);
 // Adjust delay as needed
 delay(10000);
}

int requestData(byte slaveAddress) {
  Wire.requestFrom(slaveAddress, 4); // Request 4 bytes 
  int i = 0;
  while (Wire.available() && i < 4) {
    data[i] = Wire.read();
    i++;
  }
  data[i] = '\0'; // Null-terminate the string
  Serial.print("Slave ");
  Serial.print(slaveAddress, HEX);
  Serial.print(": ");
  Serial.println(data);
  if(data[0]!='0')
  {
    Serial.println("value is not 0");
    Wire.write(sleep_num);
    Wire.endTransmission();
    Wire.end();
    return 1;
  }
  else
  {
    return 0;
  }
}