#include "alu.h"

static unsigned long ALURightInput=0;
static unsigned long ALULeftInput=0;
static unsigned long ALUResult=0;
static unsigned long ALUZero=0;
static unsigned long ALUOperation=0;

static void setALUZero ();
static void performALUOperation ();

/************************Public Methods*******************************/ 

unsigned long getALUResult(void)
/*
   Return the current value of ALUResult.
*/
{
    return ALUResult;
}


unsigned long getALUZero(void)
/*
   Return the current value of ALUZero.
*/
{  
    return ALUZero;
}


void setALUOperation(unsigned long val)
/* 
   Set the value of the ALU operation.  As a side effect
   evaluate the operation on the current ALU inputs and
   set ALUResult and ALUZero.
 
   The ALU control input values are:
        control          function
          000              AND
          001              OR
          010              add
          110              subtract  (left - right)
          111              set on less than
   Note:  The controls should be set to numbers between 0 and 7.
   If the control is not valid, do not change the ALUOperation
   or re-evaluate the ALU outputs.
*/
{
  if ((val >= 0) && (val <= 7)){
    ALUOperation = val;
    performALUOperation();
  }
    return;
}

  static void performALUOperation () {
    switch (ALUOperation){
    case AND:
      { 
	ALUResult = ALULeftInput & ALURightInput;
	setALUZero();
	break;
      }
    case OR:
      {
	ALUResult = ALULeftInput | ALURightInput;
	setALUZero();
	break;
      }
    case ADD:
      {
	ALUResult = ALULeftInput+ALURightInput;
	setALUZero();
	break;
      }
    case SUB:
      {
	ALUResult = ALULeftInput-ALURightInput;
	setALUZero();
	break;
      }
    case SLT:
      {
	ALUResult = ALULeftInput - ALURightInput;
	setALUZero();
	if ((int) ALUResult < 0)
	  ALUResult = 1;
	else
	  ALUResult = 0;
	break;
      }
    }

  }
 
void setALULeftInput(unsigned long val)
/*
   Set the left input of the ALU to val.  (As a side effect,
   reevaluate the ALU outputs as in setALUOperation.) 
*/
{
  ALULeftInput = val;
  performALUOperation();
  return;
}


void setALURightInput(unsigned long val)
/*
   Set the right input of the ALU to val.  (As a side effect,
   reevaluate the ALU outputs as in setALUOperation.) 
*/
{
  ALURightInput = val;
  performALUOperation();
  return;
}


static void setALUZero (){
  if (ALUResult!= 0)
     ALUZero = 0;
   else
     ALUZero = 1;
 }
