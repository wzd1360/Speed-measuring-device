#include "PO.h"

//波形输出管脚初始化
void PO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
  GPIO_Init(GPIOF, &GPIO_InitStructure);	
  GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12);
  GPIO_ResetBits(GPIOF,GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12);
}

//线圈1 3位波形选择管脚：PE14~PE12
//线圈1 1位使能管脚：PE15
//线圈2 3位波形选择管脚：PF14~PF12
//线圈2 1位使能管脚：PF15