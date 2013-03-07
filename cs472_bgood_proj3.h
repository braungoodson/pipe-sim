//FILE:cs472_bgood_proj3.h
//By Braun Goodson 2012-04-25

#ifndef cs472_bgood_proj3_h
#define cs472_bgood_proj3_h

#include <pipelineregisters.h>
#include <stdlib.h>
#include <stdio.h>

int* InitInstructionMemory(const unsigned int* instructions, const unsigned short num_instructions);
int* InitRegisters(const unsigned short num_registers);
short* InitDataMemory(const unsigned short data_memory_size);
pr_t* InitPipelineRegisters(const unsigned short num_prs);
void* PrintThese(int i, int j, int k, int* im, int* r, short* dm);

#endif
