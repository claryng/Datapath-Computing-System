OBJS = alu.o control.o dataMemory.o instructionMemory.o registers.o

all:	MIPSsim alu.o $(OBJS) 

MIPSsim:	MIPSsim.c $(OBJS)
	gcc -g MIPSsim.c $(OBJS) -o MIPSsim

alu.o:	alu.c alu.h
	gcc -g -c alu.c -o alu.o

control.o:	control.c control.h
	gcc -g -c control.c -o control.o

dataMemory.o:	dataMemory.c dataMemory.h
	gcc -g -c dataMemory.c -o dataMemory.o

instructionMemory.o:	instructionMemory.c instructionMemory.h
	gcc -g -c instructionMemory.c -o instructionMemory.o

registers.o:	registers.c registers.h
	gcc -g -c registers.c -o registers.o

clean:
	rm *.o MIPSsim
