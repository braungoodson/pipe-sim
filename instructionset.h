//FILE:instructionset.h
//By Braun Goodson 2012-04-25

#ifndef instructionset_h
#define instructionset_h

#define MASK_OPC 0xFC000000
#define MASK_RG1 0x03E00000
#define MASK_RG2 0x001F0000
#define MASK_RG3 0x0000F800
#define MASK_SHM 0x000007C0
#define MASK_FNC 0x0000003F
#define MASK_CNS 0x0000FFFF

#define NUM_INSTRUCTIONS 0xC

struct instruction_type { char instruction [3]; int opcode; int format; };

struct r_format_type
{
	unsigned int opcode; unsigned int rs; unsigned int rt;
	unsigned int rd; unsigned int shmt; unsigned int func;
};

struct i_format_type
{
	unsigned int opcode; unsigned int rs; unsigned int rt;
	signed short constant;
};

static const unsigned int instructions [NUM_INSTRUCTIONS] =
{
	0xA1020000, 0x810AFFFC, 0x00831820, 0x01263820,
	0x01224820, 0x81180000, 0x81510010, 0x00624022,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static const struct instruction_type instruction_set [10] =
{
	{"add", 0x20, 1}, {"sub", 0x22, 1}, 
	{"lb ", 0x20, 2}, {"sb ", 0x28, 2},
	{"nop", 0x00, 0}
};

void parser
(
	const unsigned int* const instruction, 
	struct r_format_type* const format_type
);

void parsei
(
	const unsigned int* const opcode, 
	const unsigned int* const instruction, 
	struct i_format_type* const format_type
);

void printr
(
	const struct r_format_type* const format_type, 
	const unsigned int instruction_addr
);

void printi
(
	const struct i_format_type* const format_type, 
	const unsigned int instruction_addr
);

#endif
