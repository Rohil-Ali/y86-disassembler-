#include <stdio.h>
#include <stdlib.h>

//This is the help function that reads y86 binary code from a file
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize);
int immedValue(char memLocation1, char memLocation2, char memLocation3, char memLocation4);

int main(int argc, char **argv)
{
  unsigned int MAX_MEMSIZE = 4096;
  unsigned char memory[MAX_MEMSIZE]; //This array represents the 4KB memory space
  unsigned int PC = 0;               //This is the initial program counter address

  if (argc < 2)
  {
    printf("Usage: please specify a y86 binary program file in the argument.\n");
    return -1;
  }

  unsigned int program_size = loadBinFromFile(argv[1], memory, MAX_MEMSIZE);

  if (program_size == 0)
    return 0;

  // This is an array of register mnemonics in y86
  const char *register_names[] = {"%eax", "%ecx", "%edx",
                                  "%ebx", "%esp", "%ebp",
                                  "%esi", "%edi", "UNKNOWN_REGSITER"};


  //TODO: Implement your disassembler from here
  //At the moment, it simply display each byte from the memory.
  while (PC < program_size)
  {
    //printf("%02X\n", memory[PC]);

    //Variables which store the integer id for the registers
    int operandA = 0;
    int operandB = 0;

    //These 2 variables store the result of teh functions which essentially decodes the immediate and memory access values. 
    int immediateVal = immedValue(memory[PC + 2], memory[PC + 3], memory[PC + 4], memory[PC + 5]);
    unsigned int unsignedMemory = immedValue(memory[PC + 1], memory[PC + 2], memory[PC + 3], memory[PC + 4]);

    //These if statements wait for any matching operand byte and will execute the relavent statement. each if statement represents an intruction and will print out the correct structure in accordance to the operand. Once the if statement is executed, the PC will be incremented by the size of the instruction.

    if (memory[PC] == 0x30)
    {
      operandB = memory[PC + 1] & 0x0F;
      printf("irmovl $%d, %s\n", immediateVal, register_names[operandB]);
      PC += 5;
    } 

    else if (memory[PC] == 0x20)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      printf("rrmovl %s, %s\n", register_names[operandA], register_names[operandB]);
      PC += 1;
    }

    else if (memory[PC] == 0x40)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      printf("rmmovl %s, %d(%s)\n", register_names[operandA], immediateVal, register_names[operandB]);
      PC += 5;
    } 

    else if (memory[PC] == 0x60)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      printf("addl %s, %s\n", register_names[operandA], register_names[operandB]);
      PC += 1;
    }

    else if (memory[PC] == 0x70)
    {
      printf("jmp %u\n", unsignedMemory);
      PC += 4;
    } 

    else if (memory[PC] == 0x50)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      //This if statement checks if the immediate value is 0, if it is then it will print out the instruction without the immediate value, this avoids the 0 being outputted. This is so it matches the same format as prog1.o
      if (immediateVal == 0)
      {
        printf("mrmovl (%s), %s\n", register_names[operandB], register_names[operandA]);
      }
      else
      {
         printf("mrmovl %d(%s), %s\n", immediateVal, register_names[operandB], register_names[operandA]);
      }    
      PC += 5;
    } 

    else if (memory[PC] == 0x62)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      printf("andl %s, %s\n", register_names[operandA], register_names[operandB]);
      PC += 1;
    }

    else if (memory[PC] == 0x74)
    {
      printf("jne %u\n", unsignedMemory);
      PC += 4;
    } 

    else if (memory[PC] == 0xB0)
    {
      operandA = memory[PC + 1] >> 4;
      printf("popl %s\n", register_names[operandA]);
      PC += 1;
    }

    else if(memory[PC] == 0x90)
    {
      printf("ret\n");
    }

    else if (memory[PC] == 0x80)
    {
      printf("call %u\n", unsignedMemory);
      PC += 4;
    } 

    else if (memory[PC] == 0x00)
    {
      printf("halt\n");      
    }

    else if (memory[PC] == 0xA0)
    {
      operandA = memory[PC + 1] >> 4;
      printf("pushl %s\n", register_names[operandA]);
      PC += 1;
    }

    else if (memory[PC] == 0x63)
    {
      operandA = memory[PC + 1] >> 4;
      operandB = memory[PC + 1] & 0x0F;
      printf("xorl %s, %s\n", register_names[operandA], register_names[operandB]);
      PC += 1;
    }

    else if (memory[PC] == 0x73)
    {
      printf("je %u\n", unsignedMemory);
      PC += 4;
    } 

    //this if statement checks if the intruction has reached the end of the file, if it has it will break the while loop.
    if (memory[PC] == 0x90 && memory[PC + 1] == 0x00 && memory[PC + 2] == 0x0D)
    {
      break;
    }
    PC++;
  }

  return 0;
}

//function which converted the memory location or immediate values into decimal.
int immedValue(char memLocation1, char memLocation2, char memLocation3, char memLocation4)
{

  signed int signedValue =
      (memLocation4 << 24) | (memLocation3 << 16) |
      (memLocation2 << 8) | memLocation1;

  return signedValue;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function reads in a y86 machine bytecode from file and
store them in an unsigned char array.
******************************************************************************/
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize)
{
  unsigned int bytes_read = 0;

  unsigned int file_size = 0;

  FILE *pfile = fopen(filename, "rb");

  if (!pfile)
  {
    printf("Unable to load file %s, please check if the path and name are correct.\n", filename);
    return 0;
  }

  fseek(pfile, 0, SEEK_END);
  file_size = ftell(pfile);
  rewind(pfile);

  if (file_size > memsize)
  {
    printf("Program size exceeds memory size of %d.\n", memsize);
    return 0;
  }

  bytes_read = fread(memory, 1, file_size, pfile);

  if (bytes_read != file_size)
  {
    printf("Bytes read does not match the file size.\n");
    return 0;
  }

  fclose(pfile);

  return bytes_read;
}
