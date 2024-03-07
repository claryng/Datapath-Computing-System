/*
    Authors: Clary Nguyn, Amy Le, Miriam Xu
    Version: 4/10/23
    Description: Simulates a MIPS processor by executing instructions 
                and displaying the contents of registers and data memory
*/
#include "control.h"
#include <stdlib.h>

static int total; // The total number of instructions
static int numInst; // The number of instructions in the input file
static unsigned long PC; // The program counter
static int halt = 0; // A flag indicating whether a halt instruction has been encountered

/*
    Displays the menu of options
*/
static void menu(void)
{
    printf("Please choose 1 option from below:\n");
    printf("\t0 (DEFAULT) - Execute the next n instruction\n");
    printf("\t1 - Dump Data Memory\n");
    printf("\t2 - Display Registers\n");
    printf("\t3 - Exit\n");
    printf("\tOthers - Show this menu again\n");
    return;
}

/*
    Executes the next n instructions
*/
static void execute(void)
{
    int num = 0; // The number of instructions to execute
    unsigned int tempAddress; // A temporary address variable
    int count = 0; // A counter for the number of instructinos executed

    // Prompt the user for the number of instructions to execute
    printf("\nHow many instructions do you want to execute next? Please enter a number less than or equal to %d: ", total);
    scanf("%d", &num);

    // Check if input is valid
    if(num <= total && num)
    {
        // Calculate the starting address of the instructions to execute
        tempAddress = total < numInst ? (numInst - total) * 4 : 0;
        setPC(tempAddress);

        // Loop through the instructions and execute
        while(count < num)
        {
            // Execute the instruction and check if it was a halt instruction
            halt = executeInstruction();
            if(halt)
            {
                printf("\nHalt!\n\n");
                break;
            }
            // Check if there was a jump or branch instruction
            if((getPC() - PC) != 4) 
            {
                // Update the total number of instructions left to execute based on the new PC value
                total = numInst - (getPC() / 4) + 1;
                printf("\nJumped or Branched! PC = %lu\n\n", getPC());
            }
            // Update the program counter and counter variables
            setPC(PC = getPC());
            count++;
            total--;
        }
    }
    else{
        printf("No more/too few instructions to execute or out of range.\n");
        return;
    }
    return;
}

// The main function of the program
int main(int argc, char *argv[])
/*
    Main program for group project.
*/
{
    //initialize instruction memory
    FILE* in;   // A file pointer for the instruction input file
    char *inFile = argv[1]; // The name of the instruction input file
    total = numInst = atoi(argv[2]); // The total and number of instructions
    in = fopen(inFile, "r"); // Open the instruction input file
    initializeInstructionMemory(in, numInst); // Initialize the instruction memory

    //initialize data memory
    FILE* inData;   // A file pointer for the data input file
    char *inDataFile = argv[3];  // The name of the data input file
    int numData = atoi(argv[4]); // The number of data 
    inData = fopen(inDataFile, "r");
    initializeDataMemory(inData, numData); // Initialize the data memory

    // display regs
    FILE* out;

    // display data
    FILE* outData;
    int startAddress = 0, endAddress = 0;

    //loop to display menu and execute user-selected options
    int option = 0;
    while(1)
    {
        menu(); // Display the menu
        printf("Please enter your option: ");
        scanf("%d", &option); // Get user input for menu option
        if(option == 3) // If user selects option 3, exit
        {
            return 0;
        }

        switch(option) // Execute user-selected option
        {
            case 0: // Execute the next n instruction
                if(!halt) execute(); // If the CPU is not halted, execute the next instruction 
                else printf("\nHalt instruction encountered!\n\n"); // Otherwise, display a message indicating that a halt instruction was encountered
                break;
            case 1: // Dump Data Memory
                outData = fopen("outData.txt", "w"); // Open a file to write the contents of data memory
                printf("Please enter a decimal start address divisible by 4, it should be from 0 up: "); //
                scanf("%d", &startAddress); // Get user input for start address 
                printf("Please enter a decimal end address divisible by 4, it should be from 0 up: ");
                scanf("%d", &endAddress); // Get user input for end address 
                dumpDataMemory(outData, startAddress, endAddress); // Dump the contents of data memory between start and end addresses to the file
                fclose(outData);
                printf("\nLook for outData.txt in current directory.\n\n"); // Display a message indicating where the file can be found
                break;
            case 2: // Display Registers
                out = fopen("outRegs.txt", "w"); // Open a file to write the contents of registers
                displayRegisters(out); // Write the contents of registers to the file
                fclose(out);
                printf("\nLook for outRegs.txt in current directory.\n\n"); // Display a message indicating where the file can be found
                break;
            default: 
                menu(); // If user input is not valid, display the menu again
                break; 
        }
    }
    return 0; // Exit the program
}


