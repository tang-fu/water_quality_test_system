#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
SoftwareSerial Lora(D6, D5);  // rx tx

void WiFi_Creat(void);

const char *ssid = "Water Test";
const char *password = "12345678";
const char *host = "192.168.4.2";  //即将连接服务器网址/IP
const int port = 8888;             //即将连接服务器端口8888

int ESP_Quit_Flag = 0;
int WiFi_Flag = 0;
int Lora_Flag = 0;
char temp = '\0';
String box = "";


void setup() {
  Serial.begin(9600);
  Lora.begin(9600);
  while (!Serial)
    continue;
  WiFi_Creat();
  delay(10000);
  System_Begin();
}

void loop() {
}

void System_Begin() {
  WiFiClient tcpclient;
  /*******************print message*******************/
  Serial.print("connecting to device ");
  Serial.println(host);
  Serial.print("port:");
  Serial.println(port);
  /***************************************************/
  while (1) {
    if (!tcpclient.connect(host, port)) {
      Serial.println("connection failed");
      Serial.println("/*****************************************************/");
      delay(2000);
      Lora_Flag++;
      if (Lora_Flag == 2) {
        Lora_Flag = 0;
        Serial.println("WiFi AP Close");
        Serial.println("System will change to Lora");
        Serial.println("/*****************************************************/");
        WiFi.softAPdisconnect(1);  //关闭热点
        return;
      }
    }
    if (tcpclient.connect(host, port)) {
      Lora_Flag = 0;  //置零
      Serial.println("connection successfully");
      Serial.println("/*****************************************************/");
      while (1) {
        delay(2000);
        if (tcpclient.available()) {
          temp = tcpclient.read();
          Serial.print("Get Command:");
          Serial.println(temp);
          switch (temp) {
            case 'S':  //系统正常工作
              {
                Serial.println("/*********************System Start*********************/");
                Serial.println("Please input -1 to exit");
                Serial.println("/*****************************************************/");
                Lora.print(temp);
                while (1) {
                  Lora.print("s");
                  delay(1500);
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
                      Serial.println("/*********************Quit WiFi***********************/");
                      Serial.println("quit wifi successfully");
                      Serial.println("/*****************************************************/");
                      temp = '\0';
                      ESP_Quit_Flag = 0;
                      return;
                    }
                  }
                  if (!tcpclient.connect(host, port)) {
                    Lora.print("N");
                    Serial.println("error! device ip lost!");
                    Serial.println("/*****************************************************/");
                    WiFi_Flag = 1;
                    break;
                  }
                }
                break;
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
            case 's':  //multiple test
              Lora.print(temp);
              break;
            case '6':  //暂无功能
              Lora.print(temp);
              break;
            default:
              break;
          }
          if (WiFi_Flag == 1) {
            WiFi_Flag = 0;
            delay(1000);
            Serial.println("waiting for reconnect...");
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
            Serial.println("/*********************Quit WiFi***********************/");
            Serial.println("quit wifi successfully");
            Serial.println("/*****************************************************/");
            ESP_Quit_Flag = 0;
            return;
          }
        }
      }
    }
  }
}

void WiFi_Creat(void) {
  Serial.println();
  Serial.println("/*********************WiFi AP Start********************/");
  WiFi.softAP(ssid, password);
  Serial.print("Access Point: ");
  Serial.println(ssid);
  Serial.print("IP address: ");     // 以及NodeMCU的IP地址
  Serial.println(WiFi.softAPIP());  // 通过调用WiFi.softAPIP()可以得到NodeMCU的IP地址
  Serial.println("/******************************************************/");
}