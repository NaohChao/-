/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,野火科技
*     All rights reserved.
*     技术讨论：野火初学论坛 http://www.chuxue123.com
*
*     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留野火科技的版权声明。
*
* @file       main.c
* @brief      野火K60 平台主程序
* @author     野火科技
* @version    v5.0
* @date       2013-08-28
*/

#include "common.h"
#include "include.h"

extern uint8 imgbuff[CAMERA_R_H][CAMERA_W];                             //定义存储接收图像的数组
extern int16 speed_right,speed_left,speed_set,speed,first_slow_speed,first_slow_speed,second_slow_speed,chaoche_start,speed_set_to_left,speed_set_to_right;
extern int speedset_st, speedset_co;
extern bool stop_flag;
extern int init_distance;
extern int distance;
extern int init_speed_right;
extern int init_speed_left;
extern uint8 chaoche_ready_flag,former_flag;

extern int fache_flag;
extern int stop_distance;
extern bool zhongdianjiasu_flag;
extern uint8 send_num;
extern uint16 avg_distance;
extern int Distance;
extern bool fache_finish_flag;
extern bool chusaidao_flag,podao_flag;
int16 rightline[40];
void speed_setting();
//extern void SEND(float a,float b,float c,float d);
//extern void OutPut_Data(void);
void  main(void)
{
  
  DisableInterrupts;
  
  initialization();
  FTM_PWM_Duty(FTM1, FTM_CH0, steer_mid);
  for(int i=39;i>=0;i--)
  {
    rightline[i]=CAMERA_W-1;
  }
  setting();
  systick_delay_ms(8000);
  EnableInterrupts;   //数据采集完成
  
  
  speed_setting();  //速度设置
  while(1)
  {
    camera(); //图像处理
    distance_control(Distance); //控距
    chasu();
    //    LCD_Show_Number(50, 5, avg_distance);
    //    LCD_Show_Number(50, 6, speed_set);
    //    LCD_Show_Number(50, 7, fache_distance);
  }
}



void speed_setting()
{
  speed=speedset_st;;
  first_slow_speed=70;
  second_slow_speed=70;
}