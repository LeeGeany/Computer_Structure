/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include "util.h"
#include "parse.h"

int text_size;
int data_size;

/* Implement this function */
instruction parsing_instr(const char *buffer, const int index )
{
    instruction instr;
    char opcode[7]; char rs[6]; char rt[6]; char imm[17]; char rd[6]; char shamt[6]; char func_code[7]; char target[27];
    
    strncpy(opcode, buffer, 6); opcode[6] = NULL;
    instr.value = MEM_TEXT_START + index ; instr.opcode = (short)fromBinary(opcode);
    mem_write_32( MEM_TEXT_START + index, fromBinary(buffer));
    switch(instr.opcode)
    {
        //Type I
        case 0x9:		//(0x001001)ADDIU
        case 0xc:		//(0x001100)ANDI
        case 0xf:		//(0x001111)LUI	
        case 0xd:		//(0x001101)ORI
        case 0xb:		//(0x001011)SLTIU
        case 0x23:		//(0x100011)LW
        case 0x2b:		//(0x101011)SW
        case 0x4:		//(0x000100)BEQ
        case 0x5:		//(0x000101)BNE
            strncpy(rs, buffer + 6, 5); rs[5] = NULL;
            strncpy(rt, buffer + 11, 5); rt[5] = NULL;
            strncpy(imm, buffer + 16, 17); imm[16] = NULL;

            instr.r_t.r_i.rs = fromBinary(rs);
            instr.r_t.r_i.rt = fromBinary(rt);
            instr.r_t.r_i.r_i.imm = fromBinary(imm);
            break;

        //TYPE R
        case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR       
            strncpy(rs, buffer + 6, 5); rs[5] = NULL;
            strncpy(rt, buffer + 11, 5); rt[5]=NULL;
            strncpy(rd, buffer + 16, 5); rd[5] = NULL;
            strncpy(shamt, buffer + 21, 5); shamt[5] = NULL;
            strncpy(func_code, buffer + 26, 6); func_code[6] = NULL;

            instr.r_t.r_i.rs = fromBinary(rs);
            instr.r_t.r_i.rt = fromBinary(rt);
            instr.r_t.r_i.r_i.r.rd = fromBinary(rd);
            instr.r_t.r_i.r_i.r.shamt = fromBinary(shamt);
            instr.func_code = fromBinary(func_code);
            break;

        //TYPE J
        case 0x2:		//(0x000010)J
        case 0x3:		//(0x000011)JAL      
            strncpy(target, buffer+6, 27); target[26] = NULL;
            instr.r_t.target = fromBinary(target);
            break;

        default:
            printf("Not available instruction\n");
            assert(0);
    }
    return instr;
}

void parsing_data(const char *buffer, const int index)
{
    mem_write_32(MEM_DATA_START + index, fromBinary(buffer) );
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
