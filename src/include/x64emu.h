#ifndef __X86EMU_H_
#define __X86EMU_H_

typedef struct x64emu_s x64emu_t;
typedef struct box64context_s box64context_t;

x64emu_t *NewX86Emu(box64context_t *context, uintptr_t start, uintptr_t stack, int stacksize, int ownstack);
x64emu_t *NewX86EmuFromStack(x64emu_t* emu, box64context_t *context, uintptr_t start, uintptr_t stack, int stacksize, int ownstack);
void SetupX86Emu(x64emu_t *emu);
void FreeX86Emu(x64emu_t **x64emu);
void FreeX86EmuFromStack(x64emu_t **emu);
void CloneEmu(x64emu_t *newemu, const x64emu_t* emu);
void SetTraceEmu(uintptr_t trace_start, uintptr_t trace_end);

box64context_t* GetEmuContext(x64emu_t* emu);
uint32_t* GetParityTab();

uint32_t GetEAX(x64emu_t *emu);
uint64_t GetRAX(x64emu_t *emu);
void SetEAX(x64emu_t *emu, uint32_t v);
void SetEBX(x64emu_t *emu, uint32_t v);
void SetECX(x64emu_t *emu, uint32_t v);
void SetEDX(x64emu_t *emu, uint32_t v);
void SetEDI(x64emu_t *emu, uint32_t v);
void SetESI(x64emu_t *emu, uint32_t v);
void SetEBP(x64emu_t *emu, uint32_t v);
//void SetESP(x64emu_t *emu, uint32_t v);
void SetRAX(x64emu_t *emu, uint64_t v);
void SetRBX(x64emu_t *emu, uint64_t v);
void SetRCX(x64emu_t *emu, uint64_t v);
void SetRDX(x64emu_t *emu, uint64_t v);
void SetRDI(x64emu_t *emu, uint64_t v);
void SetRSI(x64emu_t *emu, uint64_t v);
void SetRBP(x64emu_t *emu, uint64_t v);
void SetRSP(x64emu_t *emu, uint64_t v);
void SetRIP(x64emu_t *emu, uint64_t v);
void SetFS(x64emu_t *emu, uint16_t v);
uint16_t GetFS(x64emu_t *emu);
uint64_t GetRSP(x64emu_t *emu);
void ResetFlags(x64emu_t *emu);
void ResetSegmentsCache(x64emu_t *emu);
const char* DumpCPURegs(x64emu_t* emu, uintptr_t ip);

void StopEmu(x64emu_t* emu, const char* reason);
//void PushExit(x64emu_t* emu);
//void* GetExit();
void EmuCall(x64emu_t* emu, uintptr_t addr);
void AddCleanup(x64emu_t *emu, void *p);
void AddCleanup1Arg(x64emu_t *emu, void *p, void* a);
void CallCleanup(x64emu_t *emu, void* p);
void CallAllCleanup(x64emu_t *emu);
void UnimpOpcode(x64emu_t* emu);

uint64_t ReadTSC(x64emu_t* emu);

double FromLD(void* ld);        // long double (80bits pointer) -> double
void LD2D(void* ld, void* d);   // long double (80bits) -> double (64bits)
void D2LD(void* d, void* ld);   // double (64bits) -> long double (64bits)

void printFunctionAddr(uintptr_t nextaddr, const char* text);
const char* getAddrFunctionName(uintptr_t addr);

#endif //__X86EMU_H_