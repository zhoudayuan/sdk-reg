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
// MASK_WR
#define CANMOD_MASK                  0X1F
#define CANIER_MASK                  0XFF
#define CANBTR0_MASK                 0XDF
#define CANBTR1_MASK                 0XFF
#define CANOCR_MASK                  0X03
#define CANACR_MASK                  0XFF
#define CANRBSA_MASK                 0X3F
#define CANCDR_MASK                  0XFF



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
#define GET_REG32_BIT(_idx, _val)                    ((((_val) & (1<<_idx)) >> _idx) & 0x1)
#define GET_REG32_MULTI_BIT(_bitmask, _idx, _val)    ((((_val) & (_bitmask<<_idx)) >> _idx) & _bitmask)


//#define WRITE_REG32_CAN(_reg,_val)        do {REG32_CAN(_reg) = _val; printf("reg:[%s] op:[W] val:[%#x]\n", #_reg, _val);} while (0)



typedef struct _reg_info_t {
    char     *field;
    char     *name;
    uint32_t  default_val;
    uint32_t  real_val;
    char     *access;
} reg_info_t;


typedef struct _reg_info_wr_t {
    char     *field;
    char     *name;
    uint32_t  default_value;
    uint32_t  write;
    uint32_t  read;
    char     *access;
    char     *result;
} reg_info_wr_t;


typedef struct _check_wr_reg_t {
    char     *name;
    uint32_t  offset;
    uint32_t  write;
    uint32_t  read;
    uint32_t  mask;
} check_wr_reg_t;

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


reg_info_wr_t  reg_info_wr_CANMOD[] = {
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

reg_info_wr_t  reg_info_w_CANCMR[] = {
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

reg_info_wr_t  reg_info_wr_CANIER[] = {
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
reg_info_wr_t  reg_info_wr_CANBTR0[] = {
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
reg_info_wr_t  reg_info_wr_CANBTR1[] = {
    [CANBTR1_bit7]   = {"7",   "SAM",        0, 0x0, 0x0, "W/R", NULL},
    [CANBTR1_bit6_4] = {"6:4", "TSEG2[2:0]", 0, 0x0, 0x0, "W/R", NULL},
    [CANBTR1_bit3_0] = {"3:0", "TSEG1[3:0]", 0, 0x0, 0x0, "W/R", NULL},
};

typedef enum {
    CANOCR_bit1_0
} bit_CANOCR_e;

reg_info_wr_t  reg_info_wr_CANOCR[] = {
    [CANOCR_bit1_0] = {"1:0", "OCMODE", 0, 0x0, 0x0, "W/R", NULL},
};


// CANACR
// Name: CAN Acceptance Code Register.
// Address offset: 0x060
typedef enum {
    CANACR_bit7_4 = 0,
    CANACR_bit3_0
} bit_CANACR_e;

reg_info_wr_t  reg_info_wr_CANACR[] = {
    [CANACR_bit7_4] = {"7:4", "AMR[3:0]", 0, 0x0, 0x0, "W/R", NULL},
    [CANACR_bit3_0] = {"3:0", "ACR[3:0]", 0, 0x0, 0x0, "W/R", NULL},
};


// Name: CAN Receive Buffer Start Address.
// Address offset: 0x068
typedef enum {
    CANRBSA_bit5_0 = 0
} bit_CANRBSA_e;
reg_info_wr_t  reg_info_wr_CANRBSA[] = {
    [CANRBSA_bit5_0] = {"5:0", "RBSA", 0, 0x0, 0x0, "W/R", NULL},
};

// Name: CAN Clock Divider Register.
// Address offset: 0x06C
typedef enum {
    CANCDR_bit7_0 = 0
} bit_CANCDR_e;
reg_info_wr_t  reg_info_wr_CANCDR[] = {
    [CANCDR_bit7_0] = {"7:0", "CDR", 0xc0, 0x0, 0x0, "W/R", NULL},
};



static void show_all_reg();
static void show_reg_info_CANMOD(uint32_t reg_val);
static void print_reg_info_read(reg_info_t *p_reg_info, uint32_t reg_info_len);
static void print_reg_info_write(reg_info_wr_t *reg_info_w, uint32_t reg_info_w_len);
static void write_all_reg(uint32_t reg_write);
static void check_wr_reg_info(check_wr_reg_t *p_check, reg_info_wr_t *p_reg, uint32_t reg_len);
// reg-wr
static void wr_reg_info_CANMOD(uint32_t reg_write);
static void wr_reg_info_CANIER(uint32_t reg_write);
static void wr_reg_info_CANBTR0(uint32_t reg_write);
static void wr_reg_info_CANBTR1(uint32_t reg_write);
static void wr_reg_info_CANOCR(uint32_t reg_write);
static void wr_reg_info_CANACR(uint32_t reg_write);
static void wr_reg_info_CANRBSA(uint32_t reg_write);
static void wr_reg_info_CANCDR(uint32_t reg_write);


typedef void (*wr_reg_info_f)(uint32_t reg_write);

wr_reg_info_f wr_reg_info_array[] = {
    wr_reg_info_CANMOD,
    wr_reg_info_CANIER,
    wr_reg_info_CANBTR0,
    wr_reg_info_CANBTR1,
    wr_reg_info_CANOCR,
    wr_reg_info_CANACR,
    wr_reg_info_CANRBSA,
    wr_reg_info_CANCDR,
};

void test_case_reg_wr()
{
    uint32_t i,j;
    uint32_t wr_val[] = {
        0xffffffff,
        0xAAAAAAAA,
        0x55555555,
        0x00000000,
    };

    printf("test: CANMOD, CANIER, CANBTR0, CANBTR1, CANOCR, CANACR, CANRBSA, CANCDR\n");
    printf("op:   step-1: write, step-2:read\n");
    printf("write value: 0xffffffff, 0xAAAAAAAA, 0x55555555, 0x00000000\n");

    for (i = 0; i < ARRAY_SIZE(wr_val); i++) {
        printf("round:[%u] input:[%#x]\n", i+1, wr_val[i]);
        for (j = 0; j < ARRAY_SIZE(wr_reg_info_array); j++)
        wr_reg_info_array[j](wr_val[i]);
    }
}


int main(void)
{
    int i;
    for (i = 0; i < 1; i++) {
        printf("Hello my friends!\n");
    }

    test_case_reg_wr();
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

static void print_reg_info_write(reg_info_wr_t *reg_info_w, uint32_t reg_info_w_len)
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
#if 0
static void wr_reg_info_CANMOD(uint32_t reg_write)
{
    uint32_t i;
    uint32_t reg_read;
    REG32_CAN(CANMOD) = reg_write;
    reg_info_wr_CANMOD[CANMOD_bit0].write = GET_REG32_BIT(0, reg_write);
    reg_info_wr_CANMOD[CANMOD_bit1].write = GET_REG32_BIT(1, reg_write);
    reg_info_wr_CANMOD[CANMOD_bit2].write = GET_REG32_BIT(2, reg_write);
    reg_info_wr_CANMOD[CANMOD_bit3].write = GET_REG32_BIT(3, reg_write);
    reg_info_wr_CANMOD[CANMOD_bit4].write = GET_REG32_BIT(4, reg_write);
    reg_read = REG32_CAN(CANMOD);
    reg_info_wr_CANMOD[CANMOD_bit0].read  = GET_REG32_BIT(0, reg_read);
    reg_info_wr_CANMOD[CANMOD_bit1].read  = GET_REG32_BIT(1, reg_read);
    reg_info_wr_CANMOD[CANMOD_bit2].read  = GET_REG32_BIT(2, reg_read);
    reg_info_wr_CANMOD[CANMOD_bit3].read  = GET_REG32_BIT(3, reg_read);
    reg_info_wr_CANMOD[CANMOD_bit4].read  = GET_REG32_BIT(4, reg_read);

    if ((reg_read & CANMOD_MASK) != (reg_write & CANMOD_MASK)) {
        for (i = 0; i < ARRAY_SIZE(reg_info_wr_CANMOD); i++) {
            if (strcmp(reg_info_wr_CANMOD[i].access, "W/R") == 0) {
                reg_info_wr_CANMOD[i].result = (reg_info_wr_CANMOD[i].write != reg_info_wr_CANMOD[i].read) ? "X" : "";
            }
        }
    }
    printf("reg:[%s] offset:[%#x]\n", "CANMOD", CANMOD);
    print_reg_info_write(reg_info_wr_CANMOD, ARRAY_SIZE(reg_info_wr_CANMOD));
    printf("write: [%#x]\nread:  [%#x]\nMASK:  [%#x]\nresult:[%s]\n\n", reg_write, reg_read, CANMOD_MASK, (reg_read & CANMOD_MASK) != (reg_write & CANMOD_MASK) ? "X" : "ok");
}
#endif


static void check_wr_reg_info(check_wr_reg_t *p_check, reg_info_wr_t *p_reg, uint32_t reg_len)
{
    uint32_t i;
    if ((p_check->write & p_check->mask) != (p_check->read & p_check->mask)) {
        for (i = 0; i < reg_len; i++) {
            if (strcmp(p_reg[i].access, "W/R") == 0) {
                p_reg[i].result = (p_reg[i].write != p_reg[i].read) ? "X" : "";
            }
        }
        printf("reg:[%s] offset:[%#x]\n", p_check->name, p_check->offset);
        print_reg_info_write(p_reg, reg_len);
    }
    printf("CHECK:[%s] w:[%#x] r:[%#x] mask:[%#x] result:[%s]\n\n",
           p_check->name,
           p_check->write,
           p_check->read,
           p_check->mask,
           (p_check->write & p_check->mask) != (p_check->read & p_check->mask) ? "X" : "ok");
}


static void wr_reg_info_CANMOD(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANMOD";
    check_wr_reg.offset = CANMOD;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANMOD_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANMOD;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANMOD);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;
    p_reg_info[CANMOD_bit0].write = GET_REG32_BIT(0, check_wr_reg.write);
    p_reg_info[CANMOD_bit1].write = GET_REG32_BIT(1, check_wr_reg.write);
    p_reg_info[CANMOD_bit2].write = GET_REG32_BIT(2, check_wr_reg.write);
    p_reg_info[CANMOD_bit3].write = GET_REG32_BIT(3, check_wr_reg.write);
    p_reg_info[CANMOD_bit4].write = GET_REG32_BIT(4, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANMOD_bit0].read  = GET_REG32_BIT(0, check_wr_reg.read);
    p_reg_info[CANMOD_bit1].read  = GET_REG32_BIT(1, check_wr_reg.read);
    p_reg_info[CANMOD_bit2].read  = GET_REG32_BIT(2, check_wr_reg.read);
    p_reg_info[CANMOD_bit3].read  = GET_REG32_BIT(3, check_wr_reg.read);
    p_reg_info[CANMOD_bit4].read  = GET_REG32_BIT(4, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANIER(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANIER";
    check_wr_reg.offset = CANIER;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANIER_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANIER;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANIER);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;
    p_reg_info[CANIER_bit0].write = GET_REG32_BIT(0, check_wr_reg.write);
    p_reg_info[CANIER_bit1].write = GET_REG32_BIT(1, check_wr_reg.write);
    p_reg_info[CANIER_bit2].write = GET_REG32_BIT(2, check_wr_reg.write);
    p_reg_info[CANIER_bit3].write = GET_REG32_BIT(3, check_wr_reg.write);
    p_reg_info[CANIER_bit4].write = GET_REG32_BIT(4, check_wr_reg.write);
    p_reg_info[CANIER_bit5].write = GET_REG32_BIT(5, check_wr_reg.write);
    p_reg_info[CANIER_bit6].write = GET_REG32_BIT(6, check_wr_reg.write);
    p_reg_info[CANIER_bit7].write = GET_REG32_BIT(7, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANIER_bit0].read  = GET_REG32_BIT(0, check_wr_reg.read);
    p_reg_info[CANIER_bit1].read  = GET_REG32_BIT(1, check_wr_reg.read);
    p_reg_info[CANIER_bit2].read  = GET_REG32_BIT(2, check_wr_reg.read);
    p_reg_info[CANIER_bit3].read  = GET_REG32_BIT(3, check_wr_reg.read);
    p_reg_info[CANIER_bit4].read  = GET_REG32_BIT(4, check_wr_reg.read);
    p_reg_info[CANIER_bit5].read  = GET_REG32_BIT(5, check_wr_reg.read);
    p_reg_info[CANIER_bit6].read  = GET_REG32_BIT(6, check_wr_reg.read);
    p_reg_info[CANIER_bit7].read  = GET_REG32_BIT(7, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANBTR0(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANBTR0";
    check_wr_reg.offset = CANBTR0;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANBTR0_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANBTR0;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANBTR0);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;;
    p_reg_info[CANBTR0_bit4_0].write = GET_REG32_MULTI_BIT(0x1f, 0, check_wr_reg.write);
    p_reg_info[CANBTR0_bit7_6].write = GET_REG32_MULTI_BIT(0x03, 6, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANBTR0_bit4_0].read  = GET_REG32_MULTI_BIT(0x1f, 0, check_wr_reg.read);
    p_reg_info[CANBTR0_bit7_6].read  = GET_REG32_MULTI_BIT(0x03, 6, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANBTR1(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANBTR1";
    check_wr_reg.offset = CANBTR1;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANBTR1_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANBTR1;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANBTR1);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;;
    p_reg_info[CANBTR1_bit3_0].write = GET_REG32_MULTI_BIT(0x0F, 0, check_wr_reg.write);
    p_reg_info[CANBTR1_bit6_4].write = GET_REG32_MULTI_BIT(0x07, 4, check_wr_reg.write);
    p_reg_info[CANBTR1_bit7].write   = GET_REG32_MULTI_BIT(0X01, 7, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANBTR1_bit3_0].read  = GET_REG32_MULTI_BIT(0x0F, 0, check_wr_reg.read);
    p_reg_info[CANBTR1_bit6_4].read  = GET_REG32_MULTI_BIT(0x07, 4, check_wr_reg.read);
    p_reg_info[CANBTR1_bit7].read    = GET_REG32_MULTI_BIT(0X01, 7, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANOCR(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANOCR";
    check_wr_reg.offset = CANOCR;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANOCR_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANOCR;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANOCR);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset)  = check_wr_reg.write;;
    p_reg_info[CANOCR_bit1_0].write = GET_REG32_MULTI_BIT(0x03, 0, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANOCR_bit1_0].read  = GET_REG32_MULTI_BIT(0x03, 0, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANACR(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANACR";
    check_wr_reg.offset = CANACR;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANACR_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANACR;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANACR);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;;
    p_reg_info[CANACR_bit3_0].write = GET_REG32_MULTI_BIT(0x0F, 0, check_wr_reg.write);
    p_reg_info[CANACR_bit7_4].write = GET_REG32_MULTI_BIT(0x0F, 4, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANACR_bit3_0].read  = GET_REG32_MULTI_BIT(0x0F, 0, check_wr_reg.read);
    p_reg_info[CANACR_bit7_4].read  = GET_REG32_MULTI_BIT(0x0F, 4, check_wr_reg.read);
     check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANRBSA(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANRBSA";
    check_wr_reg.offset = CANRBSA;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANRBSA_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANRBSA;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANRBSA);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;;
    p_reg_info[CANRBSA_bit5_0].write = GET_REG32_MULTI_BIT(0x3F, 0, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANRBSA_bit5_0].read  = GET_REG32_MULTI_BIT(0x3F, 0, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}

static void wr_reg_info_CANCDR(uint32_t reg_write)
{
    check_wr_reg_t check_wr_reg;
    check_wr_reg.name   = "CANCDR";
    check_wr_reg.offset = CANCDR;
    check_wr_reg.write  = reg_write;
    check_wr_reg.mask   = CANCDR_MASK;
    reg_info_wr_t *p_reg_info   = reg_info_wr_CANCDR;
    uint32_t       reg_info_len = ARRAY_SIZE(reg_info_wr_CANCDR);
    //-----------------------------------------------------------
    REG32_CAN(check_wr_reg.offset) = check_wr_reg.write;;
    p_reg_info[CANCDR_bit7_0].write = GET_REG32_MULTI_BIT(0XFF, 0, check_wr_reg.write);
    check_wr_reg.read = REG32_CAN(check_wr_reg.offset);
    p_reg_info[CANCDR_bit7_0].read  = GET_REG32_MULTI_BIT(0XFF, 0, check_wr_reg.read);
    check_wr_reg_info(&check_wr_reg, p_reg_info, reg_info_len);
}




