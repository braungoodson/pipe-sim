//FILE:cs472_bgood_proj3.c
//By Braun Goodson 2012-04-25

#include <cs472_bgood_proj3.h>

int* InitInstructionMemory(const unsigned int* instructions, const unsigned short num_instructions)
{
	int* im = (int*) malloc(num_instructions * sizeof(int));
	int j;
	for (j = 0; j < num_instructions; ++j)
	{
		im[j] = instructions[j];
	}
	return im;
}

int* InitRegisters(const unsigned short num_registers)
{
	int* r = (int*) malloc(num_registers * sizeof(int));
	int i;
	r[0] = 0x0;
	for (i = 1; i < num_registers; ++i)
	{
		r[i] = i +0x100;
	}
	return r;
}

short* InitDataMemory(const unsigned short data_memory_size)
{
	short* dm = (short*) malloc(data_memory_size * sizeof(short));
	int i;
	for (i = 0; i < data_memory_size; ++i)
	{
		dm[i] = i % 256;
	}
	return dm;
}

pr_t* InitPipelineRegisters(const unsigned short num_prs)
{
	return (pr_t*) malloc(num_prs * sizeof(pr_t));
}

void* PrintThese(int i, int j, int k, int* im, int* r, short* dm);
void* PrintThese(int i, int j, int k, int* im, int* r, short* dm)
{
	int n, m, l;

	for (n = 0; n < i; ++n)
	{
		printf("im=0x%03X\n",im[n]);
	}

	for (m = 0; m < j; ++m)
	{
		printf("r=0x%03X\n",r[m]);
	}

	for (l = 0; l < k; ++l)
	{
		printf("dm=0x%03X\n",dm[l]);
	}
	
}
