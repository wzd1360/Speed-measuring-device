#include "PO.h"

//��������ܽų�ʼ��
void PO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
  GPIO_Init(GPIOF, &GPIO_InitStructure);	
  GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12);
  GPIO_ResetBits(GPIOF,GPIO_Pin_15|GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12);
}

//��Ȧ1 3λ����ѡ��ܽţ�PE14~PE12
//��Ȧ1 1λʹ�ܹܽţ�PE15
//��Ȧ2 3λ����ѡ��ܽţ�PF14~PF12
//��Ȧ2 1λʹ�ܹܽţ�PF15