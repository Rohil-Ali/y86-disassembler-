#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is an array of register mnemonics in y86
const char *register_names[] = {"%eax", "%ecx", "%edx",
                                "%ebx", "%esp", "%ebp",
                                "%esi", "%edi", "UNKNOWN_REGSITER"};

int convertStrToByteCode(const char *str, unsigned char inst[], int size);
void disassembleOpcode(unsigned char opcode, unsigned char operandA,
                       unsigned char operandB);

int main(int argc, char **argv) {
  FILE *pFile = NULL;

  char buffer[15];

  if (argc < 2) {
    pFile = fopen("./test2.txt", "r");
  } else {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL) {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1) {
    // This unsigned char array stores an instruction read from the file
    // As the largest y86 instruction is 6 bytes, there are 6 unsigned char in
    // the array where each represents a byte.
    unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
    convertStrToByteCode(buffer, instruction, 6);

    // TODO: From here, your task is to complete the implementation so that all
    // y86 opcode and operands can be disassembled. Any undisassembled opcode
    // should display as "TODO: undisassembled opcode and operands"
    // printf("TODO: undisassembled opcode and operands. The first byte of the
    // instruction is 0x%X\n", instruction[1]);

    // variables that store the register id values
    int operandA = 0;
    int operandB = 0;
    operandA = instruction[1] >> 4;
    operandB = instruction[1] & 0x0F;
    
    // Decodes immediate values and stores them into a variable.
    signed int signedImmediateValue =
        (instruction[5] << 24) | (instruction[4] << 16) |
        (instruction[3] << 8) | instruction[2];

    // Decodes memory address and stores them into a variable.
    unsigned int unsignedMemory = 
    (instruction[4] << 24) | (instruction[3] << 16) |
    (instruction[2] << 8) | instruction[1];

    //This function will check for the hexcode in instruction[0], it will then execute the   corrosponding switch case and print out its assembly version.
    switch (instruction[0]) {
    case 0x10:
      printf("nop\n");
      break;
    case 0x90:
      printf("ret\n");
      break;
    case 0x60:
      printf("addl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x62:
      printf("andl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x61:
      printf("subl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x63:
      printf("xorl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x20:
      printf("rrmovl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x21:
      printf("cmovle %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x23:
      printf("cmove %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x22:
      printf("cmovl %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x24:
      printf("cmovne %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x26:
      printf("cmovg %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0x25:
      printf("cmovge %s, %s\n", register_names[operandA],
             register_names[operandB]);
      break;
    case 0xA0:
      printf("pushl %s\n", register_names[operandA]);
      break;
    case 0xB0:
      printf("popl %s\n", register_names[operandA]);
      break;
    case 0x00:
      printf("halt ");
      break;
    case 0x30:
      printf("irmovl $%d, %s\n", signedImmediateValue,
             register_names[operandB]);
      break;
    case 0x50:
      printf("mrmovl %d(%s), %s\n", signedImmediateValue,
             register_names[operandB], register_names[operandA]);
      break;
    case 0x40:
      printf("rmmovl %s, %d(%s)\n", register_names[operandA],
             signedImmediateValue, register_names[operandB]);
      break;
    case 0x74:
      printf("jne %u\n", unsignedMemory);
      break;
    case 0x73:
      printf("je %u\n", unsignedMemory);
      break;
    case 0x80:
      printf("call %u\n", unsignedMemory);
      break;
    case 0x70:
      printf("jmp %u\n", unsignedMemory);
      break;
    case 0x75:
      printf("jge %u\n", unsignedMemory);
      break;
    case 0x72:
      printf("jl %u\n", unsignedMemory);
      break;
    case 0x71:
      printf("jle %u\n", unsignedMemory);
      break;
    case 0x76:
      printf("jg %u\n", unsignedMemory);
      break;
    default:
      printf("TODO: undisassembled opcode. The first byte of the instruction "
             "is 0x%X\n",
             instruction[0]);
      break;
    }
  }

  fclose(pFile);

  return 0;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char *str, unsigned char inst[], int size) {
  int numHexDigits = 0;
  char *endstr;
  // Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  // Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0) {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  // Return the size of the instruction in bytes
  return numHexDigits >> 1;
}