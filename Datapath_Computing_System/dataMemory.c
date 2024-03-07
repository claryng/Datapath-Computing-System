#include "dataMemory.h"

#define DATAMEMORYSIZE 100
#define BUFSIZE 100

/*********************Private Members**********************/

static unsigned long dataMem[DATAMEMORYSIZE];
static unsigned long Address = 0;
static unsigned long WriteData = 0;
static unsigned long ReadData = 0;


/**********************static functions *******************/

static int getIndex (unsigned long a) {
  int index;

  if (a > 0)
    index = a/4;
  else
    index = a;
  return index;
}



/**********************Public Methods**********************/


void assertMemWrite(void)
/*
   Writes the value of WriteData to dataMem[Address/4].
   NOTE:  We are simulating data memory using an array.  The array is word-addressable, but the addresses for MIPS are for a byte-addressable memory.  Therefore, you must divide by 4 to get the correct array location.
*/
{
  int index;

  index = getIndex(Address);
  dataMem[index] = WriteData;
}



unsigned long getReadData(void)
/*
   Returns the value of ReadData.
*/
{
     return ReadData;
}



void setDataMemAddress(unsigned long a)
/*
   Sets Address to a and ReadData to dataMem[Address/4]
*/
{
  int index = getIndex(a);
  Address = a;
  ReadData = dataMem[index];
}
   


void setMemWriteData(unsigned long d)
/* 
   Sets WriteData to d.
*/
{
  WriteData = d;
}


int initializeDataMemory(FILE *fp, int numWords)
/*
   Read numWords words of data from fp and put them
   in the first numWords entries of the dataMem.
   The words in the file are represented in ASCII Hex.

   Return 0 if successful or -1 if unsuccessful.
*/ 
{
  unsigned int dataVal;

  int i = 0;
  if (fp == NULL){
    fprintf(stderr, "Can't read data file: null file pointer.\n");
    exit(-1);
  }
  else {
    int scanned;
    char line[BUFSIZE];

    while (fgets(line, BUFSIZE, fp) != NULL) {// while the file still contains lines to read, read the next one into line buffer
      if ( i < numWords) { // get another data value 
	scanned = sscanf(line, "%x", &dataVal); // read from line
	if(scanned == 1) { // successfully read a data value from the string
	  dataMem[i] = dataVal; // store value in memory
	  i++;
	}
	else // the item on the scanned line does not contain a hex value
	  {
	    fprintf(stderr,"Data memory file format error.  %d data values read\n", i);
	    return(-1);
	  }
      }
      else {// the file isn't empty, but we've already read numWords items
	fprintf(stderr,"Data memory file error.  Too many values in file\n");
	return(-1);
      }
    }
    if (i == numWords)
      return(0);
    else // the file did not contain numWords data items
      return(-1);
  }
}



 
void dumpDataMemory(FILE *fp, int startAddress, int endAddress)
/*
   Output the contents of the dataMem from startAddress to
   endAddress to the file fp in a readable format for debugging.

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

    fprintf(fp, "data memory: \n");
    for (i = startIdx; i <= endIdx; i++) {
      fprintf(fp, "%#lx\n", dataMem[i]);
    }
  }
}
