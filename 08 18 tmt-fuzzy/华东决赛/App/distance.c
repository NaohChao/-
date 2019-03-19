#include "distance.h"
#include "common.h"
#include "include.h"

int error_distance[4];
extern uint16 avg_distance;
int init_distance;
extern int16 speed_right, speed_left, speed_set_to_left, speed_set_to_right, speed_set,speed;
int16 chaosheng_speed_set;
extern uint8 chuhuandao_flag,ruhuandao_flag;
extern uint8 former_flag,chaoche_ready_flag,jieshouchaoche_flag;
extern uint8 chaoche_stop_flag;
extern bool cruise_mode_on;
//int diushi_count,diushi_time;
bool gg_flag=0;
extern uint8 send_num;
extern bool qipaoxian_flag,fache_finish_flag;
void distance_control(int distance_set)
{
if(former_flag==1||(former_flag==0&&(fache_finish_flag==0||qipaoxian_flag==1||ruhuandao_flag==1||chuhuandao_flag!=1)))
  {
    speed_set=speed;
  }
  else
  {
    if (avg_distance > 2500&&gg_flag==0)//||cruise_mode_on==1) //|| avg_distance < 100)//else if
    {
      speed_set=speed+3;
//      diushi_count++;
//      if(diushi_count>500)
//      {
//        diushi_time=diushi_time+1;
//        diushi_count=0;
//      }
//      if(diushi_time>200)
//      {
//        gg_flag=1;
//        send_num=10;
//        speed=0;
//        speed_set=0;
//      }
      // cruise_mode_on=1;
    }
    else 
    {
      //if(gg_flag==0)
      //{
//        diushi_count=0;
//        diushi_time=0;
        error_distance[2] = error_distance[1];
        error_distance[1] = error_distance[0];
        error_distance[0] = avg_distance - distance_set;
//        if (error_distance[0] >= 800)             //     1600<avg   
//        {
//          init_distance = 15;
//        }      
        if (error_distance[0] >= 250)         //     1050<avg<1250
        {
          init_distance = error_distance[0] * 0.02 + 0.01 * (error_distance[0] - error_distance[1]);//0.025               // 6.25<init_distance<11.25
          if (init_distance > 10)
          {
            init_distance = 10;
          }
        }
        else if (error_distance[0] >= 100)          //     900<avg<1050           ????????
        {
          init_distance = error_distance[0] * 0.015 + 0.01 * (error_distance[0] - error_distance[1]); //0.02              // 2<init_distance<5
          if (init_distance > 4)//5
          {
            init_distance = 4;
          }
        }
        else if (error_distance[0] >= 0)           //     800<avg<900          
        {
          init_distance = error_distance[0] * 0.015 + 0.01 * (error_distance[0] - error_distance[1]);               // 0<init_distance<2
          if (init_distance > 2)  //3
          {
            init_distance = 2;
          }
        }
        else if (error_distance[0] >= -100)           //     700<avg<800
        {
          init_distance = error_distance[0] * 0.015 - 0.01 * (error_distance[0] - error_distance[1]);
          if (init_distance <- 4)   //-3
          {
            init_distance = -4;
          }
        }
        else if (error_distance[0] >= -250)           //     550<avg<700           ????????
        {
          init_distance = error_distance[0] * 0.02 - 0.01 * (error_distance[0] - error_distance[1]); //0.025               
          if (init_distance <- 10)  //-12
          {
            init_distance = -10;
          }
        }
        else if (error_distance[0] >= -450)           //     350<avg<550
        {
          init_distance = error_distance[0] * 0.03 - 0.02 * (error_distance[0] - error_distance[1]);                   
          if (init_distance <- 15)
          {
            init_distance = -15;
          }
        }
        else if (error_distance[0] >= -800)           //     0<avg<350
        {
          init_distance = -25;//-25
        }
        speed_set = speed + init_distance;
        if (speed_set < 0)
        {
          speed_set = 0;
        }
        if (speed_set > 120)
        {
          speed_set = 120;
        }   
      //}
    }    
  }
  //SEND(avg_distance*0.1,diushi_time*10,init_distance+75,75);
}