#ifndef __PI_H
#define __PI_H	 

#include "sys.h"
//P为线圈一
#define PI11 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) //PE11
#define PI10 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10) //PE10
#define PI9 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)  //PE9
#define PI8 	    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)	 //PE8
#define PI7 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)  //PE7
#define PI6 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)	 //PE6 
#define PI5 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)  //PE5
#define PI4 	    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)	 //PE4
#define PI3 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)  //PE3
#define PI2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)  //PE2
#define PI1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)  //PE1
#define PI0 	    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)	 //PE0

//Q为线圈二
#define QI11 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11) //PF11
#define QI10 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10) //PF10
#define QI9 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)  //PD3-----------------原PF9（PWM）
#define QI8 	    GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)	 //PF8
#define QI7 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)  //PF7
#define QI6 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)	 //PF6 
#define QI5 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)  //PF5
#define QI4 	    GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)	 //PF4
#define QI3 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)  //PF3
#define QI2 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)  //PF2
#define QI1 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)  //PF1
#define QI0 	    GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)	 //PF0

void PI_Init(void);

#endif