#include <stdio.h>
#include <stdlib.h>

#include "alu.h"
#include "dataMemory.h"
#include "instructionMemory.h"
#include "registers.h"

#define  ALUOP_SIG  0
#define  ALUSRC_SIG  1
#define  BRANCH_SIG  2
#define  MEMREAD_SIG  3
#define  MEMWRITE_SIG  4
#define  MEMTOREG_SIG  5
#define  JUMP_SIG  6
#define  REGDST_SIG   7
#define  REGWRITE_SIG  8

#define RTYPE  0
#define JUMP  2
#define BEQ  4
#define ORI 13
#define LW  35
#define SW  43

#define AND_OP  0
#define OR_OP  1
#define ADD_OP  2
#define SUB_OP  6
#define SLT_OP  7
#define UNKNOWN 8


typedef struct instruction {
             char formatType;
             unsigned int op;
             unsigned int rs;
             unsigned int rt;
             unsigned int rd;
             unsigned int shamt;
             int immedOrAddress;
             unsigned long target;
             unsigned int funct;
} mipsInstruction;

static void get_itype (mipsInstruction *iptr);
static void setALUOP (unsigned int opcode) ;
static void assertSignal (int controlLine);
static void deassertSignal (int controlLine); 
 
int executeInstruction(void);
unsigned long getPC(void);
void setPC(unsigned long p);
void decodeInstruction(unsigned int instruction); 
void dumpControlLines(FILE *fp);
unsigned int getALUcontrol(void);
mipsInstruction *parseMipsInstruction(unsigned int instructionRegister);
void setControl(void);
unsigned int signExtend(unsigned int p);
void dumpMipsInstruction(mipsInstruction *instruction);
void dumpCurrentInstruction();

