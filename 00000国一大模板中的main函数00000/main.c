//代码名称：蚂蚁工厂国一大模板终极版
//版本：V1.0
//研发单位：北京以小科技有限公司蚂蚁工厂科技实验室
//代码作者：杜永博
//制定日期：2021.3.8
//版权所有，侵权必究

#include "stdio.h"//给sprintf专用

#include "Timer.h"//内部就一个

#include "bsp_init.h"//外部挺多余的
#include "bsp_key.h"//三大金刚之1 key 用手触摸的
#include "bsp_seg.h"//三大金刚之2 seg 用眼能看到
#include "bsp_led.h"//三大金刚之3 led 你还弥红灯了，晃眼，我不爱你



//根据考核模块添加头文件

//--考察温度传感器时用
//#include "bsp_onewire.h"

//--考察时钟时专用
//#include "bsp_ds1302.h"

//--考察EEPROM/AD/DA时用
//#include "bsp_iic.h"




//三大金刚函数声明区
void Key_Proc(void);
void Seg_Proc(void);
void Led_Proc(void);

//变量区
//--定时器滴答变量
unsigned long ms_Tick = 0;

//--定时器减速变量
unsigned int Key_Slow_Down;
unsigned int Seg_Slow_Down;
unsigned int Led_Slow_Down;

//--按键专用变量
unsigned char Key_Value;
unsigned char Key_Old, Key_Down;

//--数码管专用变量
unsigned char seg_string[10];
unsigned char seg_buf[8];
unsigned char pos;

//--LED专用变量
unsigned char ucLed;




/***用户自定义变量区开始↓***/

bit state_flag = 0;//测试模板好坏需要用的一个变量

//--EEPROM测试专用变量
//unsigned char eeprom_string[4] = {11,12,13,14};
//unsigned char eeprom_string_null[4];

//--时钟专用变量
//unsigned char Rtc[3] = {23,59,55};

/***用户自定义变量区结束↑***/



void main(void)
{

	Cls_Peripheral();//关闭LED/继电器/蜂鸣器
	Timer1Init();//定时器1初始化
	EA = 1;//打开总中断

	//--EEPROM测试
	//EEPROM_Write(eeprom_string, 0, 4);	
	
	//--时钟测试
	//Set_Rtc(Rtc);
	
	
	while(1)
	{
		Key_Proc();//三大金刚子函数
		Seg_Proc();
		Led_Proc();
	}
}



/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
	ms_Tick++;//滴答定时器，能记录的数据长达29天
	
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//减速变量区，控制子函数的刷新频率
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;	
	if(++Led_Slow_Down == 100) Led_Slow_Down = 0;	

	
	
  Seg_Disp(seg_buf, pos);//用于数码管显示
	if(++pos == 8) pos = 0;
	
	Led_Disp(ucLed);//用于LED显示
	
}









void	Key_Proc(void)
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	//三行代码
	Key_Value = Key_Read();
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//下降沿，按得哪个按键，这个值就是几
	Key_Old = Key_Value;//检测电平，一直按着一直等于一个数
	
	
	/***用户自定义代码区↓***/	
	switch(Key_Down)//这个结构很常见，判别是哪个按键按下去了，根据按键按下的情况，控制内部数据变量的变化
	{
		case 11:
				state_flag ^= 1;//0-（测试数码管和定时器）显示12345678，（测试灯）灯亮；1-温度或者其他值
				break;//这个break很重要

	}
	/***用户自定义代码区↑***/		

	
}





void Seg_Proc(void)
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	/***用户自定义代码区↓***/	
	if(state_flag == 0)
		sprintf(seg_string,"12345678");			
	else //屏幕上显示的内容，在这个地方，就等同于seg_string中放了什么内容，你敢放什么，我就敢显示什么，不敢装逼，只限制于seg能显示的范围。
	{
		//--温度传感器专用
		//sprintf(seg_string,"%03d",(unsigned int)(rd_temperature()/16.0));	
		
		//--时钟专用
		//Read_Rtc(Rtc);
		//sprintf(seg_string,"%02d-%02d-%02d",(unsigned int)Rtc[0],(unsigned int)Rtc[1],(unsigned int)Rtc[2]);	
		
		//--EEPROM测试
		//EEPROM_Read(eeprom_string_null, 0, 4);
		//sprintf(seg_string,"%2d--%2d",(unsigned int)eeprom_string_null[0],(unsigned int)eeprom_string_null[3]);	

		//--AD测试
		//sprintf(seg_string,"%03d",(unsigned int)Pcf8591_Adc(Adj_Res_Channel));	
		
		//--DA测试
		//Pcf8591_Dac(255);
		
	}
	
	/***用户自定义代码区↑***/			
	
	Seg_Tran(seg_string, seg_buf);
}






void	Led_Proc(void)
{
	if(Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	
	/***用户自定义代码区↓***/		
 	if(state_flag == 0)
		ucLed = 0xff;			
	else 
	 	ucLed = 0;	
	/***用户自定义代码区↑***/			

}



