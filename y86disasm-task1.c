#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convertStrToByteCode(const char *str, unsigned char inst[], int size);
void disassembleOpcode(unsigned char opcode);

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];

  if (argc < 2)
  {
    //to test 'test2' switch the file name to test2.txt
    pFile = fopen("./test1.txt", "r");
  }
  else
  {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL)
  {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1)
  {
    //This unsigned char array stores an instruction read from the file
    //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
    //each represents a byte.
    unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
    convertStrToByteCode(buffer, instruction, 6);

//TODO: From here, your task is to complete the implementation so that all y86 opcodes can be disassembled.
//Any undisassembled opcode should display as "TODO: undisassembled opcode"
    disassembleOpcode(instruction[0]);
  }

  fclose(pFile);

  return 0;
}
//This function will check for the hexcode in instruction[0], it will then execute the corrosponding switch case and print out its assembly version.
void disassembleOpcode(unsigned char opcode)
{
    switch (opcode)
    {
      //Dissasemble for Test1 
    case 0x10:
        printf("nop\n");
        break;
    case 0x90:
        printf("ret\n");
        break;
    case 0x60:
        printf("addl\n");
        break;
    case 0x62:
        printf("andl\n");
        break;
    case 0x61:
        printf("subl\n");
        break;
    case 0x63:
        printf("xorl\n");
        break;
    case 0x20:
        printf("rrmovl\n");
        break;
    case 0x21:
        printf("cmovle\n");
        break;
    case 0x23:
        printf("cmove\n");
        break;
    case 0x22:
        printf("cmovl\n");
        break;
    case 0x24:
        printf("cmovne\n");
        break;
    case 0x26:
        printf("cmovg\n");
        break;
    case 0x25:
        printf("cmovge\n");
        break;
    case 0xA0:
        printf("pushl\n");
        break;
    case 0xB0:
        printf("popl\n");
        break;
    case 0x00:
        printf("halt\n");
        break;
      //Dissasemble for Test2
    case 0x30:
        printf("irmovl\n");
        break;
    case 0x50:
        printf("mrmovl\n");
        break;
    case 0x40:
        printf("rmmovl\n");
        break;
    case 0x74:
        printf("jne\n");
        break;
    case 0x73:
        printf("je\n");
        break;
    case 0x80:
        printf("call\n");
        break;
    case 0x70:
        printf("jmp\n");
        break;
    case 0x75:
        printf("jge\n");
        break;
    case 0x72:
        printf("jl\n");
        break;
    case 0x71:
        printf("jle\n");
        break;
    case 0x76:
        printf("jg\n");
        break;
    default:
        printf("TODO: undisassembled opcode. The first byte of the instruction is 0x%X\n", opcode);
        break;
    }
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char *str, unsigned char inst[], int size)
{
  int numHexDigits = 0;
  char *endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0)
  {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}