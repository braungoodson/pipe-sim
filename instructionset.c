//FILE:instructionset.c
//By Braun Goodson 2012-04-25

#include <instructionset.h>
#include <stdio.h>

void parser
(
	const unsigned int* const instruction, 
	struct r_format_type* const format_type
)
{
	format_type->opcode = 0;
	format_type->rs = (*instruction & MASK_RG1) >> 21;
	format_type->rt = (*instruction & MASK_RG2) >> 16;
	format_type->rd = (*instruction & MASK_RG3) >> 11;
	format_type->shmt = (*instruction & MASK_SHM) >> 6;
	format_type->func = (*instruction & MASK_FNC);

}

void parsei
(
	const unsigned int* const opcode, 
	const unsigned int* const instruction, 
	struct i_format_type* const format_type
)
{
	format_type->opcode = *opcode;
	format_type->rs = (*instruction & MASK_RG1 ) >> 21;
	format_type->rt = (*instruction & MASK_RG2 ) >> 16;
	format_type->constant = (signed short) (*instruction & MASK_CNS);
}

void printr
(
	const struct r_format_type* const format_type, 
	const unsigned int instruction_addr
)
{
	unsigned int i;
	for (i = 0; i < 10; ++i)
	{
		if ((format_type->func == instruction_set[i].opcode) && (instruction_set[i].format == 1))
		{
			printf
			(
				"%s $%i, $%i, $%i",
				instruction_set[i].instruction, 
				format_type->rd, 
				format_type->rs, 
				format_type->rt
			);
		}
	}
}

void printi
(
	const struct i_format_type* const format_type,
	const unsigned int instruction_addr
)
{
	const unsigned int* const program_counter = &instruction_addr;
	int j;
	for (j = 0; j < 10; ++j)
	{
		if ((format_type->opcode == instruction_set[j].opcode) && (instruction_set[j].format == 2))
		{
			//if ((format_type->opcode == 0x20) || (format_type->opcode == 0x28))
			//{
			//	printf
			//	(
			//		"%s $%i, $%i, $%i\n",
			//		instruction_set[j].instruction, 
			//		format_type->rs, 
			//		format_type->rt, 
			//		(format_type->constant << 2)
			//	);
			//}
			//else
			//{
				printf
				(
					"%s $%i, %i($%i)", 
					instruction_set[j].instruction, 
					format_type->rt,
					format_type->constant, 
					format_type->rs
				);
			//}
		}
	}
}
