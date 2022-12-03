#include <Arduino.h>
#include <GNSS.h>

/******************************GPS******************************/
int GPS_Debug_Flag = 0;
static SpGnss Gnss;
double Lat, Lon;  //维度,经度

void GNSS(void);
void GNSS_Fun(void);

/*****************************GPS_Fun*******************************/
void GNSS_Fun(void) {
  Serial.println("This is GPS_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  Gnss.begin();
  Gnss.setInterval(5);
  Gnss.start();
  while (1) {
    GNSS();
    if (Serial.available()) {
      GPS_Debug_Flag = Serial.parseInt();
      if (GPS_Debug_Flag == -1) {
        GPS_Debug_Flag = 0;
        Serial.println("/*****************************************************/");
        return;
      }
    }
  }
}
/*****************************GPS*******************************/
void GNSS(void) {
  if (Gnss.waitUpdate(-1)) {
    SpNavData NavData;
    Gnss.getNavData(&NavData);
    Lat = NavData.latitude;
    Lon = NavData.longitude;
    Lat -= 0.002351;
    Lon += 0.002323;

    Serial.print("Lat=");
    Serial.print(Lat, 6);
    Serial.print(", Lon=");
    Serial.print(Lon, 6);
    Serial.print(", Satellites=");
    Serial.print(NavData.numSatellites);
    Serial.println("");
  }
  if (Gnss.isUpdate()) {
    SpNavData NavData;
    Gnss.getNavData(&NavData);
  }
}