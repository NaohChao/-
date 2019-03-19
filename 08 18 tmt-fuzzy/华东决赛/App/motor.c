#include "motor.h"
#include "common.h"
#include "include.h"

int16 kp_speed_70 = 200; //  120
int16 kp_speed_40 = 150;
int16 kp_speed_10 = 140;
int16 kp_speed_5 = 140;
int16 kp_speed = 140;

int16 ki_speed_70 = 80; //  2 40
int16 ki_speed_40 = 40; ///6    40
int16 ki_speed_10 = 15; // 10   60 
int16 ki_speed_5 = 15;//15      80
int16 ki_speed = 15;//15       100

int16 kd_speed = 30; //10

int16 kp_speed_right = 200;
int16 kp_speed_left = 200;

int8 ki_speed_right = 50;
int8 ki_speed_left = 50;

int8 kd_speed_right;
int8 kd_speed_left;

int error_speed_right[3] = {0};
int error_speed_left[3] = {0};

long init_speed_right = 0;
long init_speed_left = 0;

extern int16 speed_right, speed_left,speed_set,speed;
extern int16 chaosheng_speed_set;
/*******************************************
右轮速度PID调节
*******************************************/

void speed_set_right(int speed_set_to_right)
{
    int adjust_speed_right;

    error_speed_right[2] = error_speed_right[1];
    error_speed_right[1] = error_speed_right[0];
    error_speed_right[0] = speed_set_to_right - speed_right;

    if (speed_set!=0)
    {
      if (abs(error_speed_right[0]) >= 70)
      {
        kp_speed_right = kp_speed_70; //150
        ki_speed_right = ki_speed_70;           //2
      }
      if (abs(error_speed_right[0]) > 40)
      {
        kp_speed_right = kp_speed_40; //130
        ki_speed_right = ki_speed_40;           //15
      }
      else if (abs(error_speed_right[0]) > 10)
      {
        kp_speed_right = kp_speed_10; //120
        ki_speed_right = ki_speed_10;          //20
      }
      else if (abs(error_speed_right[0]) > 5)
      {
        kp_speed_right = kp_speed_5; //110
        ki_speed_right = ki_speed_5;         //25
      }
      else
      {
        kp_speed_right = kp_speed;
        ki_speed_right = ki_speed;
      }
    }
    else 
    {
      if (abs(error_speed_right[0]) >= 70)
      {
        kp_speed_right = 140; //150
        ki_speed_right = 40;           //90  50
      }
      if (abs(error_speed_right[0]) > 40)
      {
        kp_speed_right = 130; //130
        ki_speed_right = 30;           //100
      }
      else if (abs(error_speed_right[0]) > 10)
      {
        kp_speed_right = 120; //120
        ki_speed_right = 20;          //110
      }
      else if (abs(error_speed_right[0]) > 5)
      {
        kp_speed_right = 100; //110
        ki_speed_right = 20;         //120
      }
      else
      {
        kp_speed_right = 100;
        ki_speed_right = 20;        // 100
      }
    }

    adjust_speed_right = kp_speed_right * (error_speed_right[0] - error_speed_right[1]) + (int)(ki_speed_right * error_speed_right[0]) + kd_speed_right * (error_speed_right[0] - 2 * error_speed_right[1] + error_speed_right[2]);
    init_speed_right = init_speed_right + adjust_speed_right;

    if (init_speed_right > 9500) //limit the max PWM for motor
    {
        init_speed_right = 9500;
    }
    else if (init_speed_right < -9500) //limit the min PWM for motor
    {
        init_speed_right = -9500;
    }

    if (init_speed_right < 0)
    {
        //  ftm_pwm_duty(FTM0, FTM_CH1,-init_speed_right);
        //ftm_pwm_duty(FTM0, FTM_CH2 ,0);
        FTM_PWM_Duty(FTM0, FTM_CH3, -init_speed_right);
        FTM_PWM_Duty(FTM0, FTM_CH4, 0);
    }
    else
    {
        FTM_PWM_Duty(FTM0, FTM_CH3, 0);
        FTM_PWM_Duty(FTM0, FTM_CH4, init_speed_right); //init_speed_right
                                                       // ftm_pwm_duty(FTM0, FTM_CH1, 0);
                                                       // ftm_pwm_duty(FTM0, FTM_CH2, init_speed_right);
    }
}

/*******************************************
左轮速度PID调节
*******************************************/

void speed_set_left(int speed_set_to_left)
{
    int adjust_speed_left;

    error_speed_left[2] = error_speed_left[1];
    error_speed_left[1] = error_speed_left[0];
    error_speed_left[0] = speed_set_to_left - speed_left;

   if (speed_set!=0)
    {
      if (abs(error_speed_right[0]) >= 70)
      {
        kp_speed_left = kp_speed_70; //150
        ki_speed_left = ki_speed_70;           //2
      }
      if (abs(error_speed_right[0]) > 40)
      {
        kp_speed_left = kp_speed_40; //130
        ki_speed_left = ki_speed_40;           //15
      }
      else if (abs(error_speed_right[0]) > 10)
      {
        kp_speed_left = kp_speed_10; //120
        ki_speed_left = ki_speed_10;          //20
      }
      else if (abs(error_speed_right[0]) > 5)
      {
        kp_speed_left = kp_speed_5; //110
        ki_speed_left = ki_speed_5;         //25
      }
      else
      {
        kp_speed_left = kp_speed;
        ki_speed_left = ki_speed;
      }
    }
    else 
    {
      if (abs(error_speed_right[0]) >= 70)
      {
        kp_speed_left = 140; //150
        ki_speed_left = 40;           //90
      }
      if (abs(error_speed_right[0]) > 40)
      {
        kp_speed_left = 130; //130
        ki_speed_left = 30;           //100
      }
      else if (abs(error_speed_right[0]) > 10)
      {
        kp_speed_left = 120; //120
        ki_speed_left = 20;          //110
      }
      else if (abs(error_speed_right[0]) > 5)
      {
        kp_speed_left = 100; //110
        ki_speed_left = 20;         //120
      }
      else
      {
        kp_speed_left = 100;
        ki_speed_left = 20;        // 100
      }
    }
    adjust_speed_left = kp_speed_left * (error_speed_left[0] - error_speed_left[1]) + (int)(ki_speed_left * error_speed_left[0]) + kd_speed_left * (error_speed_left[0] - 2 * error_speed_left[1] + error_speed_left[2]);
    init_speed_left = init_speed_left + adjust_speed_left;
    if (init_speed_left > 9500) //limit the max PWM for motor
    {
        init_speed_left = 9500;
    }
    else if (init_speed_left < -9500) //limit the min PWM for motor
    {
        init_speed_left = -9500;
    }

    if (init_speed_left < 0)
    {
        //        FTM_PWM_Duty(FTM0, FTM_CH4, -init_speed_left);
        //        FTM_PWM_Duty(FTM0, FTM_CH3, 0);
        FTM_PWM_Duty(FTM0, FTM_CH2, -init_speed_left);
        FTM_PWM_Duty(FTM0, FTM_CH1, 0);
    }
    else
    {
        //        FTM_PWM_Duty(FTM0, FTM_CH3, init_speed_left);
        //        FTM_PWM_Duty(FTM0, FTM_CH4, 0);
        FTM_PWM_Duty(FTM0, FTM_CH2, 0);
        FTM_PWM_Duty(FTM0, FTM_CH1, init_speed_left);
    }
}
