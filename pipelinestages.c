//FILE:pipelinestages.c
//By Braun Goodson 2012-04-25

#include <pipelinestages.h>
// -----------------------------------------------------------------------------------------------------------
void* IF_stage(int* pc, int* im, ifid_t* IFIDWrite)
{
	// Fetch next instruction out of instruction cache
	//  put it in the write version of ifid pr
	
	IFIDWrite->_32BitInstruction = im[*pc];
	IFIDWrite->ProgramCounter = (*pc) += 0x1;
}
// -----------------------------------------------------------------------------------------------------------
void* ID_stage(int* r, ifid_t* IFIDRead, idex_t* IDEXWrite)
{
	// decode instruction from ifidread
	// fetch registers
	// write to idexwrite
	// set control signals
	// if a control signal doesn't matter, then it's prolly set anyways
	
	struct r_format_type* r_format = malloc (sizeof *r_format);
	struct i_format_type* i_format = malloc (sizeof *i_format);
	
	int opcode = ((IFIDRead->_32BitInstruction & MASK_OPC) >> 26);
	
	if (opcode == 0) //r-format || nop
	{
		parser(&IFIDRead->_32BitInstruction, r_format);
		if ((r_format->func == 0x20) || (r_format->func == 0x22)) // add || sub
		{
			IDEXWrite->IDData.ReadData1 = r[r_format->rs];
			IDEXWrite->IDData.ReadData2 = r[r_format->rt];
			IDEXWrite->IDData.SignExtendedImmediate = (signed int) 0x0;
			IDEXWrite->IDData.Rt = r_format->rt;
			IDEXWrite->IDData.Rd = r_format->rd;
			IDEXWrite->WriteBack.MemToReg = 0;
			IDEXWrite->WriteBack.RegWrite = 1;
			IDEXWrite->MemoryAccess.MemWrite = 0;
			IDEXWrite->MemoryAccess.MemRead = -1;
			IDEXWrite->Execute.RegDst = 1;
			if (r_format->func == 0x20)
				IDEXWrite->Execute.ALUOp = 1;
			else if (r_format->func == 0x22)
				IDEXWrite->Execute.ALUOp = 0;
			IDEXWrite->Execute.ALUSrc = 0;
		}
		else if (r_format->func == 0x0)
		{
			IDEXWrite->IDData.ReadData1 = 0;
			IDEXWrite->IDData.ReadData2 = 0;
			IDEXWrite->IDData.SignExtendedImmediate = (signed int) 0x0;
			IDEXWrite->IDData.Rt = 0;
			IDEXWrite->IDData.Rd = 0;
			IDEXWrite->WriteBack.MemToReg = 0;
			IDEXWrite->WriteBack.RegWrite = 0;
			IDEXWrite->MemoryAccess.MemWrite = 0;
			IDEXWrite->MemoryAccess.MemRead = -1;
			IDEXWrite->Execute.RegDst = 0;
			IDEXWrite->Execute.ALUOp = 0;
			IDEXWrite->Execute.ALUSrc = 0;
		}
	}
	else if ((opcode == 0x20) || (opcode == 0x28)) //i-format, lb || sb
	{
		parsei(&opcode, &IFIDRead->_32BitInstruction, i_format);
		IDEXWrite->IDData.ReadData1 = r[i_format->rs];
		IDEXWrite->IDData.ReadData2 = r[i_format->rt];
		IDEXWrite->IDData.SignExtendedImmediate = (signed int) (i_format->constant);
		IDEXWrite->IDData.Rt = i_format->rt;
		IDEXWrite->IDData.Rd = i_format->rs;
		if (opcode == 0x28) // sb
		{
			IDEXWrite->WriteBack.MemToReg = -1;
			IDEXWrite->WriteBack.RegWrite = 0;
			IDEXWrite->MemoryAccess.MemWrite = 1;
			IDEXWrite->MemoryAccess.MemRead = 0;
			IDEXWrite->Execute.RegDst = -1;
			IDEXWrite->Execute.ALUOp = 1;
			IDEXWrite->Execute.ALUSrc = 1;
		}
		else if (opcode == 0x20) // lb
		{
			IDEXWrite->WriteBack.MemToReg = 1;
			IDEXWrite->WriteBack.RegWrite = 1;
			IDEXWrite->MemoryAccess.MemWrite = 0;
			IDEXWrite->MemoryAccess.MemRead = 1;
			IDEXWrite->Execute.RegDst = 0;
			IDEXWrite->Execute.ALUOp = 1;
			IDEXWrite->Execute.ALUSrc = 1;
		}
	}
	free(r_format);
	free(i_format);
	
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------
void* EX_stage(idex_t* IDEXRead, exmem_t* EXMEMWrite)
{
	// perform requested instruction
	// read from IDEXRead
	// write to EXMEMWrite
	
	// ALUSrc MUX
	if (IDEXRead->Execute.ALUSrc == 0)
	{
		if (IDEXRead->Execute.ALUOp == 1)
		{
			EXMEMWrite->ALUResult = IDEXRead->IDData.ReadData1 + IDEXRead->IDData.ReadData2;
		}
		else if (IDEXRead->Execute.ALUOp == 0)
		{
			EXMEMWrite->ALUResult = IDEXRead->IDData.ReadData1 - IDEXRead->IDData.ReadData2;
		}
	}
	else if (IDEXRead->Execute.ALUSrc == 1)
	{
		EXMEMWrite->ALUResult = IDEXRead->IDData.ReadData1 + IDEXRead->IDData.SignExtendedImmediate;
	}
	
	// RegDst MUX
	if (IDEXRead->Execute.RegDst == 0)
	{
		EXMEMWrite->RtRd = IDEXRead->IDData.Rt;
	}
	else if (IDEXRead->Execute.RegDst == 1)
	{
		EXMEMWrite->RtRd = IDEXRead->IDData.Rd;
	}
	
	EXMEMWrite->ReadData2 = IDEXRead->IDData.ReadData2;
	EXMEMWrite->WriteBack = IDEXRead->WriteBack;
	EXMEMWrite->MemoryAccess = IDEXRead->MemoryAccess;
	
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------
void* MEM_stage(short* dm, exmem_t* EXMEMRead, memwb_t* MEMWBWrite)
{
	// trivial at this point
	
	// control
	if (EXMEMRead->MemoryAccess.MemWrite == 0)
	{
		if (EXMEMRead->MemoryAccess.MemRead == 1)
		{
			MEMWBWrite->ReadData = dm[EXMEMRead->ALUResult];
		}
	}
	else if (EXMEMRead->MemoryAccess.MemWrite == 1)
	{
		dm[EXMEMRead->ALUResult] = EXMEMRead->ReadData2;
	}
	
	MEMWBWrite->ALUResult = EXMEMRead->ALUResult;
	MEMWBWrite->WriteBack = EXMEMRead->WriteBack;
	MEMWBWrite->RtRd = EXMEMRead->RtRd;
	
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------
void* WB_stage(int* r, memwb_t* MEMWBRead)
{
	// trivial as well
	
	// MemToReg MUX
	if (MEMWBRead->WriteBack.MemToReg == 1)
	{
		r[MEMWBRead->RtRd] = MEMWBRead->ReadData;
	}
	else if (MEMWBRead->WriteBack.MemToReg == 0)
	{
		r[MEMWBRead->RtRd] = MEMWBRead->ALUResult;
	}
	
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------
void* Print_out_everything(pipelineregisters_t* pr, int* r, int num_registers, int num_plrs, int clock_cycle)
{
	// Print out registers
	// Print out READ + WRITE PRs
	printf("----------------------------------- CLOCK CYCLE : %i -----------------------------------\n",clock_cycle);
	printf("----------------------------------- CLOCK CYCLE : %i -----------------------------------\n\n",clock_cycle);
	int i, j;
	printf("************************************* Registers: **************************************");
	for (i = 0; i < num_registers; ++i)
	{
		if (!(i % 7)) printf("\n");
		printf("Reg%02i=0x%03X;",i,r[i]);
	}
	printf("\n\n");
	printf("********************************* Pipeline Registers: *********************************\n");
	// --------------------------- IF/ID WRITE ----------------------------------
	printf
	(
		"-> IF/ID Write\nInst=0x%08X, [",
		pr->IFIDWrite._32BitInstruction
	);
	int opcode = ((pr->IFIDWrite._32BitInstruction & MASK_OPC) >> 26);
	if (opcode == 0) //r-format || nop
	{
		struct r_format_type r_format;
		parser(&pr->IFIDWrite._32BitInstruction, &r_format);
		printr(&r_format, 0x0);
	}
	else if ((opcode == 0x20) || (opcode == 0x28)) //i-format, lb || sb
	{
		struct i_format_type i_format;
		parsei(&opcode, &pr->IFIDWrite._32BitInstruction, &i_format);
		printi(&i_format,0x0);
	}
	printf
	(
		"], IncrPC=%i\n",
		pr->IFIDWrite.ProgramCounter
	);
	// --------------------------- IF/ID READ ----------------------------------
	printf
	(
		"-> IF/ID Read\nInst=0x%08X, [",
		pr->IFIDRead._32BitInstruction
	);
	opcode = ((pr->IFIDRead._32BitInstruction & MASK_OPC) >> 26);
	if (opcode == 0) //r-format || nop
	{
		struct r_format_type r_format;
		parser(&pr->IFIDRead._32BitInstruction, &r_format);
		printr(&r_format, 0x0);
	}
	else if ((opcode == 0x20) || (opcode == 0x28)) //i-format, lb || sb
	{
		struct i_format_type i_format;
		parsei(&opcode, &pr->IFIDRead._32BitInstruction, &i_format);
		printi(&i_format,0x0);
	}
	printf
	(
		"], IncrPC=%i\n\n",
		pr->IFIDWrite.ProgramCounter
	);	
	// --------------------------- ID/EX WRITE ----------------------------------
	printf("-> ID/EX Write\n");
	printf
	(
		"Control: RegDst=%i, ALUSrc=%i, ALUOp=%i, MemRead=%i, MemWrite=%i, MemToReg=%i, RegWrite=%i\n",
		pr->IDEXWrite.Execute.RegDst,
		pr->IDEXWrite.Execute.ALUSrc,
		pr->IDEXWrite.Execute.ALUOp,
		pr->IDEXWrite.MemoryAccess.MemRead,
		pr->IDEXWrite.MemoryAccess.MemWrite,
		pr->IDEXWrite.WriteBack.MemToReg,
		pr->IDEXWrite.WriteBack.RegWrite
	);
	printf
	( 
		"ReadRegValue1 = 0x%03X, ReadRegValue2 = 0x%03X, SEOffset = 0x%08X, WriteRegNum = %i,%i\n",
		pr->IDEXWrite.IDData.ReadData1,
		pr->IDEXWrite.IDData.ReadData2,
		pr->IDEXWrite.IDData.SignExtendedImmediate,
		pr->IDEXWrite.IDData.Rt, pr->IDEXWrite.IDData.Rd
	);
	printf("\n");
	// --------------------------- ID/EX Read ----------------------------------
	printf("-> ID/EX Read\n");
	printf
	(
		"Control: RegDst=%i, ALUSrc=%i, ALUOp=%i, MemRead=%i, MemWrite=%i, MemToReg=%i, RegWrite=%i\n",
		pr->IDEXRead.Execute.RegDst,
		pr->IDEXRead.Execute.ALUSrc,
		pr->IDEXRead.Execute.ALUOp,
		pr->IDEXRead.MemoryAccess.MemRead,
		pr->IDEXRead.MemoryAccess.MemWrite,
		pr->IDEXRead.WriteBack.MemToReg,
		pr->IDEXRead.WriteBack.RegWrite
	);
	printf
	( 
		"ReadRegValue1 = 0x%03X, ReadRegValue2 = 0x%03X, SEOffset = 0x%08X, WriteRegNum = %i,%i\n",
		pr->IDEXRead.IDData.ReadData1,
		pr->IDEXRead.IDData.ReadData2,
		pr->IDEXRead.IDData.SignExtendedImmediate,
		pr->IDEXRead.IDData.Rt, pr->IDEXRead.IDData.Rd
	);
	printf("\n");
	// --------------------------- EX/MEM Write ----------------------------------
	printf("-> EX/MEM Write\n");
	printf
	(
		"Control: MemRead=%i, MemWrite=%i, MemToReg=%i, RegWrite=%i\nALUResult = 0x%03X, SBValue= 0x%03X, WriteRegNum = %i\n",
		pr->EXMEMWrite.MemoryAccess.MemRead, pr->EXMEMWrite.MemoryAccess.MemWrite,
		pr->EXMEMWrite.WriteBack.MemToReg, pr->EXMEMWrite.WriteBack.RegWrite,
		pr->EXMEMWrite.ALUResult, pr->EXMEMWrite.ReadData2, pr->EXMEMWrite.RtRd
	);
	printf("\n");
	// --------------------------- EX/MEM Read ----------------------------------
	printf("-> EX/MEM Read\n");
	printf
	(
		"Control: MemRead=%i, MemWrite=%i, MemToReg=%i, RegWrite=%i,\nALUResult = 0x%03X, SBValue= 0x%03X, WriteRegNum = %i\n",
		pr->EXMEMRead.MemoryAccess.MemRead, pr->EXMEMRead.MemoryAccess.MemWrite,
		pr->EXMEMRead.WriteBack.MemToReg, pr->EXMEMRead.WriteBack.RegWrite,
		pr->EXMEMRead.ALUResult, pr->EXMEMRead.ReadData2, pr->EXMEMRead.RtRd
	);
	printf("\n");
	// --------------------------- MEM/WB Write ----------------------------------
	printf("-> MEM/WB Write\n");
	printf
	(
		"Control: MemToReg=%i, RegWrite=%i\nLWDataValue=0x%03X, ALUResult = 0x%03X, WriteRegNum = %i\n",
		pr->MEMWBWrite.WriteBack.MemToReg, pr->MEMWBWrite.WriteBack.RegWrite,
		pr->MEMWBWrite.ReadData, pr->MEMWBWrite.ALUResult, pr->MEMWBWrite.RtRd
	);
	printf("\n");
	// --------------------------- MEM/WB Read ----------------------------------
	printf("-> MEM/WB Read\n");
	printf
	(
		"Control: MemToReg=%i, RegWrite=%i\nLWDataValue=0x%03X, ALUResult = 0x%03X, WriteRegNum = %i\n",
		pr->MEMWBRead.WriteBack.MemToReg, pr->MEMWBRead.WriteBack.RegWrite,
		pr->MEMWBRead.ReadData, pr->MEMWBRead.ALUResult, pr->MEMWBRead.RtRd
	);
	printf("\n");
}
// -----------------------------------------------------------------------------------------------------------
void* Copy_write_to_read(pipelineregisters_t* pr)
{
	pr->IFIDRead = pr->IFIDWrite;
	pr->IDEXRead = pr->IDEXWrite;
	pr->EXMEMRead = pr->EXMEMWrite;
	pr->MEMWBRead = pr->MEMWBWrite;

	return NULL;
};
// -----------------------------------------------------------------------------------------------------------
