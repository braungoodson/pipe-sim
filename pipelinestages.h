//FILE:pipelinestages.h
//By Braun Goodson 2012-04-25

#ifndef pipelinestages_h
#define pipelinestages_h

#include <instructionset.h>
#include <pipelineregisters.h>
#include <cs472_bgood_proj3.h>
#include <stdio.h>

void* IF_stage(int* pc, int* im, ifid_t* IFIDWrite);
void* ID_stage(int* r, ifid_t* IFIDRead, idex_t* IDEXWrite);
void* EX_stage(idex_t* IDEXRead, exmem_t* EXMEMWrite);
void* MEM_stage(short* dm, exmem_t* EXMEMRead, memwb_t* MEMWBWrite);
void* WB_stage(int* r, memwb_t* MEMWBRead);
void* Print_out_everything(pipelineregisters_t* pr, int* r, int num_registers, int num_plrs, int clock_cycle);
void* Copy_write_to_read(pipelineregisters_t* pr);

#endif
