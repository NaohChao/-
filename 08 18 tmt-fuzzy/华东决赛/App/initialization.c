#include "initialization.h"
#include "common.h"
#include "include.h"

int16 speed_right, speed_left, speed_set_to_left = 70, speed_set_to_right = 70, speed_set = 70, speed, first_slow_speed, second_slow_speed;

extern uint8 imgbuff[CAMERA_R_H][CAMERA_W]; //定义存储接收图像的数组
extern uint8 imgpro[CAMERA_R_H][CAMERA_W];  //二值化图像数组
extern int16 chaosheng_speed_set;
extern int16 leftline[40];
extern int16 rightline[40];
extern uint8 leftline_flag, rightline_flag;

extern int16 kp_speed_70, kp_speed_40, kp_speed_10, kp_speed_5, kp_speed;
extern float Temp_PID_1, Temp_PID_2, Temp_PID_3, Temp_PID_4, Temp_PID_5, Temp_PID_6, Temp_PID_7;
extern float steer_zhi_kp;
extern float steer_zhi_kd;
extern float steer_xiaos_kp;
extern float steer_xiaos_kd;
extern float steer_wan_kp_left;
extern float steer_wan_kd_left;
extern float steer_wan_kp_right;
extern float steer_wan_kd_right;
extern float chasu_rate;

// int distance = 0;       //距离，单位毫米
int avg_distance = 0; //距离平均值
int length[5] = {0};  //加权平均算距离平均值
int distance;
uint8 dat[3];
uint8 num1;
// uint8 data1 = 0;        //发送数据的第一个字节
// uint8 data2 = 0;        //第二个字节
// uint8 data3 = 0;        //第三个字节
// int temp1 = 0;          //用于第二个字节左移8位的中间变量
// int temp2 = 0;          //存储第三个字节的中间变量

void LED_init(void)
{
    gpio_init(PTC17, GPO, 0); // D1
    gpio_init(PTC16, GPO, 0); // D0
    gpio_init(PTC19, GPO, 0); // DC
    gpio_init(PTC18, GPO, 1); // RST
    LCD_Init();
}

void set_priority_irq(void)
{
    set_irq_priority(UART2_RX_TX_IRQn, PIT0_IRQn);
    set_irq_priority(UART2_RX_TX_IRQn, PORTB_IRQn);
    set_irq_priority(UART2_RX_TX_IRQn, PORTC_IRQn);
    set_irq_priority(PIT0_IRQn, PORTB_IRQn);
    set_irq_priority(PIT0_IRQn, PORTC_IRQn);
}

void initialization(void)
{
  FTM_PWM_init(FTM0, FTM_CH1, 10000, 0);      // A4
  FTM_PWM_init(FTM0, FTM_CH2, 10000, 0);      // A5
  FTM_PWM_init(FTM0, FTM_CH3, 10000, 0);      // A6
  FTM_PWM_init(FTM0, FTM_CH4, 10000, 0);      // A7
  FTM_PWM_init(FTM1, FTM_CH0, 50, steer_mid); // A12
  LED_init();
    while (!nrf_init())
        ;
    set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler); //
    enable_irq(PORTA_IRQn);
    /*******************************************
    摄像头相关中断初始化
  *******************************************/
    //  set_irq_priority(PIT0_IRQn, PORTB_IRQn);
    //  set_irq_priority(PIT0_IRQn, PORTC_IRQn);
    set_priority_irq();                               //设置中断优先级
    exti_init(PTC3, rising_up);                       // VS
    exti_init(PTB7, rising_down);                     // HS
    port_init(PTC0, ALT1 | DMA_FALLING | PULLUP);     // PCLK
    set_vector_handler(PORTB_VECTORn, portb_handler); //设置中断复位函数为 PORTB_IRQHandler
    set_vector_handler(PORTC_VECTORn, portc_handler); //设置中断复位函数为 PORTC_IRQHandler
    // DMA通道0初始化，PTA27触发源(默认上升沿)，源地址为PTE_B0_IN，目的地址为：IMG_BUFF，每次传输1Byte
    dma_portx2buff_init(DMA_CH0, (void *)&PTE_B0_IN, (void *)imgbuff, PTC0,
                        DMA_BYTE1, CAMERA_W, DADDR_KEEPON);
    enable_irq(PORTC_IRQn);
    /*******************************************
  摄像头相关中断初始化结束
  *******************************************/
    uart_init(UART4, 115200);

    // set_vector_handler(UART2_RX_TX_VECTORn, uart2_handler); // 设置中断服务函数到中断向量表里
    // uart_rx_irq_en(UART2);                                  //开串口接收中断

    /*******************************************
    测速相关初始化
  *******************************************/
    gpio_init(PTD0, GPI, 0); //速度采集
    gpio_init(PTD1, GPI, 0); //速度采集
    gpio_init(PTD8, GPO, 1); //蜂鸣器初始化  PTA13
    DMA_count_Init(DMA_CH1, PTA10, 0x7FFF, 0xA2u);
    DMA_count_Init(DMA_CH2, PTB18, 0x7FFF, 0xA2u);
    pit_init_ms(PIT0, 5);
    set_vector_handler(PIT0_VECTORn, PIT0_IRQHandler);
    enable_irq(PIT0_IRQn);
    /*******************************************
    测速相关初始化结束
  *******************************************/
    //    lptmr_pulse_init(LPT0_ALT2, 0xFFFF, LPT_Rising); //test

    /*******************************************
    测距相关初始化
    *******************************************/
    uart_init(UART2, 115200);
    set_vector_handler(UART2_RX_TX_VECTORn, uart2_handler);
    enable_irq(UART2_RX_TX_IRQn);
    uart_rx_irq_en(UART2);

    /*******************************************
    按键初始化
    *******************************************/
    gpio_init(PTB1, GPI, 0);
    gpio_init(PTB2, GPI, 0);
    gpio_init(PTB3, GPI, 0);
    gpio_init(PTB4, GPI, 0);
    /*******************************************
    按键初结束
    *******************************************/

    /*******************************************
    PID参数设置temp传递
    *******************************************/
    Temp_PID_1 = steer_wan_kp_left;
    Temp_PID_2 = steer_wan_kd_left;
    Temp_PID_3 = steer_zhi_kp;
    Temp_PID_4 = steer_zhi_kd;
    Temp_PID_5 = steer_xiaos_kp;
    Temp_PID_6 = steer_xiaos_kd;
    Temp_PID_7 = chasu_rate;
}

/*******************************************
  测速控制中断复位函数
*******************************************/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0); //清中断标志位

    speed_left = DMA_count_get(DMA_CH1);
    if (gpio_get(PTD0) == 1)
        speed_left = -speed_left;
    DMA_count_reset(DMA_CH1);

    speed_right = DMA_count_get(DMA_CH2);
    if (gpio_get(PTD1) == 0)
        speed_right = -speed_right;
    DMA_count_reset(DMA_CH2);

    speed_set_left(speed_set_to_left); //speed_set_to_left
    speed_set_right(speed_set_to_right);//speed_set_to_right
//    speed_set_left(30);
//    speed_set_right(30);
}

/*******************************************
  超声中断
*******************************************/
void uart2_handler(void)
{

    // uart_getchar(UART2, &data1); //读取第一个字节
    // if (data1 == 0xa5)           //只要读取到第一个字节为0xa5，开始进入计算数据包
    // {
    //     uart_getchar(UART2, &data2); //读取第二个字节
    //     temp1 = data2;
    //
    //     uart_getchar(UART2, &data3); //读取第三个字节
    //     temp2 = data3;
    //
    //     distance = (temp1 << 8) + temp2; //第二个字节为高八位，需要左移8位再加上第三个字节
    //
    //     length[0] = length[1];
    //     length[1] = length[2];
    //     length[2] = length[3];
    //     length[3] = length[4];
    //     length[4] = distance; //滑动滤波
    //
    //     avg_distance = (length[0] + length[1] + length[2] + length[3] + length[4]) / 5; //均值滤波
    // }

    uart_getchar(UART2, &dat[num1]);
    if (dat[0] != 0xa5)
        num1 = 0; //检查头帧是否正确，不正确就重新接收
    else
        num1++;

    if (num1 == 3) //接收完成，开始处理数据
    {
        num1 = 0;
        distance = dat[1] << 8 | dat[2];

        length[0] = length[1];
        length[1] = length[2];
        length[2] = length[3];
        length[3] = length[4];
        length[4] = distance; //滑动滤波

        avg_distance = (4 * length[0] + 3 * length[1] + 2 * length[2] + length[3] + length[4]) / 11; //均值滤波
    }
}

/*******************************************
  NRF中断复位函数
*******************************************/
void PORTA_IRQHandler()
{
    uint8 n; //引脚号
    uint32 flag;

    flag = PORTA_ISFR;
    PORTA_ISFR = ~0; //清中断标志位

    n = 19;
    if (flag & (1 << n)) //PTE27触发中断
    {
        nrf_handler();
    }
}

/*******************************************
 无线发射及虚拟示波器
*******************************************/
//**************************************************************************
/*
*  功能说明：SCI示波器CRC校验
内部调用函数
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
  unsigned short CRC_Temp;
  unsigned char i,j;
  CRC_Temp = 0xffff;
  
  for (i=0;i<CRC_CNT; i++){
    CRC_Temp ^= Buf[i];
    for (j=0;j<8;j++) {
      if (CRC_Temp & 0x01)
        CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
      else
        CRC_Temp = CRC_Temp >> 1;
    }
  }
  return(CRC_Temp);
}
