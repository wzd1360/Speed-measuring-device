#ifndef __PO_H
#define __PO_H	 

#include "sys.h"
//PΪ��Ȧ1
#define PO2 PEout(14)		
#define PO1 PEout(13)		
#define PO0 PEout(12)
//QΪ��Ȧ2
#define QO2 PFout(14)		
#define QO1 PFout(13)	
#define QO0 PFout(12)	

#define rst1  PEout(15)
#define rst2  PFout(15)
void PO_Init(void);//��ʼ��	

#endif