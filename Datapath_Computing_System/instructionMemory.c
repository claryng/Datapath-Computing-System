#include "instructionMemory.h" 
#define INSTRUCTIONMEMORYSIZE 100
#define BUFSIZE 100


/********************Private members **********************/
static unsigned int instructionMem[INSTRUCTIONMEMORYSIZE];
static unsigned int readAddress = 0;
static unsigned int Instruction = 0;

/**********************static functions *******************/

static int getIndex (unsigned long a) {

  int index;

  if (a > 0)
    index = a/4;
  else
    index = a;
  return index;
}



/********************Public methods ***********************/
 

void dumpInstructionMemory(FILE *fp, int startAddress, int endAddress)
/*
   Output the values instructionMem[startAddress]..instructionMem[endAddress]
   to the file fp.
*/
{ 
  if (fp == NULL){
    fprintf(stderr, "ERROR: output file pointer NULL\n");
    exit(-1);
  }
  else {
    int startIdx = getIndex(startAddress);
    int endIdx = getIndex(endAddress);
    int i = 0;
    
    fprintf(fp, "instruction memory: \n");
    for (i = startIdx; i <= endIdx; i++) {
      fprintf(fp, "%#x\n", instructionMem[i]);
    }
  }  
}



unsigned int getInstruction(void)
/*
   return the current value of Instruction.
*/
{
   return Instruction;
}


int initializeInstructionMemory(FILE *fp, int numInst)
/*
   Read numInst MIPS instructions from fp and put them
   in the first numInst entries of the instructionMem.
   The instructions in the file are represented in ASCII Hex.

   Return 0 if successful or -1 if unsuccessful.
*/ 
{
  unsigned int instruct;

  int i = 0;
  if (fp == NULL){
    fprintf(stderr, "Can't read instruction file: null file pointer.\n");
    exit(-1);
  }
  else {
    int scanned;
    char line[BUFSIZE];

    // should probably change way to read a line since in a file
    // it is possible to not use a newline at the end
    // of a line

    while (fgets(line, BUFSIZE, fp) != NULL) {// the file still contains lines to read, get the next one
      if ( i < numInst) { // if there are still instructions to read
	scanned = sscanf(line, "%x", &instruct); // read from line
	if(scanned == 1) { // successfully read an instruction from the line
	  instructionMem[i] = instruct; // store instruction in memory
	  i++;
	}
	else // the item on the scanned line does not contain a hex value
	  {
	    fprintf(stderr,"Instruction memory file format error.  %d instructions read\n", i);
	    return(-1);
	  }
      }
      else {// the file isn't empty, but we've already read numWords items
	fprintf(stderr,"Instruction memory file error.  Too many values in file\n");
	return(-1);
    	}
    }
    if (i == numInst)
      return(0);
    else // the file did not contain numWords data items
      return(-1);
  }
}



void setInstructionReadAddress(unsigned int p)
/*
    Set the value of instructionReadAddress to p and set
    Instruction to instructionMem[p/4];
    NOTE:  This is a word-addressable memory module but p is
    for a byte addressable memory.
*/

{
  int index = getIndex(p);
  readAddress = p;
  Instruction = instructionMem[index];
}


   


