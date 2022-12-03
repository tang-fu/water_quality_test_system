#include "system_debug.h"
#include "sensor.h"
#include <SoftwareSerial.h>

void System_Begin(void);
void Debug_Or_Quit(void);
void WiFi_Debug(void);
void Lora_Debug(void);
void WiFi_Mode(void);
void Lora_Mode(void);

char Get_Comand_temp = '0';
int System_Start_Key = 0;
int Quit_System_Flag = 0;
int Debug_Flag = 0;

SoftwareSerial Lora(5, 4);   //定义软串口rx,tx(4,5)
SoftwareSerial esp32(6, 7);  //定义软串口rx,tx(6,7)

void setup() {
  Serial.begin(9600);
  esp32.begin(9600);
  Lora.begin(9600);
  System_Begin();
}

void loop() {
}
/**
  * @brief  系统启动函数，直接进入Start Debug(esp调试)状态，收到esp指令后可进入系统开始状态，并退出esp调试    
  * @param  none  
  * @retval none
  */
void System_Begin(void) {
  Serial.println();
  Serial.println("/*****************************************************/");
  Serial.println("System Begin! Please input command");
  Serial.println("1 Start Debug");
  //Serial.println("2 System Start");
  Serial.println("-1 Exit System");
  Serial.println("/*****************************************************/");
  while (1) {
    Read_Sensor();              //获取传感器数据
    Debug_Or_Quit();            //读取串口数据判断退出或者进入Debug
    if (System_Start_Key == 0)  //接收到esp的值判断是否进入调试
    {
      WiFi_Debug();
      Lora_Debug();
    }
    if (System_Start_Key == 1) {
      WiFi_Mode();
    }
    if (System_Start_Key == 2) {
      Lora_Mode();
    }
  }
}
/**
  * @brief  esp8266的调试菜单，会根据esp传入的值进行选择值，并将某一传感器的数据 返回给esp8266    
  * @param  none  
  * @retval none
  */
void WiFi_Debug(void) {
  if (esp32.available() > 0) {
    Get_Comand_temp = esp32.read();
    switch (Get_Comand_temp) {
      case '1':
        Serial.println("Get Command:TDS");
        esp32.print("TDS Sensor Working!");
        esp32.print("TDS:" + String(Return_Data(Get_Comand_temp)) + "\n");
        break;
      case '2':
        Serial.println("Get Command:PH");
        esp32.print("PH Sensor Working!");
        esp32.print("PH:" + String(Return_Data(Get_Comand_temp)) + "\n");
        break;
      case '3':
        Serial.println("Get Command:Tem");
        esp32.print("TEM Sensor Working!");
        esp32.print("TEM:" + String(Return_Data(Get_Comand_temp)) + "\n");
        break;
      case '4':
        Serial.println("Get Command:NTU");
        esp32.print("NTU Sensor Working!");
        esp32.print("NTU:" + String(Return_Data(Get_Comand_temp)) + "\n");
        break;
      case 'S':
        Serial.println("/*****************************************************/");
        Serial.println("System_Start!");
        Serial.println("/*****************************************************/");
        System_Start_Key = 1;
        break;
      case 's':  //multiple
        esp32.print(String(Return_Data('1')) + 'A' + String(Return_Data('2')) + 'B' + String(Return_Data('3')) + 'C' + String(Return_Data('4')) + 'D' + '\n');
      default:
        break;
    }
    Get_Comand_temp = '0';
  }
}

void Lora_Debug(void) {
  if (Lora.available() > 0) {
    Get_Comand_temp = Lora.read();
    switch (Get_Comand_temp) {
      case '1':
        Serial.println("Get Command:TDS");
        Lora.print("TDS Sensor Working!");
        Lora.println("TDS:" + String(Return_Data(Get_Comand_temp)));
        break;
      case '2':
        Serial.println("Get Command:PH");
        Lora.print("PH Sensor Working!");
        Lora.println("PH:" + String(Return_Data(Get_Comand_temp)));
        break;
      case '3':
        Serial.println("Get Command:Tem");
        Lora.print("TEM Sensor Working!");
        Lora.println("TEM:" + String(Return_Data(Get_Comand_temp)));
        break;
      case '4':
        Serial.println("Get Command:NTU");
        Lora.write("NTU Sensor Working!");
        Lora.println("NTU:" + String(Return_Data(Get_Comand_temp)));
        break;
      case 'S':
        Serial.println("/*****************************************************/");
        Serial.println("System_Start!");
        Serial.println("/*****************************************************/");
        System_Start_Key = 2;
        break;
      case 's':  //multiple
        Lora.print(String(Return_Data('1')) + 'A' + String(Return_Data('2')) + 'B' + String(Return_Data('3')) + 'C' + String(Return_Data('4')) + 'D' + '\n');
      default:
        break;
    }
    Get_Comand_temp = '0';
  }
}
/**
  * @brief  退出系统或进入Debug 函数，退出时需要两次输入-1，期间间隔不能超过两秒。输入1进入Debug调试界面（延时较高，
            可将进入Debug的if复制到System_Begin函数当中）   
  * @param  none  
  * @retval none
  */
void Debug_Or_Quit(void) {
  if (Serial.available()) {
    Debug_Flag = Quit_System_Flag = Serial.parseInt();
    if (Quit_System_Flag == -1) {  //双层-1验证退出system，Debug单层1进入
      Quit_System_Flag = 0;
      Debug_Flag = 0;
      Serial.println("check exit system");
      delay(2000);
      if (Serial.available()) {
        Quit_System_Flag = Serial.parseInt();
        if (Quit_System_Flag == -1) {
          Quit_System_Flag = 0;
          Serial.println("exit system successfully!");
          while (1) {}
        }
      }
      Serial.println("error!");
      Serial.println("/*****************************************************/");
    }
    if (Debug_Flag == 1) {  //可移动至System_Begin函数当中
      Quit_System_Flag = 0;
      System_Start_Key = 0;  //关闭其他功能
      Debug_Flag = 0;
      Debug_Start();  //进入调试
      Serial.println("/*****************************************************/");
      Serial.println("You are in System!");
      Serial.println("Please input 1 to start debug");
      Serial.println("Please input double -1 to exit");
      Serial.println("/*****************************************************/");
    }
  }
}
/**
  * @brief  WiFi模式函数，包含对连接状态的监测，若esp未与上位机连接，则不发送数据   
  * @param  none  
  * @retval none
  */
void WiFi_Mode(void) {
  if (esp32.available() > 0) {
    Get_Comand_temp = esp32.read();
    switch (Get_Comand_temp) {
      case 'N':
        Serial.println("WiFi Status error!");
        Serial.println("/*****************************************************/");
        Get_Comand_temp = '0';
        System_Start_Key = 0;
        return;
      case 's':
        esp32.print(String(Return_Data('1')) + 'A' + String(Return_Data('2')) + 'B' + String(Return_Data('3')) + 'C' + String(Return_Data('4')) + 'D' + '\n');
        break;
      default:
        break;
    }
    Get_Comand_temp = '0';
  }
}
void Lora_Mode(void) {
  if (Lora.available() > 0) {
    Get_Comand_temp = Lora.read();
    switch (Get_Comand_temp) {
      case 'N':
        Serial.println("Lora Status error!");
        Serial.println("/*****************************************************/");
        Get_Comand_temp = '0';
        System_Start_Key = 0;
        return;
      case 's':
        Lora.print(String(Return_Data('1')) + 'A' + String(Return_Data('2')) + 'B' + String(Return_Data('3')) + 'C' + String(Return_Data('4')) + 'D' + '\n');
        break;
      default:
        break;
    }
    Get_Comand_temp = '0';
  }
}