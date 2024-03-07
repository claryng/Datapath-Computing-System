#include <stdio.h>
#include <stdlib.h>

static int getIndex (unsigned long a);

void assertMemWrite(void);
unsigned long getReadData(void);
void setDataMemAddress(unsigned long a);
void setMemWriteData(unsigned long d); 
int initializeDataMemory(FILE *fp, int numWords);
void dumpDataMemory(FILE *fp, int startAddress, int endAddress);
