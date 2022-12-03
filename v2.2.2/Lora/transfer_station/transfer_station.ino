#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
WiFiClient client;
SoftwareSerial Lora(D6, D5);  // rx tx

int ESP_Quit_Flag = 0;
char temp = '\0';
String box = "";

const char *host = "192.168.177.173";  //即将连接服务器网址/IP，192.168.177.173
int port = 8888;                       //即将连接服务器端口8888
const char *ssid = "yy";               // wifi名称(2.4GHZ)yy
const char *pass = "yuyan20021025";    //密码yuyan20021025

void setup() {
  Serial.begin(9600);  //初始化串口
  Lora.begin(9600);    //初始化软串口
  while (!Serial)      //判断串口是否接收到数据
    continue;
  WiFiConnect();  //接收到数据后连接WiFi
  while (1) {
    System_Begin();  //正常启动（包含debug）
  }
}

void loop() {
}

void WiFiConnect() {
  Serial.print("Connecting WiFi:");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.println("-----------------------------------------");
}

void System_Begin() {
  WiFiClient tcpclient;
  Serial.print("connecting to");
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  while (1) {
    if (!tcpclient.connect(host, port)) {
      Serial.println("connection failed");
      delay(3000);
    }
    if (tcpclient.connect(host, port)) {
      Serial.println("connection successfully");
      while (1) {
        delay(2000);
        if (tcpclient.available()) {
          temp = tcpclient.read();
          Serial.print("Get Command:");
          Serial.println(temp);
          switch (temp) {
            case 'S':  //系统正常工作
              {
                Serial.print("/*********************System Start*********************/");
                Serial.println("Please input -1 to exit");
                Serial.println("/*****************************************************/");
                Lora.print(temp);
                while (1) {
                  delay(1000);
                  Lora.flush();
                  while (Lora.available() > 0) {
                    temp = Lora.read();
                    if (temp == '\n') {
                      break;
                    } else {
                      box += temp;
                    }
                  }
                  Serial.print("Get Data:");
                  Serial.println(box);
                  tcpclient.print(box);
                  box = "";
                  if (Serial.available()) {
                    ESP_Quit_Flag = Serial.read();
                    if (ESP_Quit_Flag == -1) {
                      temp = '\0';
                      ESP_Quit_Flag = 0;
                      return;
                    }
                  }
                }
              }
            case '1':
              Lora.print(temp);
              break;
            case '2':
              Lora.print(temp);
              break;
            case '3':
              Lora.print(temp);
              break;
            case '4':
              Lora.print(temp);
              break;
            case '5':
              Lora.print(temp);
              break;
            case '6':
              Lora.print(temp);
              break;
            default:
              break;
          }
          delay(1000);
          while (Lora.available() > 0) {
            temp = Lora.read();
            if (temp == '\n') {
              break;
            } else {
              box += temp;
            }
          }
          Serial.print("Get Data:");
          Serial.println(box);
          tcpclient.print(box);
          temp = '\0';
          box = "";
        }
        if (Serial.available()) {
          ESP_Quit_Flag = Serial.read();
          if (ESP_Quit_Flag == -1) {
            ESP_Quit_Flag = 0;
            return;
          }
        }
      }
    }
  }
}