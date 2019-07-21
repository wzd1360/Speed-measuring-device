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
//ALIENTEK ̽����STM32F407������ ʵ��21
//DACʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


	u16  sum1=0;           //��һ·�źŵ�ѹ���ֵ
	u16  sum2=0;
int main(void)
{ 

	
	u16  dacval=0;
	u8   E1_count=0;
	u8   t;
	u8   len=0;             //���ڽ��յ��ֽڳ���
	//****���ڲ�����ȡ****//
	u8   command=0;       	//�趨������
    u8   coil=0;          //��Ȧ���
    u8   speed_coding=0;  //�ٶȱ���
    u8   speed=0;         //�ٶ�ֵ
    u8   length_coding=0; //���ȱ���
    u8   length_H=0;      //����ֵ��λ
	u8   length_L=0;      	//����ֵ��λ
	u16  length=0;       		//����ֵ
	u8   voltage=0;      		//��ѹ����
	u8   current_H=0;     	//����ֵ��λ
	u8   current_L=0;     	//����ֵ��λ
	
	u8  gap_H=0;    				//��Ȧ����λ
	u8  gap_L=0;     				//��Ȧ����λ
	u16 gap=0;
	//****�������ظ�֡****//
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
	//****��������ĵ�Ƶ�ʿ���****//
	u32 pwmval=1000;					//ռ�ձ��и�����Ŀ�ȣ�0.001ms)
	u32  frq;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    //****��ʼ��****//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);            //��ʼ����ʱ����
	uart_init(115200);	 	    //��ʼ�����ڲ�����Ϊ115200
	

	//TIM3_Init()����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	TIM_TimeBaseInitStructure.TIM_Period = 2-1; 	   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400/5-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//****************//
	
	
	LED_Init();  
	KEY_Init(); 											//������ʼ��
	Dac12_Init();		 									//DACͨ��1��ʼ��	
	PI_Init();                  			//����ܽų�ʼ��
	PO_Init();                  			//����ܽų�ʼ��
 	TIM14_PWM_Init(2000-1,84-1);			//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ2000������PWMƵ��Ϊ 1M/2000=0.5Khz.         ������FPGAROM�����ݵ�ˢд�ٶ� 
	TIM_SetCompare1(TIM14,pwmval);  	//�޸�ռ�ձȣ�����50% pwmval=arr/2��
	DAC_SetChannel1Data(DAC_Align_12b_R,dacval);  //��ʼֵΪ0	--------------20180528--------------------
  DAC_SetChannel2Data(DAC_Align_12b_R,dacval);	//��ʼֵΪ0-------------20180528--------------------
	
	while(1)
	{		
		if(USART_RX_STA&0x8000)                          //���һ֡�������
		{	
			LED0=!LED0;
			len=USART_RX_STA&0x3ffff;                    //�õ��˴ν��յ������ݳ���
			command=(USART_RX_BUF[1]);
			coil=(USART_RX_BUF[2]);
			speed_coding=(USART_RX_BUF[3]);
			speed=(USART_RX_BUF[4]);
			length_coding=(USART_RX_BUF[5]);            //���Ʋ���
			length_H=(USART_RX_BUF[6]);
			length_L=(USART_RX_BUF[7]);
			voltage=(USART_RX_BUF[8]);
			current_H=(USART_RX_BUF[9]);
			current_L=(USART_RX_BUF[10]);
			gap_H=(USART_RX_BUF[11]);
			gap_L=(USART_RX_BUF[12]);
			gap=gap_L+(gap_H<<8);
			time_spend=gap*36000/speed;   //ʱ��ĵ�λ��΢��
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
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���    (������һ֡�� ------20180529
				for(t=0;t<14;t++)	
				{
				USART_SendData(USART1,sendcache[t]);                  //�򴮿�1��������  �����Ǵӵ͵�ַ��ʼ���͵�                               
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���    (������һ֡��              
				}
				USART_RX_STA=0;	
				
				
				
					//frq=(10000*length*3.6)/(speed*256);				//256t=(length*3.6)/(speed*100*256)    ��ʽԭ�ͣ�����Ϊ�������󣬻ᵼ�¼����м�����ݳ����³���
					frq=(12000*length)/(speed*256);				//256t=(length*3.6)/(speed*100*256)    ��ʽԭ�ͣ�����Ϊ�������󣬻ᵼ�¼����м�����ݳ����³���			
					//printf("frq=%u\n",frq);
					TIM14->ARR=10*(frq/8-1);                       //�ı�ԭ����Ƶ��(����ʵֵ�Ŵ�10������Ӧ��Ȧ����Ӧʱ��)
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

					while(time_spend!=0)    //��ֹ����delay_us�����������
					{
						if((time_spend/500000)>0)
						{
							delay_us(500000);						
							time_spend-=500000;
						}
						else
						{
							delay_us(time_spend);	//delay_us����796ms������ѭ����ʱ������ʱ��̫�ûῨ��)
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
		          //sendcache[0] ���֡ͷ			
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
				USART_SendData(USART1,sendcache[t]);                  //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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

