//�������ƣ����Ϲ�����һ��ģ���ռ���
//�汾��V1.0
//�з���λ��������С�Ƽ����޹�˾���Ϲ����Ƽ�ʵ����
//�������ߣ�������
//�ƶ����ڣ�2021.3.8
//��Ȩ���У���Ȩ�ؾ�

#include "stdio.h"//��sprintfר��

#include "Timer.h"//�ڲ���һ��

#include "bsp_init.h"//�ⲿͦ�����
#include "bsp_key.h"//������֮1 key ���ִ�����
#include "bsp_seg.h"//������֮2 seg �����ܿ���
#include "bsp_led.h"//������֮3 led �㻹�ֺ���ˣ����ۣ��Ҳ�����



//���ݿ���ģ�����ͷ�ļ�

//--�����¶ȴ�����ʱ��
//#include "bsp_onewire.h"

//--����ʱ��ʱר��
//#include "bsp_ds1302.h"

//--����EEPROM/AD/DAʱ��
//#include "bsp_iic.h"




//�����պ���������
void Key_Proc(void);
void Seg_Proc(void);
void Led_Proc(void);

//������
//--��ʱ���δ����
unsigned long ms_Tick = 0;

//--��ʱ�����ٱ���
unsigned int Key_Slow_Down;
unsigned int Seg_Slow_Down;
unsigned int Led_Slow_Down;

//--����ר�ñ���
unsigned char Key_Value;
unsigned char Key_Old, Key_Down;

//--�����ר�ñ���
unsigned char seg_string[10];
unsigned char seg_buf[8];
unsigned char pos;

//--LEDר�ñ���
unsigned char ucLed;




/***�û��Զ����������ʼ��***/

bit state_flag = 0;//����ģ��û���Ҫ�õ�һ������

//--EEPROM����ר�ñ���
//unsigned char eeprom_string[4] = {11,12,13,14};
//unsigned char eeprom_string_null[4];

//--ʱ��ר�ñ���
//unsigned char Rtc[3] = {23,59,55};

/***�û��Զ��������������***/



void main(void)
{

	Cls_Peripheral();//�ر�LED/�̵���/������
	Timer1Init();//��ʱ��1��ʼ��
	EA = 1;//�����ж�

	//--EEPROM����
	//EEPROM_Write(eeprom_string, 0, 4);	
	
	//--ʱ�Ӳ���
	//Set_Rtc(Rtc);
	
	
	while(1)
	{
		Key_Proc();//�������Ӻ���
		Seg_Proc();
		Led_Proc();
	}
}



/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
	ms_Tick++;//�δ�ʱ�����ܼ�¼�����ݳ���29��
	
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���ٱ������������Ӻ�����ˢ��Ƶ��
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;	
	if(++Led_Slow_Down == 100) Led_Slow_Down = 0;	

	
	
  Seg_Disp(seg_buf, pos);//�����������ʾ
	if(++pos == 8) pos = 0;
	
	Led_Disp(ucLed);//����LED��ʾ
	
}









void	Key_Proc(void)
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	//���д���
	Key_Value = Key_Read();
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//�½��أ������ĸ����������ֵ���Ǽ�
	Key_Old = Key_Value;//����ƽ��һֱ����һֱ����һ����
	
	
	/***�û��Զ����������***/	
	switch(Key_Down)//����ṹ�ܳ������б����ĸ���������ȥ�ˣ����ݰ������µ�����������ڲ����ݱ����ı仯
	{
		case 11:
				state_flag ^= 1;//0-����������ܺͶ�ʱ������ʾ12345678�������Եƣ�������1-�¶Ȼ�������ֵ
				break;//���break����Ҫ

	}
	/***�û��Զ����������***/		

	
}





void Seg_Proc(void)
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	/***�û��Զ����������***/	
	if(state_flag == 0)
		sprintf(seg_string,"12345678");			
	else //��Ļ����ʾ�����ݣ�������ط����͵�ͬ��seg_string�з���ʲô���ݣ���ҷ�ʲô���Ҿ͸���ʾʲô������װ�ƣ�ֻ������seg����ʾ�ķ�Χ��
	{
		//--�¶ȴ�����ר��
		//sprintf(seg_string,"%03d",(unsigned int)(rd_temperature()/16.0));	
		
		//--ʱ��ר��
		//Read_Rtc(Rtc);
		//sprintf(seg_string,"%02d-%02d-%02d",(unsigned int)Rtc[0],(unsigned int)Rtc[1],(unsigned int)Rtc[2]);	
		
		//--EEPROM����
		//EEPROM_Read(eeprom_string_null, 0, 4);
		//sprintf(seg_string,"%2d--%2d",(unsigned int)eeprom_string_null[0],(unsigned int)eeprom_string_null[3]);	

		//--AD����
		//sprintf(seg_string,"%03d",(unsigned int)Pcf8591_Adc(Adj_Res_Channel));	
		
		//--DA����
		//Pcf8591_Dac(255);
		
	}
	
	/***�û��Զ����������***/			
	
	Seg_Tran(seg_string, seg_buf);
}






void	Led_Proc(void)
{
	if(Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	
	/***�û��Զ����������***/		
 	if(state_flag == 0)
		ucLed = 0xff;			
	else 
	 	ucLed = 0;	
	/***�û��Զ����������***/			

}



