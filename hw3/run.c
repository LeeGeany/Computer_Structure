/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
	/** Implement this function */
void process_instruction()
{
    instruction * INSTR = get_inst_info(CURRENT_STATE.PC);

    if(OPCODE(INSTR) == 0x0)
    {
        if(FUNC(INSTR) == 0x21){ // ADDU
        //     printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = CURRENT_STATE.REGS[RT(INSTR)] + CURRENT_STATE.REGS[RS(INSTR)];}
        else if(FUNC(INSTR) == 0x24) { //AND
           //  printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));  
            CURRENT_STATE.REGS[RD(INSTR)] = CURRENT_STATE.REGS[RT(INSTR)] & CURRENT_STATE.REGS[RS(INSTR)];}
        else if(FUNC(INSTR) == 0x27){ //NOR
            // printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = ~(CURRENT_STATE.REGS[RT(INSTR)] | CURRENT_STATE.REGS[RS(INSTR)]);}
        else if(FUNC(INSTR) == 0x25){ //OR
             //printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = CURRENT_STATE.REGS[RT(INSTR)] | CURRENT_STATE.REGS[RS(INSTR)];}
        else if(FUNC(INSTR) == 0X2b){ //SLTU
           //  printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = (CURRENT_STATE.REGS[RT(INSTR)] < CURRENT_STATE.REGS[RS(INSTR)]);}
        else if(FUNC(INSTR) == 0x00){ //SLL
          //   printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = (CURRENT_STATE.REGS[RT(INSTR)] << SHAMT(INSTR));}
        else if(FUNC(INSTR) == 0x02){ //SRL
          //   printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = (CURRENT_STATE.REGS[RT(INSTR)] >> SHAMT(INSTR));}
        else if(FUNC(INSTR) == 0x23){ //SUBU
             //printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.REGS[RD(INSTR)] = CURRENT_STATE.REGS[RS(INSTR)] - CURRENT_STATE.REGS[RT(INSTR)];}
        else if(FUNC(INSTR) == 0x08){ //JR
             //printf("OP: %d. RS: %d,  RT: %d, RD: %d, SHAMT: %d, FUNC : %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), RD(INSTR),SHAMT(INSTR), FUNC(INSTR));
            CURRENT_STATE.PC = CURRENT_STATE.REGS[RS(INSTR)] - 4;}
    }   
    else if(OPCODE(INSTR) == 0x9){ //ADDIU
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        CURRENT_STATE.REGS[RT(INSTR)] = CURRENT_STATE.REGS[RS(INSTR)] + IMM(INSTR);}
    else if(OPCODE(INSTR) == 0xc){ //ANDI
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        CURRENT_STATE.REGS[RT(INSTR)] = CURRENT_STATE.REGS[RS(INSTR)] & IMM(INSTR);}
    else if(OPCODE(INSTR) == 0xf){ // LA / LUI
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        CURRENT_STATE.REGS[RT(INSTR)] =  (IMM(INSTR) << 16);}
    else if(OPCODE(INSTR) == 0xd){ //  LA / ORI
        // printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        if(CURRENT_STATE.REGS[RT(INSTR)] > 0)
            CURRENT_STATE.REGS[RT(INSTR)] += (IMM(INSTR));
        else
            CURRENT_STATE.REGS[RT(INSTR)] = (CURRENT_STATE.REGS[RS(INSTR)]) + IMM(INSTR);
    }
    else if(OPCODE(INSTR) == 0xb){ //SLTIU
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        CURRENT_STATE.REGS[RT(INSTR)] = (CURRENT_STATE.REGS[RS(INSTR)] < IMM(INSTR)) ? 1 : 0;}
    else if(OPCODE(INSTR) == 0x23){ //LW
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        CURRENT_STATE.REGS[RT(INSTR)] = mem_read_32(CURRENT_STATE.REGS[RS(INSTR)] + IMM(INSTR)); }
    else if(OPCODE(INSTR) == 0x2b){ //SW
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        mem_write_32(CURRENT_STATE.REGS[RT(INSTR)], CURRENT_STATE.REGS[RS(INSTR)]); }
    else if(OPCODE(INSTR) == 0x4){ //BEQ
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        if(CURRENT_STATE.REGS[RS(INSTR)] == CURRENT_STATE.REGS[RT(INSTR)])
            CURRENT_STATE.PC = CURRENT_STATE.PC  + (IMM(INSTR)*4);}
    else if(OPCODE(INSTR) == 0x5){ //BNE
         //printf("OP: %d. RS: %d,  RT: %d, IMM: %d\n",OPCODE(INSTR), RS(INSTR),RT(INSTR), IMM(INSTR));
        if(CURRENT_STATE.REGS[RS(INSTR)] != CURRENT_STATE.REGS[RT(INSTR)])
            CURRENT_STATE.PC = CURRENT_STATE.PC  + (IMM(INSTR)*4);}
            
    else if(OPCODE(INSTR) == 0x2){ //J
         //printf("OP: %d, J: %d\n", OPCODE(INSTR), TARGET(INSTR));
        CURRENT_STATE.PC = TARGET(INSTR)*4;}
    else if(OPCODE(INSTR) == 0x3){ //JAL
        //printf("OP: %d, J: %d\n", OPCODE(INSTR), TARGET(INSTR));
        CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 8; CURRENT_STATE.PC =  TARGET(INSTR)*4 - 4;}

    CURRENT_STATE.PC += 4;

    if(CURRENT_STATE.PC == MEM_TEXT_START + (NUM_INST*4)){RUN_BIT = FALSE;}
}
