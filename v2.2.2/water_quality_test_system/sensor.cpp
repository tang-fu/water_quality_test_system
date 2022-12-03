#include <DS18B20_S.h>
#include <Arduino.h>

char Choose_Sensor_Order;
int Exit_Flag = 0;

/*****************************TDS*******************************/
#define TdsSensorPin A0    // A0
#define VREF 5.0           //模拟参考电压（伏特）
#define SCOUNT 30          //样本点总和
int analogBuffer[SCOUNT];  //将模拟值存储在数组中
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;
/******************************PH******************************/
#define SensorPin A1  // A1口
#define Offset 0.00   //偏差补偿
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40     //收集次数
int pHArray[ArrayLenth];  //存储传感器反馈的平均值
int pHArrayIndex = 0;
float pHValue, voltage;
/******************************Tem******************************/
DS18B20_S TEM(9);  // Pin 9口
float tem_Value = 0;
/******************************NTU******************************/
float TU = 0.0;  // A2口
float TU_value = 0.0;
float TU_calibration = 0.0;
float temp_data = 25.0;
float K_Value = 3347.19;

void TDS(void);
void PH(void);
void Tem(void);
void NTU(void);
void Choose_Sensor(char Choose_Sensor_Order);
int getMedianNum(int bArray[], int iFilterLen);
double avergearray(int *arr, int number);
void Multiple_Debug(void);
void TDS_Fun(void);
void PH_Fun(void);
void Tem_Fun(void);
void NTU_Fun(void);
float Return_Data(char Type);
void Read_Sensor(void);

void Choose_Sensor(char Choose_Sensor_Order) {
  switch (Choose_Sensor_Order) {
    case 1:
      TDS_Fun();
      break;
    case 2:
      PH_Fun();
      break;
    case 3:
      Tem_Fun();
      break;
    case 4:
      NTU_Fun();
      break;
    case 5:
      Multiple_Debug();
      break;
    default:
      break;
  }
}
/*****************************TDS_Fun*******************************/
void TDS_Fun(void) {
  Serial.println("This is TDS_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  while (1) {
    TDS();
    Serial.print("TDSValue:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
    delay(500);
    if (Serial.available()) {
      Exit_Flag = Serial.parseInt();
      if (Exit_Flag == -1) {
        Exit_Flag = 0;
        Serial.println("/*****************************************************/");
        return;
      }
    }
  }
}
/******************************PH_Fun******************************/
void PH_Fun(void) {
  Serial.println("This is PH_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  while (1) {
    PH();
    Serial.print("pHValue:");
    Serial.println(pHValue, 2);
    delay(500);
    if (Serial.available()) {
      Exit_Flag = Serial.parseInt();
      if (Exit_Flag == -1) {
        Exit_Flag = 0;
        Serial.println("/*****************************************************/");
        return;
      }
    }
  }
}

/******************************Tem_Fun******************************/
void Tem_Fun(void) {
  Serial.println("This is Tem_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  while (1) {
    Tem();
    Serial.print("TemValue:");
    Serial.println(tem_Value);
    delay(500);
    if (Serial.available()) {
      Exit_Flag = Serial.parseInt();
      if (Exit_Flag == -1) {
        Exit_Flag = 0;
        Serial.println("/*****************************************************/");
        return;
      }
    }
  }
}
/******************************NTU_Fun******************************/
void NTU_Fun(void) {
  Serial.println("This is NTU_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  while (1) {
    NTU();
    Serial.print("TUValue:");
    Serial.print(TU_value);
    Serial.println("NTU");
    delay(500);
    if (Serial.available()) {
      Exit_Flag = Serial.parseInt();
      if (Exit_Flag == -1) {
        Exit_Flag = 0;
        Serial.println("/*****************************************************/");
        return;
      }
    }
  }
}
/*****************************multiple*******************************/
void Multiple_Debug(void) {
  Serial.println("This is Multiple_Debug");
  Serial.println("Please input -1 to exit");
  Serial.println("/*****************************************************/");
  while (1) {
    for (int i = 0; i < 5; i++) {
      TDS();
      delay(50);
    }
    for (int j = 0; j < 5; j++) {
      PH();
      delay(50);
    }
    Tem();
    delay(300);
    NTU();

    Serial.print("TDSValue:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");

    Serial.print("pHValue:");
    Serial.println(pHValue, 2);

    Serial.print("TemValue:");
    Serial.println(tem_Value);

    Serial.print("TUValue:");
    Serial.print(TU_value);
    Serial.println("NTU");
    Serial.println("/*****************************************************/");

    if (Serial.available()) {
      Exit_Flag = Serial.parseInt();
      if (Exit_Flag == -1) {
        Exit_Flag = 0;
        return;
      }
    }
  }
}
/*****************************TDS*******************************/
void TDS(void) {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U)  //每40毫秒，从A1读取模拟值
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  //读取模拟值并存入缓冲区
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    }
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;                                                                                                   //通过中值滤波算法读取更稳定的模拟值，并转换为电压值
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                                                                                                //温度补偿公式: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                             //温度补偿
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;  //将电压值转换为tds值
  }
}
int getMedianNum(int bArray[], int iFilterLen)  //计算函数
{
  int bTab[iFilterLen], i, j, bTemp;
  for (byte i = 0; i < iFilterLen; i++) {
    bTab[i] = bArray[i];
  }
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
/*****************************PH*******************************/
void PH(void) {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)
      pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    pHValue += 3;
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)  //每800毫秒打印一个数值
  {
    printTime = millis();
  }
}
double avergearray(int *arr, int number)  //计算电压，PH值函数
{
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("平均数组数错误!");
    return 0;
  }
  if (number < 5)  //小于5，直接计算统计
  {
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    } else {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;  // arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  // arr>max
          max = arr[i];
        } else {
          amount += arr[i];  // min<=arr<=max
        }
      }
    }
    avg = (double)amount / (number - 2);
  }
  return avg;
}
/*****************************Tem*******************************/
void Tem(void) {
  TEM.start();
  delay(50);
  tem_Value = TEM.get();
}
/*****************************NTU*******************************/
void NTU(void) {
  int sensorValue = analogRead(A2);         //读取模拟引脚0上的输入
  float TU = sensorValue * (5.0 / 1024.0);  //将模拟读数（从0-1023）转换为电压（0-5V）
  TU_calibration = -0.0192 * (temp_data - 25) + TU;
  TU_value = -865.68 * TU_calibration + K_Value;
  if (TU_value <= 0) {
    TU_value = 0;
  }
  if (TU_value >= 3000) {
    TU_value = 3000;
  }
  TU_value = map(TU_value, 0.00, 3000.00, 0.00, 30.00);
}
/*****************************Return_Data*******************************/
float Return_Data(char Type) {
  switch (Type) {
    case '1':
      return tdsValue;
    case '2':
      return pHValue;
    case '3':
      return tem_Value;
    case '4':
      return TU_value;
    default:
      break;
  }
}
/*****************************Read_Sensor*******************************/
void Read_Sensor(void) {
  for (int i = 0; i < 5; i++) {
    TDS();
    delay(50);
  }
  for (int j = 0; j < 5; j++) {
    PH();
    delay(50);
  }
  Tem();
  delay(300);
  NTU();
}