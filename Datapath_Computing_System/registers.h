#include <stdio.h>
#include <stdlib.h>
void assertRegWrite(void);
void displayRegisters(FILE *fp);

unsigned long getReadData1Value(void);
unsigned long getReadData2Value(void);
void setReadRegister1(unsigned long val);
void setReadRegister2(unsigned long val);
void setWriteData(unsigned long val);
void setWriteRegister(unsigned long val);

