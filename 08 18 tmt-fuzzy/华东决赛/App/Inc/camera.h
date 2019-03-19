#ifndef __CAMERA_H__
#define __CAMERA_H__

#define CAMERA_W            200              //定义摄像头图像宽度
#define CAMERA_H            240              //定义摄像头图像高度
#define CAMERA_R_H          40               //定义摄像头图像高度
#define CAMERA_SIZE         CAMERA_W*CAMERA_H
#define BLACK_C 0
#define WHITE_C 254
#define  ThreadHold  150                  //二值化阈值
#define   steer_mid  382     //387
#define  steer_left_max  476
#define  steer_right_max 299
/**********跟踪算法相应参数设置***********/
#define Interval 7
#define Jump_threhold  50                  //75
#define Tracking_displacement  Tracking_NUM/2
#define Tracking_NUM 40
#define Last_line CAMERA_R_H-1

void portb_handler();
void portc_handler();
void camera(void);
void LP_image(void); 
void zhangai_check(void);
void chasu(void);
void chaoche_check(void);
void nrf_process(void);
void shizi_chaoche_check(void);

#endif


/*speed=70

*/
/*speed=75

*/
/*speed=80

*/