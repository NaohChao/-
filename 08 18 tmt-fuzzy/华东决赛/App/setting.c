#include "setting.h"
#include "common.h"
#include "include.h"

int speedset_st = 75;
int speedset_co = 85;

bool key1 = 0, key2 = 0, key3 = 0, key4 = 0;
bool lastkey1 = 0, lastkey2 = 0, lastkey3 = 0, lastkey4 = 0;
bool key1_temp = 0, key2_temp = 0, key3_temp = 0, key4_temp = 0;
bool key1_temp_last = 0, key2_temp_last = 0, key3_temp_last = 0,
     key4_temp_last = 0;
bool send_img_on = 0;
bool cruise_mode_on = 1;
int fache_flag = 0; //当为1时是起跑超车模式  当为0时为正常前后发车模式
int zhidao_chaoche_num = 0;

int term = 1, term_speed = 1, roundabout_num = 1, term_pd = 1;
int huandao_set[8] = {0, 1, 1, 1, 1, 1, 1, 1}; // 0右边，1左边
int huandao_chaoche_set[8] = {0, 1, 1, 1, 1, 0, 1, 1}; //0不超，1超
int huandao_size_set[8] = {0, 1, 2, 3, 1, 2, 3, 1}; // 1小环，2中环，大环
float Temp_PID_1, Temp_PID_2, Temp_PID_3, Temp_PID_4, Temp_PID_5, Temp_PID_6,
    Temp_PID_7;
extern int16 kp_speed_70, kp_speed_40, kp_speed_10, kp_speed_5, kp_speed;
extern float steer_zhi_kp;
extern float steer_zhi_kd;
extern float steer_xiaos_kp;
extern float steer_xiaos_kd;
extern float steer_wan_kp_left;
extern float steer_wan_kd_left;
extern float steer_wan_kp_right;
extern float steer_wan_kd_right;
extern float chasu_rate;

extern int speedset;

void setting() {

  while (1) {
    LCD_P6x8Str(55, 0, "menu");
    LCD_P6x8Str(20, 1, "Speed");
    LCD_P6x8Str(20, 2, "PID");
    LCD_P6x8Str(20, 3, "Turn");
    LCD_P6x8Str(5, 4, "start_chao");
    LCD_P6x8Str(1, 5, "cruise_mode");
    LCD_P6x8Str(1, 6, "ZD_Chao_num");
    LCD_P6x8Str(10, 7, "Send_img");

    key_scan();
    LCD_P6x8Str(70, term, "*");
    if (key2 == 0 && lastkey2 == 1) {

      LCD_P6x8Str(70, term, " ");
      term++;
      LCD_P6x8Str(70, term, "*");
      if (term == 8) {
        LCD_P6x8Str(70, 7, " ");
        term = 1;
      }
    }

    if (key1 == 0 && lastkey1 == 1) {
      LCD_P6x8Str(70, term, " ");
      term--;
      if (term == 0) {
        LCD_P6x8Str(70, 1, " ");
        term = 7;
      }
      LCD_P6x8Str(70, term, "*");
    }

    // LCD_Show_Number(50, 7, term);
    /*******************************************

                                速度设置

        *******************************************/
    if (term == 1 && key3 == 0 && lastkey3 == 1) {
      OLED_Refresh_Gram();
      LCD_P6x8Str(50, 0, "Speed");
      LCD_P6x8Str(10, 1, "Straight");
      LCD_P6x8Str(10, 2, "Corner");
      key_save();

      while (1) {
        key_scan();
        LCD_Show_Number(95, 1, speedset_st);
        LCD_Show_Number(95, 2, speedset_co);
        LCD_P6x8Str(65, term_speed, "*");
        if (key2 == 0 && lastkey2 == 1) {
          LCD_P6x8Str(65, term_speed, " ");
          term_speed++;
          LCD_P6x8Str(65, term_speed, "*");
          if (term_speed == 3) {
            LCD_P6x8Str(65, 3, " ");
            term_speed = 1;
          }
        }

        if (key1 == 0 && lastkey1 == 1) {
          LCD_P6x8Str(65, term_speed, " ");
          term_speed--;
          if (term_speed == 0) {
            LCD_P6x8Str(65, 1, " ");
            term_speed = 2;
          } else {
            LCD_P6x8Str(65, term_speed, "*");
          }
        }
        /*******************************************
                   直道速度设置
             *******************************************/
        if (term_speed == 1 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(65, term_speed, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              speedset_st = speedset_st + 5;
            }

            if (key2 == 0 && lastkey2 == 1) {

              speedset_st = speedset_st - 5;
            }
            LCD_Show_Number(95, 1, speedset_st);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(65, term_speed, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        /*******************************************
                 弯道速度设置
            *******************************************/
        if (term_speed == 2 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(65, term_speed, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              speedset_co = speedset_co + 5;
            }

            if (key2 == 0 && lastkey2 == 1) {

              speedset_co = speedset_co - 5;
            }
            LCD_Show_Number(95, 2, speedset_co);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(65, term_speed, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }

        if (key4 == 0 && lastkey4 == 1) // exit check
        {
          OLED_Refresh_Gram();
          key_save();
          break;
        }
        key_save();
      }
    }

    /*******************************************

                                pid参数设置

         *******************************************/
    if (term == 2 && key3 == 0 && lastkey3 == 1) {
      OLED_Refresh_Gram();
      //舵机参数显示
      LCD_P6x8Str(50, 0, "PID");
      LCD_P6x8Str(1, 1, "wan_kp");
      LCD_P6x8Str(1, 2, "wan_kd");
      LCD_P6x8Str(1, 3, "zhi_kp");
      LCD_P6x8Str(1, 4, "zhi_kd");
      LCD_P6x8Str(1, 5, "xiaos_kp");
      LCD_P6x8Str(1, 6, "xiaos_kd");
      LCD_P6x8Str(1, 7, "chasu_rate");

      //            LCD_P6x8Str(50, 0, "PID");
      //            LCD_P6x8Str(1, 1, "kp_speed_70");
      //            LCD_P6x8Str(1, 2, "kp_speed_40");
      //            LCD_P6x8Str(1, 3, "kp_speed_10");
      //            LCD_P6x8Str(1, 4, "kp_speed_5");
      //            LCD_P6x8Str(1, 5, "kp_speed");
      //            LCD_P6x8Str(1, 6, " ");
      //            LCD_P6x8Str(1, 7, " ");

      key_save();
      //舵机参数显示
      LCD_Show_Number(90, 1, (Temp_PID_1 * Precision_Display));
      LCD_Show_Number(90, 2, (Temp_PID_2 * Precision_Display));
      LCD_Show_Number(90, 3, (Temp_PID_3 * Precision_Display));
      LCD_Show_Number(90, 4, (Temp_PID_4 * Precision_Display));
      LCD_Show_Number(90, 5, (Temp_PID_5 * Precision_Display));
      LCD_Show_Number(90, 6, (Temp_PID_6 * Precision_Display));
      LCD_Show_Number(90, 7, (Temp_PID_7 * Precision_Display));

      while (1) {
        key_scan();

        /*******************************************
             光标控制
        *******************************************/
        LCD_P6x8Str(70, term_pd, "*");
        if (key2 == 0 && lastkey2 == 1) {

          LCD_P6x8Str(70, term_pd, " ");
          term_pd++;
          LCD_P6x8Str(70, term_pd, "*");
          if (term_pd == 8) {
            LCD_P6x8Str(70, 7, " ");
            term_pd = 1;
          }
        }

        if (key1 == 0 && lastkey1 == 1) {
          LCD_P6x8Str(70, term_pd, " ");
          term_pd--;
          if (term_pd == 0) {
            LCD_P6x8Str(70, 1, " ");
            term_pd = 7;
          }
          LCD_P6x8Str(70, term_pd, "*");
        }
        /*******************************************
             pd设置
        *******************************************/
        // P1
        if (term_pd == 1 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_1 = Temp_PID_1 + Precision_P;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_1 = Temp_PID_1 - Precision_P;
            }
            LCD_Show_Number(90, 1, Temp_PID_1 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // D1
        if (term_pd == 2 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_2 = Temp_PID_2 + Precision_D;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_2 = Temp_PID_2 - Precision_D;
            }
            LCD_Show_Number(90, 2, Temp_PID_2 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // P2
        if (term_pd == 3 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_3 = Temp_PID_3 + Precision_P;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_3 = Temp_PID_3 - Precision_P;
            }
            LCD_Show_Number(90, 3, Temp_PID_3 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // D2
        if (term_pd == 4 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_4 = Temp_PID_4 + Precision_D;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_4 = Temp_PID_4 - Precision_D;
            }
            LCD_Show_Number(90, 4, Temp_PID_4 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // P3
        if (term_pd == 5 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_5 = Temp_PID_5 + Precision_P;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_5 = Temp_PID_5 - Precision_P;
            }
            LCD_Show_Number(90, 5, Temp_PID_5 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // D3
        if (term_pd == 6 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_6 = Temp_PID_6 + Precision_D;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_6 = Temp_PID_6 - Precision_D;
            }
            LCD_Show_Number(90, 6, Temp_PID_6 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }
        // I
        if (term_pd == 7 && key3 == 0 && lastkey3 == 1) {
          LCD_P6x8Str(70, term_pd, "->");
          key_save();
          while (1) {
            key_scan();
            if (key1 == 0 && lastkey1 == 1) {

              Temp_PID_7 = Temp_PID_7 + Precision_I;
            }

            if (key2 == 0 && lastkey2 == 1) {

              Temp_PID_7 = Temp_PID_7 - Precision_I;
            }
            LCD_Show_Number(90, 7, Temp_PID_7 * Precision_Display);
            if (key4 == 0 && lastkey4 == 1) {
              LCD_P6x8Str(70, term_pd, "  ");
              key_save();
              break;
            }
            key_save();
          }
        }

        if (key4 == 0 && lastkey4 == 1) // exit check
        {
          OLED_Refresh_Gram();
          key_save();

          //                    kp_speed_70 = Temp_PID_1;
          //                    kp_speed_40 = Temp_PID_2;
          //                    kp_speed_10 = Temp_PID_3;
          //                    kp_speed_5 = Temp_PID_4;
          //                    kp_speed = Temp_PID_5;

          steer_wan_kp_left = Temp_PID_1;
          steer_wan_kp_right = Temp_PID_1;
          steer_wan_kd_left = Temp_PID_2;
          steer_wan_kd_right = Temp_PID_2;
          steer_zhi_kp = Temp_PID_3;
          steer_zhi_kd = Temp_PID_4;
          steer_zhi_kp = Temp_PID_5;
          steer_zhi_kp = Temp_PID_6;
          chasu_rate = Temp_PID_7;

          break;
        }
        key_save();
      }
    }

    /*******************************************

                                环岛设置

         *******************************************/
    if (term == 3 && key3 == 0 && lastkey3 == 1) {
      OLED_Refresh_Gram();
      LCD_P6x8Str(50, 0, "Turn");
      LCD_P6x8Str(30, 1, "1");
      LCD_P6x8Str(30, 2, "2");
      LCD_P6x8Str(30, 3, "3");
      LCD_P6x8Str(30, 4, "4");
      LCD_P6x8Str(30, 5, "5");
      LCD_P6x8Str(30, 6, "6");
      LCD_P6x8Str(30, 7, "7");

      key_save();

      while (1) {
        key_scan();
        /*******************************************
                   环岛设置显示
        *******************************************/
        for (int i = 1; i <= 7; i++) {
          if (huandao_set[i] == 1) {
            LCD_P6x8Str(85, i, "<-");
          } else if (huandao_set[i] == 0) {
            LCD_P6x8Str(85, i, "->");
          }
        }
        /*******************************************
             光标控制
        *******************************************/
        LCD_P6x8Str(60, roundabout_num, "*");
        if (key2 == 0 && lastkey2 == 1) {

          LCD_P6x8Str(60, roundabout_num, " ");
          roundabout_num++;
          LCD_P6x8Str(60, roundabout_num, "*");
          if (roundabout_num == 8) {
            LCD_P6x8Str(60, 7, " ");
            roundabout_num = 1;
          }
        }

        if (key1 == 0 && lastkey1 == 1) {
          LCD_P6x8Str(60, roundabout_num, " ");
          roundabout_num--;
          if (roundabout_num == 0) {
            LCD_P6x8Str(60, 1, " ");
            roundabout_num = 7;
          }
          LCD_P6x8Str(60, roundabout_num, "*");
        }
        /*******************************************
                   转向设置
        *******************************************/
        if (key3 == 0 && lastkey3 == 1) {
          switch (huandao_set[roundabout_num]) {
          case 0:
            huandao_set[roundabout_num] = 1;
            break;
          case 1:
            huandao_set[roundabout_num] = 0;
            break;
          }
        }

        if (key4 == 0 && lastkey4 == 1) // exit check
        {
          OLED_Refresh_Gram();
          key_save();
          break;
        }
        key_save();
      }
    }
    /*******************************************

                            直道超车数量设置

        *******************************************/
    LCD_Show_Number(83, 6, zhidao_chaoche_num);
    if (term == 6 && key3 == 0 && lastkey3 == 1) {
      LCD_P6x8Str(70, 6, "->");
      key_save();
      while (1) {
        key_scan();
        if (key1 == 0 && lastkey1 == 1) {

          zhidao_chaoche_num = zhidao_chaoche_num + 1;
          if (zhidao_chaoche_num >= 5)
            zhidao_chaoche_num = 5;
        }

        if (key2 == 0 && lastkey2 == 1) {
          zhidao_chaoche_num = zhidao_chaoche_num - 1;
          if (zhidao_chaoche_num <= 0)
            zhidao_chaoche_num = 0;
        }
        LCD_Show_Number(83, 6, zhidao_chaoche_num);
        if (key4 == 0 && lastkey4 == 1) {
          LCD_P6x8Str(70, term, "* ");
          key_save();
          break;
        }
        key_save();
      }
    }
    /*******************************************

                            图像发送开关

        *******************************************/
    if (send_img_on == 0) {
      LCD_P6x8Str(90, 7, "off");
    } else if (send_img_on == 1) {
      LCD_P6x8Str(90, 7, "on ");
    }
    if (term == 7 && key3 == 0 && lastkey3 == 1) {
      if (key3 == 0 && lastkey3 == 1) {
        switch (send_img_on) {
        case 0:
          send_img_on = 1;
          break;
        case 1:
          send_img_on = 0;
          break;
        }
      }
    }

    /*******************************************

                            发车模式切换开关

        *******************************************/
    if (fache_flag == 0) {
      LCD_P6x8Str(90, 4, "off ");
    } else if (fache_flag == 1) {
      LCD_P6x8Str(90, 4, "chao");
    }
    if (term == 4 && key3 == 0 && lastkey3 == 1) {
      if (key3 == 0 && lastkey3 == 1) {
        switch (fache_flag) {
        case 0:
          fache_flag = 1;
          break;
        case 1:
          fache_flag = 0;
          break;
        }
      }
    }

    if (cruise_mode_on == 0) {
      LCD_P6x8Str(90, 5, "off");
    } else if (cruise_mode_on == 1) {
      LCD_P6x8Str(90, 5, "on ");
    }
    if (term == 5 && key3 == 0 && lastkey3 == 1) {
      if (key3 == 0 && lastkey3 == 1) {
        switch (cruise_mode_on) {
        case 0:
          cruise_mode_on = 1;
          break;
        case 1:
          cruise_mode_on = 0;
          break;
        }
      }
    }

    if (key4 == 0 && lastkey4 == 1) {
      OLED_Refresh_Gram();

      break;
    }

    key_save();
  }
}

void key_scan(void) {

  //    key1 = !(gpio_get(PTB1));
  //    key2 = !(gpio_get(PTB3));
  //    key3 = !(gpio_get(PTB5));
  //    key4 = !(gpio_get(PTB7));

  key1 = !(gpio_get(PTB1));
  key2 = !(gpio_get(PTB2));
  key3 = !(gpio_get(PTB3));
  key4 = !(gpio_get(PTB4));

  /*
      消除抖动
      */
  //    key1_temp = !(gpio_get(PTB1));
  //    key2_temp = !(gpio_get(PTB3));
  //    key3_temp = !(gpio_get(PTB5));
  //    key4_temp = !(gpio_get(PTB7));
  //
  //    if (key1_temp == 0)
  //    {
  //        key1 = 0;
  //    }
  //    if (key2_temp == 0)
  //    {
  //        key2 = 0;
  //    }
  //    if (key3_temp == 0)
  //    {
  //        key3 = 0;
  //    }
  //    if (key4_temp == 0)
  //    {
  //        key4 = 0;
  //    }
  //
  //    if (key1_temp == 1 && key1_temp_last == 1)
  //    {
  //        key1 = 1;
  //    }
  //    if (key2_temp == 1 && key2_temp_last == 1)
  //    {
  //        key2 = 1;
  //    }
  //    if (key3_temp == 1 && key3_temp_last == 1)
  //    {
  //        key3 = 1;
  //    }
  //    if (key4_temp == 1 && key4_temp_last == 1)
  //    {
  //        key4 = 1;
  //    }
  //
  //    key1_temp_last = key1_temp;
  //    key2_temp_last = key2_temp;
  //    key3_temp_last = key3_temp;
  //    key4_temp_last = key4_temp;
}

void key_save(void) {
  lastkey1 = key1;
  lastkey2 = key2;
  lastkey3 = key3;
  lastkey4 = key4;
}
