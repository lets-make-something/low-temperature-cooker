#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{ 
    Serial.begin(9600);
    Serial.println("Temperature Demo DS18B20");

    sensors.begin();
    Serial.println(sensors.getDeviceCount());
}

void loop(void)
{
    Serial.print("Device count :");
    Serial.println(sensors.getDeviceCount());
    
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println(" done");

    for (int i=0; i < 9; i++) {
      DeviceAddress deviceAddress[10];
      char buf[128];
      char out[128];

      Serial.print(i);
      Serial.print(",");
      if (sensors.getAddress(deviceAddress[i], i)) {
        sprintf(buf, "getAddress OK,%X,%d,", i, deviceAddress[i], deviceAddress[i]);
        Serial.print(buf);
      } else {
        Serial.print("getAddress NG,,,");
      }

      sprintf(out, "Temp, %f", sensors.getTempCByIndex(i));
      Serial.println(out);
    }
    
    delay(1000);
}
