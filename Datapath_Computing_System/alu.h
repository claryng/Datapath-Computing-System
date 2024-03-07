#include <stdio.h>
#include <stdlib.h>  

#define AND 0x0
#define OR 0x1
#define ADD 0x2
#define SUB 0x6
#define SLT 0x7

unsigned long getALUZero(void);
unsigned long getALUResult(void);
void setALUOperation(unsigned long val);
void setALULeftInput(unsigned long val);
void setALURightInput(unsigned long val);

