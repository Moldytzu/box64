#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "arm64_printer.h"
#include "debug.h"

static const char* Xt[] = {"xEmu", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "xRAX", "xRCX", "xRDX", "xRBX", "xRSP", "xRBP", "xRSI", "xRDI", "xR8", "xR9", "xR10", "xR11", "xR12", "xR13", "xR14", "xR15", "xFlags", "xRIP", "x28", "FP", "LR", "xZR"};
static const char* XtSp[] = {"xEmu", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "xRAX", "xRCX", "xRDX", "xRBX", "xRSP", "xRBP", "xRSI", "xRDI", "xR8", "xR9", "xR10", "xR11", "xR12", "xR13", "xR14", "xR15", "xFlags", "xRIP", "x28", "FP", "LR", "SP"};
static const char* Wt[] = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8", "w9", "wEAX", "wECX", "wEDX", "wEBX", "wESP", "wEBP", "wESI", "wEDI", "wR8", "wR9", "wR10", "wR11", "wR12", "wR13", "wR14", "wR15", "wFlags", "w27", "w28", "w29", "w30", "wZR"};
static const char* WtSp[] = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8", "w9", "wEAX", "wECX", "wEDX", "wEBX", "wESP", "wEBP", "wESI", "wEDI", "wR8", "wR9", "wR10", "wR11", "wR12", "wR13", "wR14", "wR15", "wFlags", "w27", "w28", "w29", "w30", "wSP"};

static const char* conds[] = {"cEQ", "cNE", "cCS", "cCC", "cMI", "cPL", "cVS", "cVC", "cHI", "cLS", "cGE", "cLT", "cGT", "cLE", "c__", "inv"};

typedef struct arm64_print_s {
    int N, S;
    int t, n, m, d;
    int f, c, o, h;
    int i, r, s;
    int x, w;
} arm64_print_t;

int isMask(uint32_t opcode, const char* mask, arm64_print_t *a)
{
    if(strlen(mask)!=32) {
        printf_log(LOG_NONE, "Error: printer mask \"%s\" in not len 32 but %ld\n", mask, strlen(mask));
        return 0;
    }
    memset(a, 0, sizeof(*a));
    int i = 31;
    while(*mask) {
        uint8_t v = (opcode>>i)&1;
        switch(*mask) {
            case '0': if(v!=0) return 0; break;
            case '1': if(v!=1) return 0; break;
            case 'N': a->N = (a->N<<1) | v; break;
            case 'S': a->S = (a->S<<1) | v; break;
            case 't': a->t = (a->t<<1) | v; break;
            case 'n': a->n = (a->n<<1) | v; break;
            case 'm': a->m = (a->m<<1) | v; break;
            case 'd': a->d = (a->d<<1) | v; break;
            case 'f': a->f = (a->f<<1) | v; break;
            case 'c': a->c = (a->c<<1) | v; break;
            case 'i': a->i = (a->i<<1) | v; break;
            case 'r': a->r = (a->r<<1) | v; break;
            case 's': a->s = (a->s<<1) | v; break;
            case 'o': a->o = (a->o<<1) | v; break;
            case 'h': a->h = (a->h<<1) | v; break;
            case 'w': a->w = (a->w<<1) | v; break;
            case 'x': a->x = (a->x<<1) | v; break;
            default:
                printf_log(LOG_NONE, "Warning, printer mask use unhandled '%c'\n", *mask);
        }
        mask++;
        --i;
    }
    
    return 1;
}

int64_t signExtend(uint32_t val, int sz)
{
    int64_t ret = val;
    if((val>>(sz-1))&1 == 1)
        val |= (0xffffffffffffffffll<<sz);
    return val;
}

const char* arm64_print(uint32_t opcode, uintptr_t addr)
{
    static char buff[200];
    arm64_print_t a;
    #define Rn a.n
    #define Rt a.t
    #define Rm a.m
    #define Rd a.d
    #define sf a.f
    #define imm a.i
    #define option a.o
    #define shift a.h
    #define hw a.w
    #define cond a.c
    // --- LDR / STR
    if(isMask(opcode, "1x111000010iiiiiiiii01nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = signExtend(imm, 9);
        snprintf(buff, sizeof(buff), "LDR %s, [%s], %d", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        return buff;
    }
    if(isMask(opcode, "1x111000010iiiiiiiii11nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = signExtend(imm, 9);
        snprintf(buff, sizeof(buff), "LDR %s, [%s, %d]!", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        return buff;
    }
    if(isMask(opcode, "1x11100101iiiiiiiiiiiinnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = (imm)<<size;
        if(offset)
            snprintf(buff, sizeof(buff), "LDR %s, [%s, %d]", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        else
            snprintf(buff, sizeof(buff), "LDR %s, [%s]", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn]);
        return buff;
    }
    if(isMask(opcode, "0x011000iiiiiiiiiiiiiiiiiiittttt", &a)) {
        int size = ((opcode>>30)&1)?3:2;
        int offset = signExtend(imm, 9)<<2;
        snprintf(buff, sizeof(buff), "LDR %s, [#%+d]\t;%p", (size==2)?Wt[Rt]:Xt[Rt], offset, (void*)(addr+offset));
        return buff;
    }
    if(isMask(opcode, "1x111000011mmmmmoooS10nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        const char* extend[] = {"?0", "?1", "UXTW", "LSL", "?4", "?5", "SXTW", "SXTX"};
        int amount = size*a.S;
        if(option==3 && !amount)
            snprintf(buff, sizeof(buff), "LDR %s, [%s, %s]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm]);
        else if(!amount)
        if(option==3 && !amount)
            snprintf(buff, sizeof(buff), "LDR %s, [%s, %s, %s]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm], extend[option]);
        else
            snprintf(buff, sizeof(buff), "LDR %s, [%s, %s, %s %d]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm], extend[option], amount);
        return buff;
    }
    if(isMask(opcode, "1x111000000iiiiiiiii01nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = signExtend(imm, 9);
        snprintf(buff, sizeof(buff), "STR %s, [%s], %d", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        return buff;
    }
    if(isMask(opcode, "1x111000000iiiiiiiii11nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = signExtend(imm, 9);
        snprintf(buff, sizeof(buff), "STR %s, [%s, %d]!", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        return buff;
    }
    if(isMask(opcode, "1x11100100iiiiiiiiiiiinnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        int offset = (imm)<<size;
        if(offset)
            snprintf(buff, sizeof(buff), "STR %s, [%s, %d]", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn], offset);
        else
            snprintf(buff, sizeof(buff), "STR %s, [%s]", (size==0b10)?Wt[Rt]:Xt[Rt], XtSp[Rn]);
        return buff;
    }
    if(isMask(opcode, "1x111000001mmmmmoooS10nnnnnttttt", &a)) {
        int size = (opcode>>30)&3;
        const char* extend[] = {"?0", "?1", "UXTW", "LSL", "?4", "?5", "SXTW", "SXTX"};
        int amount = size*a.S;
        if(option==3 && !amount)
            snprintf(buff, sizeof(buff), "STR %s, [%s, %s]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm]);
        else if(!amount)
        if(option==3 && !amount)
            snprintf(buff, sizeof(buff), "STR %s, [%s, %s, %s]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm], extend[option]);
        else
            snprintf(buff, sizeof(buff), "STR %s, [%s, %s, %s %d]", (size==2)?Wt[Rt]:Xt[Rt], XtSp[Rn], ((option&1)==0)?Wt[Rm]:Xt[Rm], extend[option], amount);
        return buff;
    }
    // --- MOV
    if(isMask(opcode, "f0101010000mmmmm00000011111ddddd", &a)) {
        snprintf(buff, sizeof(buff), "MOV %s, %s", sf?Xt[Rd]:Wt[Rd], sf?Xt[Rm]:Wt[Rm]);
        return buff;
    }
    if(isMask(opcode, "f10100101wwiiiiiiiiiiiiiiiiddddd", &a)) {
        if(!hw)
            snprintf(buff, sizeof(buff), "MOVZ %s, 0x%x", sf?Xt[Rd]:Wt[Rd], imm);
        else
            snprintf(buff, sizeof(buff), "MOVZ %s, 0x%x LSL %d", sf?Xt[Rd]:Wt[Rd], imm, 16*hw);
        return buff;
    }
    if(isMask(opcode, "f11100101wwiiiiiiiiiiiiiiiiddddd", &a)) {
        if(!hw)
            snprintf(buff, sizeof(buff), "MOVK %s, 0x%x", sf?Xt[Rd]:Wt[Rd], imm);
        else
            snprintf(buff, sizeof(buff), "MOVK %s, 0x%x LSL %d", sf?Xt[Rd]:Wt[Rd], imm, 16*hw);
        return buff;
    }

    // --- MATH
    if(isMask(opcode, "f1101011001mmmmmoooiiinnnnn11111", &a)) {
        int R = 0;
        if(option==0b011 || option==0b111)
            R = 1;
        const char* extends[] ={"UXTB", "UXTH", "UXTW", "UXTX", "SXTB", "SXTH", "SXTW", "SXTX"};
        if(imm==0 && option==(sf?3:2))
            snprintf(buff, sizeof(buff), "CMP %s, %s", sf?XtSp[Rn]:WtSp[Rn], (sf&&R)?Xt[Rm]:Wt[Rm]);
        else
            snprintf(buff, sizeof(buff), "CMP %s, %s, %s 0x%x", sf?XtSp[Rn]:WtSp[Rn], (sf&&R)?Xt[Rm]:Wt[Rm], extends[option], imm);
        return buff;
    }
    if(isMask(opcode, "f11100010hiiiiiiiiiiiinnnnn11111", &a)) {
        if(shift==0)
            snprintf(buff, sizeof(buff), "CMP %s, 0x%x", sf?XtSp[Rn]:WtSp[Rn], imm);
        else
            snprintf(buff, sizeof(buff), "ADD %s, 0x%x", sf?XtSp[Rn]:WtSp[Rn], imm<<12);
        return buff;
    }
    if(isMask(opcode, "f1101011hh0mmmmmiiiiiinnnnn11111", &a)) {
        const char* shifts[] = { "LSL", "LSR", "ASR", "???"};
        if(shift==0 && imm==0)
            snprintf(buff, sizeof(buff), "CMP %s, %s", sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm]);
        else 
            snprintf(buff, sizeof(buff), "CMP %s, %s %s %d", sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm], shifts[shift], imm);
        return buff;
    }
    if(isMask(opcode, "f0010001hhiiiiiiiiiiiinnnnnddddd", &a)) {
        if(shift==0)
            snprintf(buff, sizeof(buff), "ADD %s, %s, 0x%x", sf?XtSp[Rd]:WtSp[Rd], sf?XtSp[Rn]:WtSp[Rn], imm);
        else if (shift==1)
            snprintf(buff, sizeof(buff), "ADD %s, %s, 0x%x", sf?XtSp[Rd]:WtSp[Rd], sf?XtSp[Rn]:WtSp[Rn], imm<<12);
        else 
            snprintf(buff, sizeof(buff), "ADD with unhandled shift %d", shift);
        return buff;
    }
    if(isMask(opcode, "f0110001hhiiiiiiiiiiiinnnnnddddd", &a)) {
        if(shift==0)
            snprintf(buff, sizeof(buff), "ADDS %s, %s, 0x%x", sf?Xt[Rd]:Wt[Rd], sf?XtSp[Rn]:WtSp[Rn], imm);
        else if (shift==1)
            snprintf(buff, sizeof(buff), "ADDS %s, %s, 0x%x", sf?Xt[Rd]:Wt[Rd], sf?XtSp[Rn]:WtSp[Rn], imm<<12);
        else 
            snprintf(buff, sizeof(buff), "ADDS with unhandled shift %d", shift);
        return buff;
    }
    if(isMask(opcode, "f0001011hh0mmmmmiiiiiinnnnnddddd", &a)) {
        const char* shifts[] = { "LSL", "LSR", "ASR", "???"};
        if(shift==0 && imm==0)
            snprintf(buff, sizeof(buff), "ADD %s, %s, %s", sf?Xt[Rd]:Wt[Rd], sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm]);
        else 
            snprintf(buff, sizeof(buff), "ADD %s, %s, %s %s %d", sf?Xt[Rd]:Wt[Rd], sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm], shifts[shift], imm);
        return buff;
    }
    if(isMask(opcode, "f1010001hhiiiiiiiiiiiinnnnnddddd", &a)) {
        if(shift==0)
            snprintf(buff, sizeof(buff), "SUB %s, %s, 0x%x", sf?XtSp[Rd]:WtSp[Rd], sf?XtSp[Rn]:WtSp[Rn], imm);
        else if (shift==1)
            snprintf(buff, sizeof(buff), "SUB %s, %s, 0x%x", sf?XtSp[Rd]:WtSp[Rd], sf?XtSp[Rn]:WtSp[Rn], imm<<12);
        else 
            snprintf(buff, sizeof(buff), "SUB with unhandled shift %d", shift);
        return buff;
    }
    if(isMask(opcode, "f1110001hhiiiiiiiiiiiinnnnnddddd", &a)) {
        if(shift==0)
            snprintf(buff, sizeof(buff), "SUBS %s, %s, 0x%x", sf?Xt[Rd]:Wt[Rd], sf?XtSp[Rn]:WtSp[Rn], imm);
        else if (shift==1)
            snprintf(buff, sizeof(buff), "SUBS %s, %s, 0x%x", sf?Xt[Rd]:Wt[Rd], sf?XtSp[Rn]:WtSp[Rn], imm<<12);
        else 
            snprintf(buff, sizeof(buff), "SUBS with unhandled shift %d", shift);
        return buff;
    }
    if(isMask(opcode, "f1001011hh0mmmmmiiiiiinnnnnddddd", &a)) {
        const char* shifts[] = { "LSL", "LSR", "ASR", "???"};
        if(shift==0 && imm==0)
            snprintf(buff, sizeof(buff), "SUB %s, %s, %s", sf?Xt[Rd]:Wt[Rd], sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm]);
        else 
            snprintf(buff, sizeof(buff), "SUB %s, %s, %s %s %d", sf?Xt[Rd]:Wt[Rd], sf?Xt[Rn]:Wt[Rn], sf?Xt[Rm]:Wt[Rm], shifts[shift], imm);
        return buff;
    }
    // ---- LOGIC

    // ---- BRANCH / TEST
    if(isMask(opcode, "1101011000011111000000nnnnn00000", &a)) {
        snprintf(buff, sizeof(buff), "BR %s", Xt[Rn]);
        return buff;
    }
    if(isMask(opcode, "1101011000111111000000nnnnn00000", &a)) {
        snprintf(buff, sizeof(buff), "BLR %s", Xt[Rn]);
        return buff;
    }
    if(isMask(opcode, "01010100iiiiiiiiiiiiiiiiiii0cccc", &a)) {
        int offset = signExtend(imm, 19)<<2;
        snprintf(buff, sizeof(buff), "B.%s #+%d\t; %p", conds[cond], offset, (void*)(addr + offset));
        return buff;
    }
    if(isMask(opcode, "f0110100iiiiiiiiiiiiiiiiiiittttt", &a)) {
        int offset = signExtend(imm, 19)<<2;
        snprintf(buff, sizeof(buff), "CBZ %s, #%+d\t; %p", Xt[Rt], offset, (void*)(addr + offset));
        return buff;
    }

    snprintf(buff, sizeof(buff), "%08X ???", __builtin_bswap32(opcode));
    return buff;
}