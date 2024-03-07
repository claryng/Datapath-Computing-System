/*  The simulated register file */

#include "registers.h"

/*** Note that the "static" modifier makes these variables visible only in registers.c The can not
     be accessed by functions in any other file ***/

static unsigned long reg[32];
static unsigned long writeData;
static unsigned long readData1;
static unsigned long readData2;
static unsigned long readRegister1;
static unsigned long readRegister2;
static unsigned long writeRegister;


/********************Public Methods***************************/


void assertRegWrite(void)
/*
    Causes the value of writeData to be stored in reg[writeRegister];
*/
{
  reg[writeRegister] = writeData;
  return;
}


void displayRegisters(FILE *fp)
/*
    Output the values of the 32 registers in a nice
    format to the file fp.
*/
{
  int i;

  fprintf(fp,"Register contents: \n");
  for (i = 0; i < 32; i++)
    fprintf(fp,"reg %d: %lu \n", i, reg[i]);
  return;
} 


 
unsigned long getReadData1Value(void) 
/* 
   Return the value of the register indicated by the value 
   in readRegister1. 
*/ 
{
    return reg[readRegister1]; 
} 

 
unsigned long getReadData2Value(void) 
/* 
   Return the value of the register indicated by the value 
   in readRegister2. 
*/ 
{   
    return reg[readRegister2]; 
} 


void setReadRegister1(unsigned long val)
/*
   Set readRegister1 to val.  As a side effect, it sets
   readData1 to the value of register readRegistser1. 
*/
{
  readRegister1 = val;
  readData1 = reg[readRegister1];
  return;
}


 
void setReadRegister2(unsigned long val) 
/* 
   Set readRegister2 to val.    As a side effect, it sets 
   readData2 to the value of register readRegistser2. 
*/ 
{ 
  readRegister2 = val;
  readData2 = reg[readRegister2];
  return; 
} 


void setWriteData(unsigned long val)
/*
   Set writeData to val
*/
{
  writeData = val;
  return;
}


void setWriteRegister(unsigned long val)
/*
   Set writeRegister to val.
*/
{
  writeRegister = val;
  return;
}


