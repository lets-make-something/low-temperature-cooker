//
// FILE: TwoPin_DS18B20.ino
// AUTHOR: Rob Tillaart
// VERSION: 0.1.00
// PURPOSE: two pins for two sensors demo
// DATE: 2014-06-13
// URL: http://forum.arduino.cc/index.php?topic=216835.msg1764333#msg1764333
//
// Released to the public domain
//

#include <OneWire.h>
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 4

BluetoothSerial SerialBT;

OneWire oneWire_in(ONE_WIRE_BUS_1);
OneWire oneWire_out(ONE_WIRE_BUS_2);

DallasTemperature sensor_inhouse(&oneWire_in);
DallasTemperature sensor_outhouse(&oneWire_out);

void setup(void)
{
    SerialBT.begin("ESP32");
    //Serial.begin(9600);
    SerialBT.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20");

    sensor_inhouse.begin();
    sensor_outhouse.begin();
    SerialBT.println(sensor_inhouse.getDeviceCount());
    SerialBT.println(sensor_outhouse.getDeviceCount());
    //DeviceAddress* deviceAddress;
    //if (sensor_inhouse.getAddress(&deviceAddress, 0)) {
    //  Serial.println(deviceAddress);
    //}
    //if (sensor_inhouse.getAddress(&deviceAddress, 1)) {
    //  Serial.println(deviceAddress);
    //}
    
}

void loop(void)
{
  
    delay(200);
    SerialBT.println("Device count");
    SerialBT.println(sensor_inhouse.getDeviceCount());
    SerialBT.println(sensor_outhouse.getDeviceCount());
    
    SerialBT.print("Requesting temperatures...");
    sensor_inhouse.requestTemperatures();
    sensor_outhouse.requestTemperatures();
    
    delay(200);
    SerialBT.println(" done");

    
    for (int i=0; i < 9; i++) {
      DeviceAddress deviceAddress[10];
      if (sensor_inhouse.getAddress(deviceAddress[i], i)) {
        SerialBT.print(i);
        SerialBT.print(":");
        char buf[128];
        sprintf(buf, "TEST %X , %d ,", deviceAddress[i], deviceAddress[i]);
        SerialBT.print(buf);
      } else {
        SerialBT.print("fail!");
      }

      char out[128];
      sprintf(out, "Inhouse: %d : %f : ", i, sensor_inhouse.getTempCByIndex(i));
      SerialBT.println(out);
    }
    
    for (int i=0; i < 9; i++) {
      DeviceAddress deviceAddress[10];
      if (sensor_outhouse.getAddress(deviceAddress[i], i)) {
        SerialBT.print(i);
        SerialBT.print(":");
        char buf[128];
        sprintf(buf, "TEST2 %X , %d ,", deviceAddress[i], deviceAddress[i]);
        SerialBT.print(buf);
      } else {
        SerialBT.print("fail!");
      }

      char out[128]; 
      sprintf(out, "OUThouse: %d : %f : ", i, sensor_outhouse.getTempCByIndex(i));
      SerialBT.println(out);
    }
    /*
    Serial.print("Outhouse1: ");
    Serial.println(sensor_outhouse.getTempCByIndex(0));
    
    Serial.print("Outhouse2: ");
    Serial.println(sensor_outhouse.getTempCByIndex(1));
    */
    delay(1000);
}
