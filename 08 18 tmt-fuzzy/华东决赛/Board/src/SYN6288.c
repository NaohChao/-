/***************************飞音云电子****************************
**  工程名称：YS-SYN6288语音合成配套程序
**	CPU: STC89LE52
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-SYN6288语音合成模块
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.8.25
**  说明：。。
/***************************飞音云电子******************************/

//#include "config.h"
#include "common.h"
#include "SYN6288.h"
#include "string.h"
#include "MK60_uart.h"



/**************芯片设置命令*********************/
uint8 SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//停止合成
uint8 SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//暂停合成
uint8 SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//恢复合成
uint8 SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//状态查询
uint8 SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//进入POWER DOWN 状态命令

/***********************************************/



/***********************************************************
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
            *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_FrameInfo(uint8 Music,char *HZdata)
{
/****************需要发送的文本**********************************/ 
    unsigned  char  Frame_Info[50];
    unsigned  char  HZ_Length;  
    unsigned  char  ecc  = 0;  			//定义校验字节
    unsigned  int i=0;
	int count=0;
    HZ_Length =strlen(HZdata); 			//需要发送文本的长度
 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //构造命令参数：背景音乐设定
		 

/*******************校验码计算***************************************/		 
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {   
	         ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
             uart_putchar (UART5,Frame_Info[i]);
			 //while(count==0);
			 //count=0;
			 
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	     {   
	         ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验
	         uart_putchar (UART5,HZdata[i]);
			 //while(count==0);
			// count=0;
	     }
		 
		     
                 uart_putchar (UART5,ecc);
			 //while(count==0);
			 //count=0;
			 
		 
/*******************发送帧信息***************************************/		  
		  //memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  //Frame_Info[5+HZ_Length]=ecc;
		  //PrintCom(Frame_Info,5+HZ_Length+1);
}


/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
/***void YS_SYN_Set(char *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen(Info_data);
	PrintCom(Info_data,Com_Len);
}***/




