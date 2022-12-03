#ifndef _SENSOR_H_
#define _SENSOR_H_

void TDS(void);
void PH(void);
void Tem(void);
void NTU(void);
void Choose_Sensor(char Choose_Sensor_Order);
void multiple_debug(void);
void TDS_Fun(void);
void PH_Fun(void);
void Tem_Fun(void);
void NTU_Fun(void);
float Return_Data(char Type);
void Read_Sensor(void);

#endif
