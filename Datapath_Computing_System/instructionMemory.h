#include <stdio.h>
#include<stdlib.h>

void dumpInstructionMemory(FILE *fp, int startAddress, int endAddress); 
unsigned int getInstruction(void);
int initializeInstructionMemory(FILE *fp, int numInst);
void setInstructionReadAddress(unsigned int p);

