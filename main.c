//FILE:main.c
//By Braun Goodson 2012-04-25
//
// To Compile:
// $ gcc cs472_bgood_proj3.c instructionset.c pipelinestages.c main.c -I ./

#include <instructionset.h>
#include <pipelineregisters.h>
#include <pipelinestages.h>
#include <cs472_bgood_proj3.h>
#include <stdlib.h>

#define NUM_REGISTERS 0x20
#define DATA_MEMORY_SIZE 0x400

int main(void)
{
	int* im   = InitInstructionMemory(instructions,NUM_INSTRUCTIONS);
	int* r    = InitRegisters(NUM_REGISTERS);
	short* dm = InitDataMemory(DATA_MEMORY_SIZE);

	pipelineregisters_t* pr = InitPipelineRegisters(0x4);

	int running = NUM_INSTRUCTIONS;
	int clock_cycle = 0;
	int pc = 0x0;

	while(running)
	{
		IF_stage(&pc, im, &pr->IFIDWrite);
		ID_stage(r, &pr->IFIDRead, &pr->IDEXWrite);
		EX_stage(&pr->IDEXRead, &pr->EXMEMWrite);
		MEM_stage(dm, &pr->EXMEMRead, &pr->MEMWBWrite);
		WB_stage(r, &pr->MEMWBRead);
		Print_out_everything(pr, r, NUM_REGISTERS, 0x4, ++clock_cycle);
		Copy_write_to_read(pr);
		--running;
	}

	exit(0x0);
}
