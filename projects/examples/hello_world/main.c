/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     main.c
 * @brief    hello world
 * @version  V1.0
 * @date     17. Jan 2018
 ******************************************************************************/
#include "soc.h"
#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define CANMOD_MASK                  0X1F
#define CANIER_MASK                  0XFF
#define CANBTR0_MASK                 0XDF
#define CANBTR1_MASK                 0XFF
#define CANOCR_MASK                  0X03





#define CANMOD                       0x00  //RW
#define CANCMR                       0x04
#define CANSR                        0x08
#define CANIR                        0x0C
#define CANIER                       0x10
#define CANBTR0                      0x14
#define CANBTR1                      0x18
#define CANOCR                       0x1C
#define CANALC                       0x20
#define CANECC                       0x24
#define CANEWLR                      0x28
#define CANRXERR                     0x2C
#define CANTXERR                     0x30
#define CANTRANSMIT_BUFFER_W0        0x40
#define CANTRANSMIT_BUFFER_W1        0x44
#define CANTRANSMIT_BUFFER_W2        0x48
#define CANTRANSMIT_BUFFER_W3        0x4C
#define CANTRANSMIT_BUFFER_R0        0x50
#define CANTRANSMIT_BUFFER_R1        0x54
#define CANTRANSMIT_BUFFER_R2        0x58
#define CANTRANSMIT_BUFFER_R3        0x5C
#define CANRECEIVE_BUFFER_R0         0x40  // RO
#define CANRECEIVE_BUFFER_R1         0x44  // RO
#define CANRECEIVE_BUFFER_R2         0x48  // RO
#define CANRECEIVE_BUFFER_R3         0x4C  // RO
#define CANACR                       0x60
#define CANRMC                       0x64
#define CANRBSA                      0x68
#define CANCDR                       0x6C

#define REG32(addr)                                 *((volatile unsigned int *)(addr))
#define REG32_CAN(_addr)                            *((volatile unsigned int *)(_addr+WJ_CAN_BASE))
//#define WRITE_REG32_CAN(_addr,_val)                do(*((volatile unsigned int *)(_addr+WJ_CAN_BASE))=(_val))while(0)
#define PRINT_REG32_CAN(_reg)                        do{printf("|%-25s|%#-7x|%#-7x|\n", #_reg, REG32(_reg+WJ_CAN_BASE), _reg);} while(0);
#define PRINT_REG32_CAN_WRITE(_reg,_val)             do{printf("reg:[%s] op:[W] val:[%#x]\n", #_reg, _val);} while (0)
#define GET_REG32_BIT(_idx, _val)                    (((_val & (1<<_idx)) >> _idx) & 0x1)
#define GET_REG32_MULTI_BIT(_bitmask, _idx, _val)    (((_val & (_bitmask<<_idx)) >> _idx) & _bitmask)


//#define WRITE_REG32_CAN(_reg,_val)        do {REG32_CAN(_reg) = _val; printf("reg:[%s] op:[W] val:[%#x]\n", #_reg, _val);} while (0)



typedef struct _reg_info_t {
    char     *field;
    char     *name;
    uint32_t  default_val;
    uint32_t  real_val;
    char     *access;
} reg_info_t;


typedef struct _reg_info_w_t {
    char     *field;
    char     *name;
    uint32_t  default_value;
    uint32_t  write;
    uint32_t  read;
    char     *access;
    char     *result;
} reg_info_w_t;

typedef struct _bit_field_t {
    uint32_t bit_0: 1;
    uint32_t bit_1: 1;
    uint32_t bit_2: 1;
    uint32_t bit_3: 1;
    uint32_t bit_4: 1;
    uint32_t bit_5: 1;
    uint32_t bit_6: 1;
    uint32_t bit_7: 1;
} bit_field_t;


// reg: write ---> read
typedef enum {
    CANMOD_bit4 = 0,
    CANMOD_bit3,
    CANMOD_bit2,
    CANMOD_bit1,
    CANMOD_bit0
} bit_CANMOD_e;


reg_info_w_t  reg_info_w_CANMOD[] = {
    [CANMOD_bit4] = {"4", "MOD[4]", 0, 0x0, 0x0, "W/R", NULL},
    [CANMOD_bit3] = {"3", "MOD[3]", 0, 0x0, 0x0, "W/R", NULL},
    [CANMOD_bit2] = {"2", "MOD[2]", 0, 0x0, 0x0, "W/R", NULL},
    [CANMOD_bit1] = {"1", "MOD[1]", 0, 0x0, 0x0, "W/R", NULL},
    [CANMOD_bit0] = {"0", "MOD[0]", 0, 0x0, 0x0, "W/R", NULL},
};


#if 0
typedef enum {
    CANCMR_bit4 = 0,
    CANCMR_bit3,
    CANCMR_bit2,
    CANCMR_bit1,
    CANCMR_bit0
} bit_CANCMR_e;

reg_info_w_t  reg_info_w_CANCMR[] = {
    [CANCMR_bit4] = {"4", "CMR[4]", 0, 0x0, 0x0, "WO", NULL},
    [CANCMR_bit3] = {"3", "CMR[3]", 0, 0x0, 0x0, "WO", NULL},
    [CANCMR_bit2] = {"2", "CMR[2]", 0, 0x0, 0x0, "WO", NULL},
    [CANCMR_bit1] = {"1", "CMR[1]", 0, 0x0, 0x0, "WO", NULL},
    [CANCMR_bit0] = {"0", "CMR[0]", 0, 0x0, 0x0, "WO", NULL},
};
#endif

//CANIER
//Name: CAN Interrupt Enable Register.
//Address offset: 0x010
typedef enum {
    CANIER_bit7 = 0,
    CANIER_bit6,
    CANIER_bit5,
    CANIER_bit4,
    CANIER_bit3,
    CANIER_bit2,
    CANIER_bit1,
    CANIER_bit0
} bit_CANIER_e;

reg_info_w_t  reg_info_w_CANIER[] = {
    [CANIER_bit7] = {"7", "IER[7]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit6] = {"6", "IER[6]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit5] = {"5", "IER[5]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit4] = {"4", "IER[4]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit3] = {"3", "IER[3]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit2] = {"2", "IER[2]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit1] = {"1", "IER[1]", 0, 0x0, 0x0, "W/R", NULL},
    [CANIER_bit0] = {"0", "IER[0]", 0, 0x0, 0x0, "W/R", NULL},
};

//CANBTR0
//Name: CAN Bus Timing Register 0.
//Address offset: 0x014
typedef enum {
    CANBTR0_bit7_6 = 0,
    CANBTR0_bit4_0
} bit_CANBTR0_e;
reg_info_w_t  reg_info_w_CANBTR0[] = {
    [CANBTR0_bit7_6] = {"7:6", "SJW[1:0]", 0, 0x0, 0x0, "W/R", NULL},
    [CANBTR0_bit4_0] = {"4:0", "BRP[4:0]", 0, 0x0, 0x0, "W/R", NULL},
};

//CANBTR1 
//Name: CAN Bus Timing Register 1.
//Address offset: 0x018
typedef enum {
    CANBTR1_bit7   = 0,
    CANBTR1_bit6_4,
    CANBTR1_bit3_0
} bit_CANBTR1_e;
reg_info_w_t  reg_info_w_CANBTR1[] = {
    [CANBTR1_bit7]   = {"7",   "SAM",        0, 0x0, 0x0, "W/R", NULL},
    [CANBTR1_bit6_4] = {"6:4", "TSEG2[2:0]", 0, 0x0, 0x0, "W/R", NULL},
    [CANBTR1_bit3_0] = {"3:0", "TSEG1[3:0]", 0, 0x0, 0x0, "W/R", NULL},
};

typedef enum {
    CANOCR_bit1_0
} bit_CANOCR_e;

reg_info_w_t  reg_info_w_CANOCR[] = {
    [CANOCR_bit1_0] = {"1:0", "OCMODE", 0, 0x0, 0x0, "W/R", NULL},
};


// CANACR
// Name: CAN Acceptance Code Register.
// Address offset: 0x060
typedef enum {
    CANACR_bit7_4 = 0,
    CANACR_bit3_0
} bit_CANACR_e;

reg_info_w_t  reg_info_w_CANACR[] = {
    [CANACR_bit7_4] = {"7:4", "AMR[1:0]", 0, 0x0, 0x0, "W/R", NULL},
    [CANACR_bit3_0] = {"3:0", "ACR[3:0]", 0, 0x0, 0x0, "W/R", NULL},
};


// Name: CAN Receive Buffer Start Address.
// Address offset: 0x068
typedef enum {
    CANRBSA_bit5_0 = 0
} bit_CANRBSA_e;

reg_info_w_t  reg_info_w_CANRBSA[] = {
    [CANRBSA_bit5_0] = {"5:0", "RBSA", 0xc0, 0x0, 0x0, "W/R", NULL},
};

// Name: CAN Clock Divider Register.
// Address offset: 0x06C
typedef enum {
    CANCDR_bit7_0 = 0
} bit_CANCDR_e;

reg_info_w_t  reg_info_w_CANCDR[] = {
    [CANCDR_bit7_0] = {"7:0", "CDR", 0xc0, 0x0, 0x0, "W/R", NULL},
};



static void show_all_reg();
static void show_reg_info_CANMOD(uint32_t reg_val);
static void print_reg_info_read(reg_info_t *p_reg_info, uint32_t reg_info_len);
static void print_reg_info_write(reg_info_w_t *reg_info_w, uint32_t reg_info_w_len);
static void write_all_reg(uint32_t reg_write);
static void wr_reg_info_CANMOD(uint32_t reg_write);
static void wr_reg_info_CANIER(uint32_t reg_write);


int main(void)
{
    int i;
    for (i = 0; i < 1; i++) {
        printf("Hello my friends!\n");
    }

    //show_all_reg();
    //show_reg_info_CANMOD(REG32_CAN(CANMOD));



    //WRITE_REG32_CAN(CANMOD, 0xffffffff);
    //wr_reg_info_CANMOD

    //wr_reg_info_CANMOD(0xffffffff);

    //show_reg_info_CANMOD(REG32_CAN(CANMOD));
    write_all_reg(0xffffffff);
    show_all_reg();
    return 0;
}




static void show_all_reg()
{
    printf("CAN register as follows:\naddr base:%#lx\n", WJ_CAN_BASE);
    printf("|-------------------------|-------|-------|\n");
    printf("|Name                     |Value  |Offset |\n");
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANMOD);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANCMR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANSR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANIR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANIER);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANBTR0);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANBTR1);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANOCR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANALC);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANECC);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANEWLR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRXERR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTXERR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_W0);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_W1);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_W2);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_W3);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_R0);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_R1);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_R2);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANTRANSMIT_BUFFER_R3);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRECEIVE_BUFFER_R0);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRECEIVE_BUFFER_R1);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRECEIVE_BUFFER_R2);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRECEIVE_BUFFER_R3);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANACR);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRMC);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANRBSA);
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG32_CAN(CANCDR);
    printf("|-------------------------|-------|-------|\n");
}

static void write_all_reg(uint32_t reg_write)
{
    printf("Write value:[%#x] to all CAN register:\naddr base:%#lx\n", reg_write, WJ_CAN_BASE);

    REG32_CAN(CANMOD) = reg_write;

    REG32_CAN(CANCMR) = reg_write;

    REG32_CAN(CANSR) = reg_write;

    REG32_CAN(CANIR) = reg_write;

    REG32_CAN(CANIER) = reg_write;

    REG32_CAN(CANBTR0) = reg_write;

    REG32_CAN(CANBTR1) = reg_write;

    REG32_CAN(CANOCR) = reg_write;

    REG32_CAN(CANALC) = reg_write;

    REG32_CAN(CANECC) = reg_write;

    REG32_CAN(CANEWLR) = reg_write;

    REG32_CAN(CANRXERR) = reg_write;

    REG32_CAN(CANTXERR) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_W0) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_W1) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_W2) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_W3) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_R0) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_R1) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_R2) = reg_write;

    REG32_CAN(CANTRANSMIT_BUFFER_R3) = reg_write;

    REG32_CAN(CANRECEIVE_BUFFER_R0) = reg_write;

    REG32_CAN(CANRECEIVE_BUFFER_R1) = reg_write;

    REG32_CAN(CANRECEIVE_BUFFER_R2) = reg_write;

    REG32_CAN(CANRECEIVE_BUFFER_R3) = reg_write;

    REG32_CAN(CANACR) = reg_write;

    REG32_CAN(CANRMC) = reg_write;

    REG32_CAN(CANRBSA) = reg_write;

    REG32_CAN(CANCDR) = reg_write;
}

static void show_reg_info_CANMOD(uint32_t reg_val)
{
    reg_info_t  reg_info_CANMOD[] = {
        [CANMOD_bit4] = {"4", "MOD[4]", 0x0, 0, "W/R"},
        [CANMOD_bit3] = {"3", "MOD[3]", 0x0, 0, "W/R"},
        [CANMOD_bit2] = {"2", "MOD[2]", 0x0, 0, "W/R"},
        [CANMOD_bit1] = {"1", "MOD[1]", 0x0, 0, "W/R"},
        [CANMOD_bit0] = {"0", "MOD[0]", 0x0, 0, "W/R"},
    };

    reg_info_CANMOD[CANMOD_bit0].real_val = GET_REG32_BIT(0, reg_val);
    reg_info_CANMOD[CANMOD_bit1].real_val = GET_REG32_BIT(1, reg_val);
    reg_info_CANMOD[CANMOD_bit2].real_val = GET_REG32_BIT(2, reg_val);
    reg_info_CANMOD[CANMOD_bit3].real_val = GET_REG32_BIT(3, reg_val);
    reg_info_CANMOD[CANMOD_bit4].real_val = GET_REG32_BIT(4, reg_val);
    printf("reg:[%s] offset:[%#x]\n", "CANMOD", CANMOD);
    print_reg_info_read(reg_info_CANMOD, ARRAY_SIZE(reg_info_CANMOD));
}


static void print_reg_info_read(reg_info_t *p_reg_info, uint32_t reg_info_len)
{
    int i;
//  printf("reg info: [%s] Offset:%#x\n", "CANMOD", CANMOD);
    printf("|------|------------|--------|--------|-------|\n");
    printf("|Field |Name        |Default |Real    |Access |\n");
    printf("|------|------------|--------|--------|-------|\n");
    for (i = 0; i < reg_info_len; i++) {
        printf("|%-6s|%-12s|%#-8x|%#-8x|%-7s|\n",
               p_reg_info[i].field,
               p_reg_info[i].name,
               p_reg_info[i].default_val,
               p_reg_info[i].real_val,
               p_reg_info[i].access);
        printf("|------|------------|--------|--------|-------|\n");
    }
}

static void print_reg_info_write(reg_info_w_t *reg_info_w, uint32_t reg_info_w_len)
{
    int i;
    printf("|------|----------|--------|------|------|------|---|\n");
    printf("|Field |Name      |Default |write |read  |Access|ret|\n");
    printf("|------|----------|--------|------|------|------|---|\n");
    for (i = 0; i < reg_info_w_len; i++) {
        printf("|%-6s|%-10s|%#-8x|%#-6x|%#-6x|%-6s|%-3s|\n",
               reg_info_w[i].field,
               reg_info_w[i].name,
               reg_info_w[i].default_value,
               reg_info_w[i].write,
               reg_info_w[i].read,
               reg_info_w[i].access,
               reg_info_w[i].result);
        printf("|------|----------|--------|------|------|------|---|\n");
    }
}

static void wr_reg_info_CANMOD(uint32_t reg_write)
{
    uint32_t i;
    uint32_t reg_read;
    REG32_CAN(CANMOD) = reg_write;
    reg_info_w_CANMOD[CANMOD_bit0].write = GET_REG32_BIT(0, reg_write);
    reg_info_w_CANMOD[CANMOD_bit1].write = GET_REG32_BIT(1, reg_write);
    reg_info_w_CANMOD[CANMOD_bit2].write = GET_REG32_BIT(2, reg_write);
    reg_info_w_CANMOD[CANMOD_bit3].write = GET_REG32_BIT(3, reg_write);
    reg_info_w_CANMOD[CANMOD_bit4].write = GET_REG32_BIT(4, reg_write);
    reg_read = REG32_CAN(CANMOD);
    reg_info_w_CANMOD[CANMOD_bit0].read  = GET_REG32_BIT(0, reg_read);
    reg_info_w_CANMOD[CANMOD_bit1].read  = GET_REG32_BIT(1, reg_read);
    reg_info_w_CANMOD[CANMOD_bit2].read  = GET_REG32_BIT(2, reg_read);
    reg_info_w_CANMOD[CANMOD_bit3].read  = GET_REG32_BIT(3, reg_read);
    reg_info_w_CANMOD[CANMOD_bit4].read  = GET_REG32_BIT(4, reg_read);

    if ((reg_read & CANMOD_MASK) != (reg_write & CANMOD_MASK)) {
        for (i = 0; i < ARRAY_SIZE(reg_info_w_CANMOD); i++) {
            if (strcmp(reg_info_w_CANMOD[i].access, "W/R") == 0) {
                reg_info_w_CANMOD[i].result = (reg_info_w_CANMOD[i].write != reg_info_w_CANMOD[i].read) ? "X" : "";
            }
        }
    }
    printf("reg:[%s] offset:[%#x]\n", "CANMOD", CANMOD);
    print_reg_info_write(reg_info_w_CANMOD, ARRAY_SIZE(reg_info_w_CANMOD));
    printf("write: [%#x]\nread:  [%#x]\nMASK:  [%#x]\nresult:[%s]\n\n", reg_write, reg_read, CANMOD_MASK, (reg_read & CANMOD_MASK) != (reg_write & CANMOD_MASK) ? "X" : "ok");
}

static void wr_reg_info_CANIER(uint32_t reg_write)
{
    uint32_t      i;
    uint32_t      reg_read;
    uint32_t      reg32        = CANIER;
    char         *p_reg32_str  = "CANIER";
    uint32_t      mask         = CANIER_MASK;
    reg_info_w_t *p_reg_info   = reg_info_w_CANIER;
    uint32_t      reg_info_len = ARRAY_SIZE(reg_info_w_CANIER);
    //-----------------------------------------------------------
    printf("reg:[%s] offset:[%#x]\n", p_reg32_str, reg32);
    REG32_CAN(reg32) = reg_write;
    p_reg_info[CANIER_bit0].write = GET_REG32_BIT(0, reg_write);
    p_reg_info[CANIER_bit1].write = GET_REG32_BIT(1, reg_write);
    p_reg_info[CANIER_bit2].write = GET_REG32_BIT(2, reg_write);
    p_reg_info[CANIER_bit3].write = GET_REG32_BIT(3, reg_write);
    p_reg_info[CANIER_bit4].write = GET_REG32_BIT(4, reg_write);
    p_reg_info[CANIER_bit5].write = GET_REG32_BIT(5, reg_write);
    p_reg_info[CANIER_bit6].write = GET_REG32_BIT(6, reg_write);
    p_reg_info[CANIER_bit7].write = GET_REG32_BIT(7, reg_write);
    reg_read = REG32_CAN(reg32);
    p_reg_info[CANIER_bit0].read  = GET_REG32_BIT(0, reg_read);
    p_reg_info[CANIER_bit1].read  = GET_REG32_BIT(1, reg_read);
    p_reg_info[CANIER_bit2].read  = GET_REG32_BIT(2, reg_read);
    p_reg_info[CANIER_bit3].read  = GET_REG32_BIT(3, reg_read);
    p_reg_info[CANIER_bit4].read  = GET_REG32_BIT(4, reg_read);
    p_reg_info[CANIER_bit5].read  = GET_REG32_BIT(5, reg_read);
    p_reg_info[CANIER_bit6].read  = GET_REG32_BIT(6, reg_read);
    p_reg_info[CANIER_bit7].read  = GET_REG32_BIT(7, reg_read);
    if ((reg_read & mask) != (reg_write & mask)) {
        for (i = 0; i < reg_info_len; i++) {
            if (strcmp(p_reg_info[i].access, "W/R") == 0) {
                p_reg_info[i].result = (p_reg_info[i].write != p_reg_info[i].read) ? "X" : "";
            }
        }
        print_reg_info_write(p_reg_info, reg_info_len);
        printf("write: [%#x]\nread:  [%#x]\nMASK:  [%#x]\nresult:[%s]\n\n", reg_write, reg_read, mask, (reg_read & mask) != (reg_write & mask) ? "X" : "ok");
    }
    else
    {
        printf("reg:[%s] write:[%#x] read:[%#x] result:[OK]\n", p_reg32_str, reg_write, reg_read);
    }
}

static void wr_reg_info_CANBTR0(uint32_t reg_write)
{
    uint32_t      i;
    uint32_t      reg_read;
    uint32_t      reg32        = CANBTR0;
    char         *p_reg32_str  = "CANBTR0";
    uint8_t       mask         = CANBTR0_MASK;
    reg_info_w_t *p_reg_info   = reg_info_w_CANBTR0;
    uint32_t      reg_info_len = ARRAY_SIZE(reg_info_w_CANBTR0);
    //-----------------------------------------------------------
    REG32_CAN(reg32) = reg_write;
    p_reg_info[CANBTR0_bit4_0].write = GET_REG32_MULTI_BIT(0x1f, 0, reg_write);
    p_reg_info[CANBTR0_bit7_6].write = GET_REG32_MULTI_BIT(0x03, 6, reg_write);
    reg_read = REG32_CAN(reg32);
    p_reg_info[CANBTR0_bit4_0].read  = GET_REG32_MULTI_BIT(0x1f, 0, reg_write);
    p_reg_info[CANBTR0_bit7_6].read  = GET_REG32_MULTI_BIT(0x03, 6, reg_write);
    printf("reg:[%s] offset:[%#x]\n", p_reg32_str, reg32);
    if ((reg_read & mask) != (reg_write & mask)) {
        for (i = 0; i < reg_info_len; i++) {
            if (strcmp(p_reg_info[i].access, "W/R") == 0) {
                p_reg_info[i].result = (p_reg_info[i].write != p_reg_info[i].read) ? "X" : "";
            }
        }
        print_reg_info_write(p_reg_info, reg_info_len);
        printf("write: [%#x]\nread:  [%#x]\nMASK:  [%#x]\nresult:[%s]\n\n", reg_write, reg_read, mask, (reg_read & mask) != (reg_write & mask) ? "X" : "ok");
    }
    else
    {
        printf("reg:[%s] write:[%#x] read:[%#x] result:[OK]\n", p_reg32_str, reg_write, reg_read);
    }
}

static void wr_reg_info_CANBTR1(uint32_t reg_write)
{
    uint32_t      i;
    uint32_t      reg_read;
    uint32_t      reg32        = CANBTR1;
    char         *p_reg32_str  = "CANBTR1";
    uint8_t       mask         = CANBTR1_MASK;
    reg_info_w_t *p_reg_info   = reg_info_w_CANBTR1;
    uint32_t      reg_info_len = ARRAY_SIZE(reg_info_w_CANBTR0);
    //-----------------------------------------------------------
    REG32_CAN(reg32) = reg_write;
    p_reg_info[CANBTR1_bit7].write   = GET_REG32_BIT(0, reg_write);
    p_reg_info[CANBTR1_bit6_4].write = GET_REG32_MULTI_BIT(0x03, 6, reg_write);
    p_reg_info[CANBTR1_bit3_0].write = GET_REG32_MULTI_BIT(0x03, 6, reg_write);
    reg_read = REG32_CAN(reg32);
    p_reg_info[CANBTR0_bit4_0].read  = GET_REG32_MULTI_BIT(0x1f, 0, reg_write);
    p_reg_info[CANBTR0_bit7_6].read  = GET_REG32_MULTI_BIT(0x03, 6, reg_write);
    printf("reg:[%s] offset:[%#x]\n", p_reg32_str, reg32);
    if ((reg_read & mask) != (reg_write & mask)) {
        for (i = 0; i < reg_info_len; i++) {
            if (strcmp(p_reg_info[i].access, "W/R") == 0) {
                p_reg_info[i].result = (p_reg_info[i].write != p_reg_info[i].read) ? "X" : "";
            }
        }
        print_reg_info_write(p_reg_info, reg_info_len);
        printf("write: [%#x]\nread:  [%#x]\nMASK:  [%#x]\nresult:[%s]\n\n", reg_write, reg_read, mask, (reg_read & mask) != (reg_write & mask) ? "X" : "ok");
    }
    else
    {
        printf("reg:[%s] write:[%#x] read:[%#x] result:[OK]\n", p_reg32_str, reg_write, reg_read);
    }
}









