#include "sensor.h"
#include "GPS.h"
#include <Arduino.h>

char Debug_Order;
char Sensor_Order;
int Quit_Debug_Flag = 0;

void Debug_Menu(void);
void Debug_Start(void);
void Switch_Order(int Debug_Order);
void Sensor_Debug_Menu(void);

/*
    Sensor_Debug_Menu
*/
void Sensor_Debug_Menu(void) {
  while (1) {
    Serial.println("This is Sensor_Debug!");
    Serial.println("Please choose one sensor!");
    Serial.println("1   TDS");
    Serial.println("2   PH");
    Serial.println("3   Tem");
    Serial.println("4   NTU");
    Serial.println("5   multiple debug");
    Serial.println("0   back to debug_menu");
    Serial.println("/*****************************************************/");
    while (1) {
      if (Serial.available() > 0)
        break;
    }
    Sensor_Order = Serial.parseInt();
    switch (Sensor_Order) {
      case 0:
        Quit_Debug_Flag = 1;
        return;
      case 1:
        Choose_Sensor(Sensor_Order);
        break;
      case 2:
        Choose_Sensor(Sensor_Order);
        break;
      case 3:
        Choose_Sensor(Sensor_Order);
        break;
      case 4:
        Choose_Sensor(Sensor_Order);
        break;
      case 5:
        Choose_Sensor(Sensor_Order);
        break;
      default:
        Serial.println("Please input 0-5!");
        Serial.println("/*****************************************************/");
        break;
    }
  }
}

/*
    switch_order
*/
void Switch_Order(int Debug_Order) {
  switch (Debug_Order) {
    case 0:
      Quit_Debug_Flag = -1;
      break;
    case 1:
      Sensor_Debug_Menu();
      Quit_Debug_Flag = 1;
      break;
    case 2:
      Quit_Debug_Flag = 1;
      break;
    case 3:
      GNSS_Fun();
      Quit_Debug_Flag = 1;
      break;
    default:
      break;
  }
}

/*
    debug menu
*/
void Debug_Menu(void) {
  Serial.println("/*****************************************************/");
  Serial.println("Please input your command!");
  Serial.println("1   Sensor_Debug");
  Serial.println("2   Communication_Debug");
  Serial.println("3   GPS_Debug");
  Serial.println("0   exit");
  Serial.println("/*****************************************************/");
}

/**
  * @brief  调试开始函数，读取菜单函数，选择函数，以及退出Debug功能   
  * @param  none  
  * @retval none
  */
void Debug_Start(void) {
  Serial.println("Get in Debug successfully!");
  while (1) {
    Debug_Menu();
    while (1) {
      if (Serial.available()) {
        Debug_Order = Serial.parseInt();
        Switch_Order(Debug_Order);
      }
      if (Quit_Debug_Flag == 1) {
        Serial.println("exit successfully!");
        Quit_Debug_Flag = 0;
        break;
      }
      if (Quit_Debug_Flag == -1) {
        Serial.println("exit debug successfully!");
        Quit_Debug_Flag = 0;
        return;
      }
    }
  }
}