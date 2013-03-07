//FILE:pipelineregisters.h
//By Braun Goodson 2012-04-25

#ifndef pipelineregisters_h
#define pipelineregisters_h

struct ifid_s
{
	int _32BitInstruction, ProgramCounter;
};
typedef struct ifid_s ifid_t;

struct writeback_s
{
	short MemToReg, RegWrite;
};
typedef struct writeback_s writeback_t;

struct memoryaccess_s
{
	short MemWrite, MemRead;
};
typedef struct memoryaccess_s memoryaccess_t;

struct execute_s
{
	short RegDst, ALUOp, ALUSrc;
};
typedef struct execute_s execute_t;

struct iddata_s
{
	int ReadData1, ReadData2;
	signed int SignExtendedImmediate;
	int Rt, Rd;
};
typedef struct iddata_s iddata_t;

struct idex_s
{
	writeback_t WriteBack;
	memoryaccess_t MemoryAccess;
	execute_t Execute;
	iddata_t IDData;
};
typedef struct idex_s idex_t;

struct exmem_s
{
	writeback_t WriteBack;
	memoryaccess_t MemoryAccess;
	int ALUResult;
	int ReadData2;
	int RtRd;
};
typedef struct exmem_s exmem_t;

struct memwb_s
{
	writeback_t WriteBack;
	int ALUResult;
	short ReadData;
	int RtRd;
};
typedef struct memwb_s memwb_t;

struct pipelineregisters_s
{	
	ifid_t IFIDWrite, IFIDRead;
	idex_t IDEXWrite, IDEXRead;
	exmem_t EXMEMWrite, EXMEMRead;
	memwb_t MEMWBWrite, MEMWBRead;
};
typedef struct pipelineregisters_s pipelineregisters_t;
typedef pipelineregisters_t pr_t;

#endif
