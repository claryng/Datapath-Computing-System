/*Miriam, Clary, Amy*/
#include "control.h"

/******************************** Control Lines *******************/
static unsigned int ALUOp = 0;
static unsigned int ALUSrc = 0;
static unsigned int Branch = 0;
static unsigned int MemRead = 0;
static unsigned int MemWrite = 0;
static unsigned int MemtoReg = 0;
static unsigned int Jump = 0;
static unsigned int RegDst = 0;
static unsigned int RegWrite = 0;

/* This is a bit of a kludge to be able to flag
 * when we have decoded a halt instruction which
 * indicates the end of a program.  Set to 1 when
 * the current instruction is halt.
 */
static unsigned int Halt = 0;

/****************************************************************/

static unsigned int currentInstruction = 0;

// address of next instruction
static unsigned long PC = 0;

/**
 ** the data to be read from memory
 **/
static unsigned int memData;

/*
 * NOTE: decodedInstruction is a pointer (reference) to a structure
 * of type InstructionParseType.  Much like an object variable declaration in Java,
 * all this declaration does, is reserve space for the reference ...
 * no memory is allocated for the structure.
 *
 * Memory for the structure will be allocated by the call to malloc()
 * in parseMipsInstruction.  In Java, we would call new with a class constructor.
 *
 * NOTE: Keep the following in mind as you write your own code:
 * When the current parsed instruction is has been executed, its
 * memory must be deallocated before calling parseMipsInstruction
 * on the next value of currentInstruction.  In the C programming
 * language, memory is freed via the library function free().
 * For example,
 *
 * free(decodedInstruction);
 *
 * will free storage "pointed to" by decodedInstruction.
 */

static mipsInstruction *decodedInstruction = NULL;

/*************** Any private (static) variables and functions go here ************/

static void setALUOP(unsigned int opcode)
{
  if (opcode == RTYPE) // r-type
    ALUOp = 2;
  else if (opcode == BEQ) // beq
    ALUOp = 1;
  else
    ALUOp = 0;
}

static void assertSignal(int controlLine)
{

  switch (controlLine)
  {
  case (ALUSRC_SIG):
    ALUSrc = 1;
    break;
  case (BRANCH_SIG):
    Branch = 1;
    break;
  case (MEMREAD_SIG):
    MemRead = 1;
    break;
  case (MEMWRITE_SIG):
    MemWrite = 1;
    break;
  case (MEMTOREG_SIG):
    MemtoReg = 1;
    break;
  case (REGWRITE_SIG):
    RegWrite = 1;
    break;
  case (JUMP_SIG):
    Jump = 1;
    break;
  case (REGDST_SIG):
    RegDst = 1;
    break;
  }
}

static void deassertSignal(int controlLine)
{

  switch (controlLine)
  {
  case (ALUSRC_SIG):
    ALUSrc = 0;
    break;
  case (BRANCH_SIG):
    Branch = 0;
    break;
  case (MEMREAD_SIG):
    MemRead = 0;
    break;
  case (MEMWRITE_SIG):
    MemWrite = 0;
    break;
  case (MEMTOREG_SIG):
    MemtoReg = 0;
    break;
  case (REGWRITE_SIG):
    RegWrite = 0;
    break;
  case (JUMP_SIG):
    Jump = 0;
    break;
  case (REGDST_SIG):
    RegDst = 0;
    break;
  }
}

static unsigned int getHalt(void)
{
  // we discover that all of the halt instruction are 0xffffffff
  if (decodedInstruction->op == 0x3f) // if that is the case then the opcode should be 0x3f
  {
    Halt = 1;
  }
  else
  {
    Halt = 0;
  }
  return Halt;
}

static void incrementPC(void)
/*
 * Increment PC by 4
 */
{
  PC += 4;
  setPC(PC);
  return;
}

static void checkRegDst(void)
/*
 * Check RegDst Signal
 */
{
  if (RegDst)
  {
    setWriteRegister(decodedInstruction->rd);
  }
  else
  {
    setWriteRegister(decodedInstruction->rt);
  }
  return;
}

static void checkRegWrite(void)
/*
 * Check RegWrite Signal
 */
{
  if (RegWrite)
  {
    assertRegWrite();
  }
  return;
}

static void checkALUSrc(void)
/*
 * Check ALUSrc Signal
 */
{
  if (ALUSrc)
  {
    setALURightInput(signExtend(decodedInstruction->immedOrAddress));
  }
  else
  {
    setALURightInput(getReadData2Value());
  }
  return;
}

static void checkMemRead(void)
/*
 * Check MemRead Signal
 */
{
  if (MemRead)
  {
    memData = getReadData();
  }
  return;
}

static void checkMemWrite(void)
/*
 * Check MemWrite Signal
 */
{
  if (MemWrite)
  {
    assertMemWrite();
  }
  return;
}

static void checkMemToReg(void)
/*
 * Check MemToReg Signal
 */
{
  if (MemtoReg)
  {
    setWriteData(memData);
  }
  else
  {
    setWriteData(getALUResult());
  }
  return;
}

static void checkPCSrc(void)
/*
 * Check Branch Signal and ALUZero
 */
{
  unsigned long p = getPC();
  if (Branch && getALUZero())
  {
    // for memory address, do unsign-extension
    p += (decodedInstruction->immedOrAddress & 0x0000ffff) << 2;
  }
  setPC(p);
  return;
}

static void checkJump(void)
/*
 * Check Jump Signal and set PC
 */
{
  unsigned long p = getPC();
  if (Jump)
  {
    p = (decodedInstruction->target << 2) | (p & 0xf0000000);
  }
  setPC(p);
}

static void fetchOperands(void)
/*
 * Operate fetch stage
 */
{
  checkJump();
  setReadRegister1(decodedInstruction->rs);
  setReadRegister2(decodedInstruction->rt);
  checkRegDst();
  return;
}

static void performAluOperation(void)
/*
 * ALU Operation
 */
{
  setALULeftInput(getReadData1Value());
  checkALUSrc();
  setALUOperation(getALUcontrol());
  checkPCSrc();
  return;
}

static void memoryWriteRead(void)
/*
 * Memory Write and Read Operation
 */
{
  setMemWriteData(getReadData2Value());
  setDataMemAddress(getALUResult());
  checkMemWrite();
  checkMemRead();
  return;
}

static void writeBack(void)
/*
 * Write back to the register file
 */
{
  checkMemToReg();
  checkRegWrite();
  return;
}

/***************************Public Methods********************/

int executeInstruction(void)
/*
 * Execute the instruction whose address is in PC, by performing the
 * stages/steps of the execution cycle.  Do not use any form of
 * conditional that checks for the type or opcode of the instruction.
 * Rather your code should simulate the processes that the datapath
 * performs based upon the control signals at each stage of execution.
 *
 * This function should not contain code that executes conditionally based
 * upon the instruction that is to be executed.  It should be general and
 * capable of executing any of the instructions you've implemented.
 * The best way to approach this is to write several helper
 * (must be static) functions.  Use abstraction to do this keeping in mind
 * that executing an instruction takes 5 stages.  Be sure you are clear what steps
 * are completed in each stage.
 * Be sure to use free() to release the memory used for the last instruction
 * decoded, before starting execution of the next instruction.
 * If we have decoded a Halt instruction, return 1,
 * otherwise return 0
 */
{
  // Stage 1: Fetch instruction from PC into instruction memory. Do PC + 4
  setInstructionReadAddress(getPC());
  currentInstruction = getInstruction();
  incrementPC();

  // Stage 2: Decode instruction and fetch operands into registers. Set controls
  decodeInstruction(currentInstruction);
  setControl();
  if (getHalt())
  {
    free(decodedInstruction);
    return 1;
  }
  fetchOperands();

  // Stage 3: Perform ALU
  performAluOperation();

  // Stage 4: Write or read from memory
  memoryWriteRead();

  // Stage 5: Write back
  writeBack();

  free(decodedInstruction);
  return 0;
}

unsigned long getPC(void)
/*
    return the current value of the PC.
*/
{
  return PC;
}

void setPC(unsigned long p)
/*
   Set PC to the value p.
*/
{
  PC = p;
  return;
}

void decodeInstruction(unsigned int instruction)
/*
  Calls parseMipsInstruction (written in the decoder lab (lab 4), my implmentation
  is provided below) on instruction and sets decodedInstruction by the return value.
*/
{
  decodedInstruction = parseMipsInstruction(instruction);
}

void dumpControlLines(FILE *fp)
/*
    Output the values of the Control Lines to fp.
*/
{

  fprintf(fp, "ALUOp: %u \n", ALUOp);
  fprintf(fp, "ALUSrc: %u\n", ALUSrc);
  fprintf(fp, "Branch: %u\n", Branch);
  fprintf(fp, "MemRead: %u\n", MemRead);
  fprintf(fp, "MemWrite: %u\n", MemWrite);
  fprintf(fp, "MemtoReg: %u\n", MemtoReg);
  fprintf(fp, "Jump: %u\n", Jump);
  fprintf(fp, "RegDst: %u\n", RegDst);
  fprintf(fp, "RegWrite: %u\n", RegWrite);
  return;
}

void dumpCurrentInstruction()
{
  dumpMipsInstruction(decodedInstruction);
}

unsigned int getALUcontrol(void)
/*
    Return the Operation for the ALU according Table 4.13 of PH.
    The Function field bits are obtained from the funct field of
    DecodedInstruction, and the ALUOp is the current value of
    the Control Line ALUOp.
*/
{
  unsigned int funct = decodedInstruction->funct;

  if (ALUOp == 2)
  { // rtype
    if (funct == 32)
    { // add
      return ADD_OP;
    }
    else if (funct == 42)
    { // slt
      return SLT_OP;
    }
    else if (funct == 37)
    { // or
      return OR_OP;
    }
    else if (funct == 36)
    { // and
      return AND_OP;
    }
    else if (funct == 34)
    {
      return SUB_OP;
    }
    else
      return UNKNOWN;
  }
  else if (ALUOp == 0)
  { // lw or sw
    return ADD_OP;
  }
  else if (ALUOp == 1)
  { // beq
    return SUB_OP;
  }
  else
    return UNKNOWN;
}

void setControl(void)
/*
 Sets the Control Lines declared above based on the opcode field in DecodedInstruction
 as indicated in PH Figure 4.18.
 */
{
  unsigned int opcode = decodedInstruction->op;

  setALUOP(opcode);
  switch (opcode)
  {
  case (RTYPE):
    assertSignal(REGDST_SIG);
    deassertSignal(ALUSRC_SIG);
    deassertSignal(MEMTOREG_SIG);
    assertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
    break;
  case (ORI):
    deassertSignal(REGDST_SIG);
    assertSignal(ALUSRC_SIG);
    deassertSignal(MEMTOREG_SIG);
    assertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
    break;
  case (LW):
    deassertSignal(REGDST_SIG);
    assertSignal(ALUSRC_SIG);
    assertSignal(MEMTOREG_SIG);
    assertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    assertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
    break;
  case (SW):
    assertSignal(ALUSRC_SIG);
    deassertSignal(REGWRITE_SIG);
    assertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
    break;
  case (BEQ):
    deassertSignal(ALUSRC_SIG);
    deassertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    assertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
    break;
  case (JUMP):
    deassertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    assertSignal(JUMP_SIG);
    break;
  default:
    deassertSignal(REGDST_SIG);
    deassertSignal(ALUSRC_SIG);
    deassertSignal(MEMTOREG_SIG);
    deassertSignal(REGWRITE_SIG);
    deassertSignal(MEMWRITE_SIG);
    deassertSignal(MEMREAD_SIG);
    deassertSignal(BRANCH_SIG);
    deassertSignal(JUMP_SIG);
  }
  return;
}

unsigned int signExtend(unsigned int p)
/*
 Return the value which is the lower 16 bits of p sign-extended
 to 32 bits.
 */
{
  /*
   * cast p as an integer
   */

  unsigned int val;

  // extract the value of bit 15 (sign bit)
  val = p & 0x00008000;

  if ((0x8000 & val) == 0x8000) // sign bit = 1 so set upper 16 bits to 1
    p = p | 0xFFFF0000;
  return p;
}

/* function to take an unsigned int that contains a MIPS instruction
 and parse it, returning a pointer to a structure containing the decoded
 subfields of the instruction */

mipsInstruction *parseMipsInstruction(unsigned int instructionRegister)
{

  /* declare a variable whose type is reference (address) to an mipsInstruction object */
  mipsInstruction *decodedInstruction;

  /* request memory for an mipsInstruction object */
  decodedInstruction = malloc(sizeof(mipsInstruction));

  /* if space was not available, quit*/
  if (decodedInstruction == NULL)
  {
    fprintf(stderr, "Unable to malloc memory for instruction\n");
    exit(1);
  }
  else
  { /* decode instruction*/
    decodedInstruction->op = instructionRegister >> 26;
    decodedInstruction->rs = instructionRegister >> 21;
    decodedInstruction->rs = decodedInstruction->rs & 0x0000001f;
    decodedInstruction->rt = instructionRegister >> 16;
    decodedInstruction->rt = decodedInstruction->rt & 0x000001f;
    decodedInstruction->rd = instructionRegister >> 11;
    decodedInstruction->rd = decodedInstruction->rd & 0x000001f;
    decodedInstruction->shamt = instructionRegister >> 6;
    decodedInstruction->shamt = decodedInstruction->shamt & 0x000001f;
    decodedInstruction->funct = instructionRegister & 0x000003f;
    decodedInstruction->immedOrAddress = instructionRegister & 0x0000ffff;
    decodedInstruction->target = instructionRegister & 0x03ffffff;
    get_itype(decodedInstruction);
  }

  return decodedInstruction;
}

/* param iptr is the address of a mipsInstruction structure containing
 the decoded opcode of an instruction.  This information is used
 to enter the instruction format type into the formatType field.
 */

static void get_itype(mipsInstruction *iptr)
{
  if (iptr->op == 0)
    iptr->formatType = 'R';
  else if (iptr->op == 2)
    iptr->formatType = 'J';
  else
    iptr->formatType = 'I';
}

/* function to print the fields of a MIPS instruction in both decimal and hex
 param instruction is the address of a mipsInstruction structure that
 contains the decoded fields of the instruction to be printed. */

void dumpMipsInstruction(mipsInstruction *instruction)
{

  printf("%11s:%u , %x\n", "opcode", instruction->op, instruction->op);
  printf("%11s:%u , %x\n", "rs", instruction->rs, instruction->rs);
  printf("%11s:%u , %x\n", "rt", instruction->rt, instruction->rt);
  printf("%11s:%u , %x\n", "rd", instruction->rd, instruction->rd);
  printf("%11s:%u , %x\n", "shamt", instruction->shamt, instruction->shamt);
  printf("%11s:%u , %x\n", "funct", instruction->funct, instruction->funct);
  printf("%11s:%d , %x\n", "immediate", instruction->immedOrAddress, instruction->immedOrAddress);
  printf("%11s:%lu , %lx\n", "target", instruction->target, instruction->target);
}
