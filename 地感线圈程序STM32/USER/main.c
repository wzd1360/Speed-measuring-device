#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"
#include "PI.h"
#include "PO.h"
#include "pwm.h"
#include "timer.h"
//ALIENTEK 探索者STM32F407开发板 实验21
//DAC实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


	u16  sum1=0;           //第一路信号电压输出值
	u16  sum2=0;
int main(void)
{ 

	
	u16  dacval=0;
	u8   E1_count=0;
	u8   t;
	u8   len=0;             //串口接收的字节长度
	//****串口参数获取****//
	u8   command=0;       	//设定命令字
    u8   coil=0;          //线圈序号
    u8   speed_coding=0;  //速度编码
    u8   speed=0;         //速度值
    u8   length_coding=0; //长度编码
    u8   length_H=0;      //长度值高位
	u8   length_L=0;      	//长度值低位
	u16  length=0;       		//长度值
	u8   voltage=0;      		//电压倍数
	u8   current_H=0;     	//电流值高位
	u8   current_L=0;     	//电流值低位
	
	u8  gap_H=0;    				//线圈间距高位
	u8  gap_L=0;     				//线圈间距低位
	u16 gap=0;
	//****命令三回复帧****//
	u8   sendcache[14];
	u8   v1_H=0;
	u8   v1_L=0;
	u16  v1=0;
	u8   c1_H=0;
	u8   c1_L=0;
	u16  c1=0;
	u8   v2_H=0;
	u8   v2_L=0;
	u16  v2=0;
	u8   c2_H=0;
	u8   c2_L=0;
	u16  c2=0;
	u32 time_spend=0;
	//****输出方波的的频率控制****//
	u32 pwmval=1000;					//占空比中负脉宽的宽度（0.001ms)
	u32  frq;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    //****初始化****//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);            //初始化延时函数
	uart_init(115200);	 	    //初始化串口波特率为115200
	

	//TIM3_Init()函数
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	TIM_TimeBaseInitStructure.TIM_Period = 2-1; 	   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400/5-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//****************//
	
	
	LED_Init();  
	KEY_Init(); 											//按键初始化
	Dac12_Init();		 									//DAC通道1初始化	
	PI_Init();                  			//输入管脚初始化
	PO_Init();                  			//输出管脚初始化
 	TIM14_PWM_Init(2000-1,84-1);			//84M/84=1Mhz的计数频率,重装载值2000，所以PWM频率为 1M/2000=0.5Khz.         决定了FPGAROM中数据的刷写速度 
	TIM_SetCompare1(TIM14,pwmval);  	//修改占空比（保持50% pwmval=arr/2）
	DAC_SetChannel1Data(DAC_Align_12b_R,dacval);  //初始值为0	--------------20180528--------------------
  DAC_SetChannel2Data(DAC_Align_12b_R,dacval);	//初始值为0-------------20180528--------------------
	
	while(1)
	{		
		if(USART_RX_STA&0x8000)                          //如果一帧接收完成
		{	
			LED0=!LED0;
			len=USART_RX_STA&0x3ffff;                    //得到此次接收到的数据长度
			command=(USART_RX_BUF[1]);
			coil=(USART_RX_BUF[2]);
			speed_coding=(USART_RX_BUF[3]);
			speed=(USART_RX_BUF[4]);
			length_coding=(USART_RX_BUF[5]);            //控制波形
			length_H=(USART_RX_BUF[6]);
			length_L=(USART_RX_BUF[7]);
			voltage=(USART_RX_BUF[8]);
			current_H=(USART_RX_BUF[9]);
			current_L=(USART_RX_BUF[10]);
			gap_H=(USART_RX_BUF[11]);
			gap_L=(USART_RX_BUF[12]);
			gap=gap_L+(gap_H<<8);
			time_spend=gap*36000/speed;   //时间的单位是微秒
			length=length_L+(length_H<<8);
			//printf("length=%u\n",length);
			//printf("speed=%u\n",speed);
			switch(command)
			{
				case 0x01:
				{
					sendcache[0]=0x12;
					sendcache[1]=0x41;
					sendcache[2]=0x00;
					sendcache[3]=0x00;
					sendcache[4]=0x00;
					sendcache[5]=0x00;
					sendcache[6]=0x00;
					sendcache[7]=0x00;
					sendcache[8]=0x00;
					sendcache[9]=0x00;
					sendcache[10]=0x00;
					sendcache[11]=0x00;
					sendcache[12]=0x00;
				  sendcache[13]=0x88;	
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束    (完整的一帧） ------20180529
				for(t=0;t<14;t++)	
				{
				USART_SendData(USART1,sendcache[t]);                  //向串口1发送数据  数据是从低地址开始发送的                               
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束    (完整的一帧）              
				}
				USART_RX_STA=0;	
				
				
				
					//frq=(10000*length*3.6)/(speed*256);				//256t=(length*3.6)/(speed*100*256)    公式原型，但因为参数过大，会导致计算中间变量逸出导致出错
					frq=(12000*length)/(speed*256);				//256t=(length*3.6)/(speed*100*256)    公式原型，但因为参数过大，会导致计算中间变量逸出导致出错			
					//printf("frq=%u\n",frq);
					TIM14->ARR=10*(frq/8-1);                       //改变原来的频率(将真实值放大10倍以适应线圈的响应时间)
          TIM_SetCompare1(TIM14,10*(frq/16));					
        
					if(coil==0x03)
					{   
					{
				 	switch(length_coding)
						{
							case 0x00:
						{PO2=0;PO1=0;PO0=0;
								break;} 
							case 0x01:
						{PO2=0;PO1=0;PO0=1;
								break;}
						    case 0x02:
						{PO2=0;PO1=1;PO0=0;
						        break;}
							case 0x03: 
						{PO2=0;PO1=1;PO0=1;
						        break;}
							case 0x04:
						{PO2=1;PO1=0;PO0=0;
							    break;}
							case 0x05:
						{PO2=1;PO1=0;PO0=1;
							    break;}
							default:{PO2=0;PO1=0;PO0=0;
							break;}	
						}
					rst1=1;
						delay_us(5000);
					rst1=0;
					}

					while(time_spend!=0)    //防止函数delay_us（）函数溢出
					{
						if((time_spend/500000)>0)
						{
							delay_us(500000);						
							time_spend-=500000;
						}
						else
						{
							delay_us(time_spend);	//delay_us上限796ms，采用循环延时（单次时间太久会卡死)
							time_spend=0;
						}
					}
					//delay_us(time_spend);
					//delay_ms(200);
					//delay_us(100000);
					//rst1=0;
					//delay_us(50000-900);
				  //delay_us(40000);
					{
					switch(length_coding)
						{
							case 0x00:
						{QO2=0;QO1=0;QO0=0;
								break;}
							case 0x01:
						{QO2=0;QO1=0;QO0=1;
								break;}
						    case 0x02:
						{QO2=0;QO1=1;QO0=0;
						        break;}
							case 0x03: 
						{QO2=0;QO1=1;QO0=1;
						        break;}
							case 0x04:
						{QO2=1;QO1=0;QO0=0;
							    break;}
							case 0x05:
						{QO2=1;QO1=0;QO0=1;
							    break;}
							default:{QO2=0;QO1=0;QO0=0;
							break;}	
						}
					rst2=1;
					}	
				}
				  
           rst1=0;	
          delay_us(5000);				
           rst2=0;				
		break;
				}
		 case 0x03:
				{
		          //sendcache[0] 存放帧头			
					sendcache[0]=0x12;
					sendcache[1]=0x43;
					sendcache[2]=v1_H;
					sendcache[3]=v1_L;
					sendcache[4]=c1_H;
					sendcache[5]=c1_L;
					sendcache[6]=v2_H;
					sendcache[7]=v2_L;
					sendcache[8]=c2_H;
					sendcache[9]=c2_L;
					sendcache[10]=0x00;
					sendcache[11]=0x00;
					sendcache[12]=0x00;
					sendcache[13]=0x88;
                for(t=0;t<14;t++)
               {					
				USART_SendData(USART1,sendcache[t]);                  //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
               }
			    USART_RX_STA=0;		
				break;}
				default:USART_RX_STA=0;break;	
			}
		}

		 //v1=sum1*(3/1024);
		 v1=sum1;
		 v1_L=v1;
		 v1_H=v1>>8;
		 //v2=sum2*(3/1024);
		 v2=sum2;
		 v2_L=v2;
		 v2_H=v2>>8;
		//printf("sum1=%u",sum1);
		//printf("sum2=%u",sum2);
		//printf("v1=%u\n",v1);
		//printf("v1_L=%u\n",v1_L);
		//printf("v1_H=%u\n",v1_H);
		//printf("v2=%u\n",v1);
		//printf("v2_L=%u\n",v2_L);
		//printf("v2_H=%u\n",v2_H);
	}	
}

