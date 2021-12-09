/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);
int binary_to_integer_l(BIT* A, int length);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
/* convert integer to 2's complement BIT representation */
{
  // Note: A[0] is least significant bit and A[31] is most significant bit
  convert_to_binary_char(a, A, length);
  for (int i = 0; i < length; ++i)
      A[i] = (A[i] == '1' ? TRUE : FALSE); 
}

void convert_to_binary_char(int a, char* A, int length)
/*  converting integer->2's complement binary */
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}

//1-bit adder
void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  *CarryOut = or_gate3(and_gate(A,B),and_gate(A,CarryIn),and_gate(B,CarryIn));
  *Sum = or_gate3(and_gate(and_gate(A,B),CarryIn),and_gate(xor_gate(A,B), not_gate(CarryIn)),and_gate(xor_gate(A,CarryIn), not_gate(B)));
}

//1-bit ALU
void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  BIT temp = multiplexor2(Binvert, B, not_gate(B));
  adder1(A, temp, CarryIn, CarryOut, &temp);
  *Result = multiplexor4(Op0, Op1, and_gate(A,B), or_gate(A,B), temp, Less);
  *Set = temp;
}

void decoder3(BIT* I, BIT EN, BIT* O)
{
  //3-to-8 decoder using gates
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);
  
  O[0] = and_gate(EN, O[0]);
  O[1] = and_gate(EN, O[1]);
  O[2] = and_gate(EN, O[2]);
  O[3] = and_gate(EN, O[3]);
  O[4] = and_gate(EN, O[4]);
  O[5] = and_gate(EN, O[5]);
  O[6] = and_gate(EN, O[6]);
  O[7] = and_gate(EN, O[7]);
  
  return;
}

void decoder5(BIT* I, BIT* O)
{
   //5-to-32 decoder using 2-to-4 and 3-to-8 decoders
   BIT EN[4] = {FALSE};
   decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
   decoder3(I, EN[3], &O[24]);
   decoder3(I, EN[2], &O[16]);
   decoder3(I, EN[1], &O[8]);
   decoder3(I, EN[0], &O[0]);
}




/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

void set_register(char* input, char* output)
/* convert register name to 5 bit instruction code */
{
// suppress -Wall warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"

  if (strcmp(input, "zero") == 0)
    strncpy(output, "00000", 5);
  else if (strcmp(input, "v0") == 0)
    strncpy(output, "01000", 5);
  else if (strcmp(input, "a0") == 0)
    strncpy(output, "00100", 5);
  else if (strcmp(input, "t0") == 0)
    strncpy(output, "00010", 5);
  else if (strcmp(input, "t1") == 0)
    strncpy(output, "10010", 5);
  else if (strcmp(input, "s0") == 0)
    strncpy(output, "00001", 5);
  else if (strcmp(input, "s1") == 0)
    strncpy(output, "10001", 5);
  else if (strcmp(input, "sp") == 0)
    strncpy(output, "10111", 5);
  else if (strcmp(input, "ra") == 0)
    strncpy(output, "11111", 5);

#pragma GCC diagnostic pop
}

int get_instructions(BIT Instructions[][32])
/* performs conversion of instructions to binary */
/* Output - Convert instructions to binary in the instruction memory */
/* Returns the total number of instructions */
{
// suppress -Wall warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"

  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {        
    char inst[256] = {0};
    char op1[256] = {0};
    char op2[256] = {0};
    char op3[256] = {0};

    // Use sscanf on line to get strings for instruction and registers
    sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
    
    char temp_output[33] = {0};
    char rs[6] = {0};
    char rt[6] = {0};
    char rd[6] = {0};
    char imm[17] = {0};
    char address[27] = {0};
    
    if (strcmp(inst, "lw") == 0 ||
        strcmp(inst, "sw") == 0 ||
        strcmp(inst, "beq") == 0 ||
        strcmp(inst, "addi") == 0) {
      /* Convert immediate field and jump address field to binary */
      convert_to_binary_char(atoi(op3), imm, 16);
      /* Use registers to get rt and rs fields */
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      /* Set the correct instruction opcodes */
      if (strcmp(inst, "lw") == 0) { 
        strncpy(&temp_output[26], "110001", 6);      
      } else if (strcmp(inst, "sw") == 0) { 
        strncpy(&temp_output[26], "110101", 6);      
      } else if (strcmp(inst, "beq") == 0) { 
        strncpy(&temp_output[26], "001000", 6);      
      } else if (strcmp(inst, "addi") == 0) { 
        strncpy(&temp_output[26], "000100", 6);      
      }
    } else if (strcmp(inst, "and") == 0 ||
               strcmp(inst, "or") == 0 ||
               strcmp(inst, "add") == 0 ||
               strcmp(inst, "sub") == 0 ||
               strcmp(inst, "slt") == 0 ||
               strcmp(inst, "jr") == 0) {
      /* R-type instruction format */
      /* Use registers to get rt, rd, rs fields */
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      /* set the correct instruction opcodes */
      if (strcmp(inst, "and") == 0) { 
        strncpy(&temp_output[0], "001001", 6);
      } else if (strcmp(inst, "or") == 0) { 
        strncpy(&temp_output[0], "101001", 6);
      } else if (strcmp(inst, "add") == 0) { 
        strncpy(&temp_output[0], "000001", 6);
      } else if (strcmp(inst, "sub") == 0) { 
        strncpy(&temp_output[0], "010001", 6);
      } else if (strcmp(inst, "slt") == 0) { 
        strncpy(&temp_output[0], "010101", 6);
      } else if (strcmp(inst, "jr") == 0) { 
        strncpy(&temp_output[0], "000100", 6);
      }
      /* set the correct instruction funct */
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], "000000", 6);      
    } else if (strcmp(inst, "j") == 0 ||
               strcmp(inst, "jal") == 0) { 
      /* J-type instruction format */
      /* Convert immediate field and jump address field to binary */
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      /* set the correct instruction opcodes */
      if (strcmp(inst, "j") == 0) {
        strncpy(&temp_output[26], "010000", 6);      
      } else if (strcmp(inst, "jal") == 0) {
        strncpy(&temp_output[26], "110000", 6);      
      }
    }
    
    for (int i = 0; i < 32; ++i)
      Instructions[instruction_count][i] = (temp_output[i] == '1' ? TRUE : FALSE); 
    
    // incremement instruction counter
    instruction_count++;
  }
  
#pragma GCC diagnostic pop
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here

    /** PC is project which is a binary num
     Q1. ReadAddress = PC
    
  Q2. What is PC?
    - 32 bits
    - (if PC = 0,1,2,...n) can do a conver to binary to get decimal -> then use MEM_instruction[decimal_PC] to get Instruction
  **/
  
  BIT decoded[32]={FALSE};
  BIT inp[5]={FALSE};
  int i=0;
  for(i=0;i<5;i++){
    inp[i]=ReadAddress[i];  
    // printf("%d RESULT[i=%d]\n",ReadAddress[i],i ); // for debugging
  }
  
  decoder5(inp,decoded);
  // print_binary(decoded); // for debugging
  // printf("\n"); // for debugging
  int instr_mem_arr_add = 0;
  i=31;
  while(i>=0){
    instr_mem_arr_add += i*decoded[i];
    // printf("imad = %d, j=%d decoded[i=%d]=%d\n",instr_mem_arr_add,j,i,decoded[i]); // for debugging
    // printf("i %d instr_mem_arr_add %d \n",i, instr_mem_arr_add ); // for debugging
    --i;
  }

  // printf("instr_mem_arr_add = %d\n",instr_mem_arr_add); // for debugging
  copy_bits(MEM_Instruction[instr_mem_arr_add],Instruction);
  // print_binary(Instruction); // for debugging
  // printf("\n"); // for debugging

  
}
/* Truth Table:
 Operation | Opcode | RegDst | Jump | Branch | MemRead | MemToReg | ALUOp | MemWrite | ALUSrc | RegWrite
 R-Type    | 000000 |    1   |   0  |    0   |    0    |     0    |   10  |    0     |    0   |    1
 Load Word | 110001 |    0   |   0  |    0   |    1    |     1    |   00  |    0     |    1   |    1
 S. Word   | 110101 |    0   |   0  |    0   |    0    |     0    |   00  |    1     |    1   |    0
 Branch Eq | 001000 |    0   |   0  |    1   |    0    |     0    |   01  |    0     |    0   |    0
 Addi      | 000100 |    0   |   0  |    0   |    0    |     0    |   00  |    0     |    1   |    1
 Jump      | 010000 |    0   |   1  |    0   |    0    |     0    |   00  |    0     |    0   |    0
 J & Link  | 110000 |    0   |   1  |    0   |    0    |     0    |   00  |    0     |    0   |    1
 Note: *Opcode in little-endian
*/
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits
	*RegDst = and_gate(and_gate3(not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2])), and_gate3(not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5])));
	*Jump = and_gate(OpCode[1], not_gate(OpCode[5]));
	*Branch = OpCode[2];
	*MemRead = and_gate3(OpCode[1], not_gate(OpCode[3]), OpCode[5]);
	*MemToReg = and_gate3(OpCode[1], not_gate(OpCode[3]), OpCode[5]);
	ALUOp[0] = and_gate(and_gate3(not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2])), and_gate3(not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5])));
	ALUOp[1] = OpCode[2];
	*MemWrite = and_gate(OpCode[1], OpCode[3]);
	*ALUSrc = or_gate(OpCode[3], OpCode[5]);
	*RegWrite = or_gate(and_gate(not_gate(OpCode[1]), not_gate(OpCode[2])), and_gate(not_gate(OpCode[3]), OpCode[0]));
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  BIT decoded1[32] = {FALSE};
  BIT decoded2[32] = {FALSE};
  decoder5(ReadRegister1, decoded1);
  decoder5(ReadRegister2, decoded2);
  int binary1 = binary_to_integer(decoded1);
  int binary2 = binary_to_integer(decoded2);
  copy_bits(MEM_Register[binary1], ReadData1);
  copy_bits(MEM_Register[binary2], ReadData2);
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
/*  
   Implements register write functionality 
   Input: one 5-bit register address, data to write, and control bit
   Output: None, but will modify register file
*/
{
  // convert register binary number to an integer
  BIT decoded[32] = {FALSE};
  decoder5(WriteRegister, decoded);
  int register_number = binary_to_integer(decoded); 

  // call multiplexor to put WriteData into Register Memory
  // if the RegWrite control bit is set
  multiplexor2_32(RegWrite,  MEM_Register[register_number], WriteData, 
                      MEM_Register[register_number]);

}

/*
ALUOp = 00 -> ADD -> ALUControl = 0010
ALUOp = 01 -> SUB -> ALUControl = 0110
ALUOp = 10 -> R-Type:
funct = 000001 -> ADD -> ALUControl = 0010
funct = 010001 -> SUB -> ALUControl = 0110
funct = 001001 -> AND -> ALUControl = 0000
funct = 101001 -> OR  -> ALUControl = 0001
funct = 010101 -> SLT -> ALUControl = 0111
//NOTE: funct in little-endian
*/
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
  
  ALUControl[0] = 0; //We are not implementing NOR, this is always 0;
  ALUControl[1] = or_gate(ALUOp[1], and_gate(ALUOp[0], funct[1]));
  ALUControl[2] = or_gate(not_gate(ALUOp[0]), not_gate(funct[2]));
  ALUControl[3] = and_gate(ALUOp[0], or_gate(funct[0], funct[3]));
}

/*
ALUControl[0] = NOR
ALUControl[1] = Binvert/CarryIn
ALUControl[2],ALUControl[3] = Operation 
00 = AND
01 = OR
10 = ADD/SUB
11 = Less
*/
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  BIT Less = 0; //Carries the result of subtraction from ALU 31 to ALU 0 for Set Less Than
  BIT SET; //Used as a placeholder for ALUs 0-30
  BIT Carry; //Used to pass carryOut of an ALU to carryIn of the next

  //ALU 0 takes carrtIn from ALUControl
  ALU1(Input1[0], Input2[0], ALUControl[1], ALUControl[1], Less, ALUControl[3], ALUControl[2], Result, &Carry, &SET);
  *Zero = not_gate(Result[0]);
  
  //ALUs 1-30 are identical
  for(int i = 1; i < 31; i++)
  {
    ALU1(Input1[i], Input2[i], ALUControl[1], Carry, Less, ALUControl[3], ALUControl[2], Result+i, &Carry, &SET);
    *Zero = and_gate(*Zero, not_gate(Result[i])); //Zero will be true only if all the Result bits are 0
  }
  
  //ALU 31 sets the Less variable
  ALU1(Input1[31], Input2[31], ALUControl[1], Carry, Less, ALUControl[3], ALUControl[2], Result+31, &Carry, &Less);
  *Zero = and_gate(*Zero, not_gate(Result[31]));
  //Run ALU 0 one final time for Set Less Than
  ALU1(Input1[0], Input2[0], ALUControl[1], ALUControl[1], Less, ALUControl[3], ALUControl[2], Result, &Carry, &SET);
  *Zero = and_gate(*Zero, not_gate(Result[0]));
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
    /** W/out using if statements
    Step 1 - then covert Address to decimal_address then MEM_Data[decimal_Address]=WriteData or ReadData= 
    Step 2 - if MemWrite==1, (Do this w/out if??) {Use a mux??}
    Step 3 - If lw (i.e. MEMRead==1) Read Data = MEM_Instruction[decimal_Address] {USE a mux}

  **/

  //get what loc in array
  BIT OutData[32]={FALSE};
  BIT RData[32]={FALSE};
  BIT decoded[32]={FALSE};
  BIT inp[5]={FALSE};
  int i=0;
  for(i=0;i<5;i++){
    inp[i]=Address[i];  
    // printf("%d ADDRESS[i=%d]\n",Address[i],i );
  }
  
  decoder5(inp,decoded);
  // print_binary(decoded); // for debugging
  // printf("\n"); // for debugging
  int instr_mem_arr_add = 0;
  // int j =0;
  i=31;
  while(i>=0){
    instr_mem_arr_add += i*decoded[i];
    // printf("imad = %d, j=%d decoded[i=%d]=%d\n",instr_mem_arr_add,j,i,decoded[i]);  // for debugging
    --i;
  
  }
  // printf("instr_mem_arr_add = %d\n",instr_mem_arr_add);  // for debugging

  //if MEMWrite == 0 then, write the existing value again
  multiplexor2_32(MemWrite,MEM_Data[instr_mem_arr_add],WriteData,OutData); 
  copy_bits(OutData,MEM_Data[instr_mem_arr_add]);



  //if MemRead==0, returns ZERO but shouldn't matter as MemToReg is not going to take this as an input
  multiplexor2_32(MemRead,ZERO,MEM_Data[instr_mem_arr_add],RData);
  copy_bits(RData,ReadData);
  
  
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  for (int i=0;i<16;i++) {
    Output[i] = Input[i];
  }
  for (int i=0;i<16;i++) {
    Output[i+16] = Input[15];
  }
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction
  
  //Fetch
  BIT instruction[32] = {FALSE}; // the output variable
  Instruction_Memory(PC, instruction); //to get instruction
  int tempPC = binary_to_integer(PC); // convert PC to int add 1
  tempPC++; 
  convert_to_binary_char(tempPC, PC, 32); // and then convert back to binary

  //Decode
  BIT opcode[6] = {FALSE};  // the input variable
  for (int i = 26; i < 32; i++){
    opcode[i-26] = instruction[i];
  }
  Control(opcode, &RegDst, &Jump, &Branch, &MemRead, &MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite); //determine inputs from the instruction, no ifs (use ands)
  BIT rReg1[5] = {FALSE}, rReg2[5] = {FALSE}, rData1[32] = {FALSE}, rData2[32] = {FALSE};
  Read_Register(rReg1, rReg2, rData1, rData2);
  BIT immInstr[16] = {FALSE}; // input for sign extend
  for (int i = 0; i < 16; i++){
    immInstr[i] = instruction[i];
  }
  BIT signExtended[32] = {FALSE};
  Extend_Sign16(immInstr, signExtended); //address to go to if branch

  //Execute
  BIT func[6] = {FALSE}; // input function variable
  for (int i = 0; i < 6; i++){
    func[i] = instruction[i];
  }
  BIT ALUIn2[32] = {FALSE};
  BIT ALURes[32] = {FALSE};
  ALU_Control(ALUOp, func, ALUControl);
  multiplexor2_32(ALUSrc, rData2, signExtended, ALUIn2);
  ALU(ALUControl, rData1, ALUIn2, &Zero, ALURes);
  //getting the writeregister for right back later for write back later
  BIT WriteReg[5] = {FALSE}, WriteData1[5] = {FALSE}, WriteData2[5] = {FALSE};
  for (int i = 0; i < 6; i++){
    WriteData1[i] = instruction[i+16];
    WriteData2[i] = instruction[i+11];
  }
  for (int i = 0; i < 6; i++){
    WriteReg[i] = multiplexor2(RegDst, WriteData1[i], WriteData2[i]);
  }
  //for jal
  BIT JalDst = and_gate(Jump, RegWrite);
  BIT raReg[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
  multiplexor2_32(JalDst, WriteReg, raReg, WriteReg);

  //Memory
  //load and stuff
  BIT rData[32] = {FALSE};
  Data_Memory(MemWrite, MemRead, ALURes, rData2, rData);
  //for if you branch or not
  BIT PCSrc = and_gate(Branch, Zero);
  BIT BranchMuxRes[32] = {FALSE};
  multiplexor2_32(PCSrc, PC, signExtended, BranchMuxRes);
  BIT JumpDest[32] = {FALSE}; //the address of the destination of jump
  //last 4bits of PC+4(1 in this case) + the first 26bits of instruction + 00
  for (int i = 0; i < 3; i++){
    JumpDest[i+28] = PC[i+28];
  }
  for (int i = 0; i < 26; i++){
    JumpDest[i+2] = instruction[i];
  }
  JumpDest[0] = FALSE, JumpDest[1] = FALSE;
  multiplexor2_32(Jump, BranchMuxRes, JumpDest, PC);
  //JrDec == func == 000010, and then mux if it is 1
  BIT JrDec = and_gate3(RegDst, and_gate3(not_gate(func[0]), func[1], not_gate(func[2])), and_gate3(not_gate(func[3]), not_gate(func[4]), not_gate(func[5])));
  multiplexor2_32(JrDec, PC, rData1, PC);

  //Write Back
  BIT WriteData[32] = {FALSE};
  multiplexor2_32(MemToReg, ALURes, rData, WriteData);
  multiplexor2_32(JalDst, WriteData, PC, WriteData);
  Write_Register(RegWrite, WriteReg, WriteData);

  //Update PC
  //did this in other places throughout the code
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}
