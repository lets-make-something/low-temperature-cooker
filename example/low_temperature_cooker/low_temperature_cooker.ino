#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

#define LCD_ADRS 0x3E
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BluetoothSerial SerialBT;

static int mode = 1;
static int terminal_mode = 0;
static int debug = 0;
static float group1_average = 0.0;
static float group2_average = 0.0;
static float air_temp = 0.0;

char state_name_1[]="Monitoring      ";
char state_name_2[]="Set Target Temp ";
char state_name_3[]="Testing         ";

//mode_1
void exec_mode_1() {
  char buf[17] = "";
  memset(buf, 0, 17);

  debugPrintln("writeData line1 start");
  for(int i=0; i<16; i++){
    writeData(state_name_1[i]);
  }

  writeCommand(0x40+0x80);
  debugPrintln("writeData line2 start");

  sprintf(buf, "%0.1f,%0.1f,%0.1f", group1_average, group2_average, air_temp);
  debugPrintln("writeData start");
  for (int i=0; i<16; i++) {
    writeData(buf[i]);
  }
  debugPrintln("writeData end");
}

//mode_2
void exec_mode_2() {
  debugPrintln("writeData line1 start");
  for(int i=0; i<16; i++){
    writeData(state_name_2[i]);
  }
  debugPrintln("writeData end");
}


//mode_3
void exec_mode_3() {
  debugPrintln("writeData line1 start");
  for(int i=0; i<16; i++){
    writeData(state_name_3[i]);
  }
  debugPrintln("writeData end");
}

void writeData(byte t_data){
  Wire.beginTransmission(LCD_ADRS);
  Wire.write(0x40);
  Wire.write(t_data);
  Wire.endTransmission();
  delay(1);
}

void writeCommand(byte t_command) {
  Wire.beginTransmission(LCD_ADRS);
  Wire.write(0x00);
  Wire.write(t_command);
  Wire.endTransmission();
  delay(10);
}

void init_LCD() {
  delay(100);
  writeCommand(0x38);
  delay(20);
  writeCommand(0x39);
  delay(20);
  writeCommand(0x14);
  delay(20);
  writeCommand(0x73);
  delay(20);
  writeCommand(0x52);
  delay(20);
  writeCommand(0x6C);
  delay(20);
  writeCommand(0x38);
  delay(20);
  writeCommand(0x01);
  delay(20);
  writeCommand(0x0C);
  delay(20);
}

void debugPrintln(char* str) { 
  if (!debug) { return; }
  myPrintln(str);
}

void debugPrint(char* str) { 
  if (!debug) { return; }
  myPrint(str);
}

void myPrintln(char* str) { 
  if(terminal_mode == 0) {
    Serial.println(str);
  } else {
    SerialBT.println(str);
  }
}

void myPrint(char* str) { 
  if(terminal_mode == 0) {
    Serial.print(str);
  } else {
    SerialBT.print(str);
  }
}

void setup(void) { 
  if (terminal_mode == 0) {
    Serial.begin(9600);
  } else {
    SerialBT.begin("LOWTEMP");
  }
  myPrintln(" Low Temperature init...");

  sensors.begin();
  myPrintln("init_LCD start");
  Wire.begin();
  init_LCD();    
  myPrintln("init_LCD end");
}

void printTemplature() {
  float group1_total = 0;
  float group2_total = 0;
  float air_temp_total = 0;
  char count_string[3];
  sprintf(count_string, "%d", sensors.getDeviceCount());

  
  debugPrint("Device count :");
  debugPrintln(count_string);
  debugPrint("Requesting temperatures...");
  sensors.requestTemperatures();
  debugPrintln(" done");

  for (int i=0; i < 9; i++) {
    DeviceAddress deviceAddress[10];
    char buf[256];
    if (sensors.getAddress(deviceAddress[i], i)) {
      sprintf(buf, "OK,%d,%X,%d,%f", i, deviceAddress[i], deviceAddress[i], sensors.getTempCByIndex(i));
      myPrintln(buf);
      if (i < 4) {
        group1_total += sensors.getTempCByIndex(i);
      } else if (i < 8) {
        group2_total += sensors.getTempCByIndex(i);
      } else if (i == 8) {
        air_temp_total += sensors.getTempCByIndex(i);
      }
    } else {
      myPrintln("NG,,,,");
    }
  }
  group1_average = group1_total / 4.0;
  group2_average = group2_total / 4.0;
  air_temp = air_temp_total;
}

void loop() {
  exec_mode_1();
  printTemplature();
  delay(1000);
}



