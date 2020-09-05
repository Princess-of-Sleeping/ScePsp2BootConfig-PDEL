/*
 * PS Vita Kernel bootconfig Header
 * Copyright (C) 2019, Princess of Sleeping
 */

#ifndef _KBL_H_
#define _KBL_H_

#include <psp2kern/types.h>

// SceDebug
int ksceKblPutchar(void *args, char c);
int ksceKblPrintf(const char *fmt, ...);
// ksceKblPrintfLevel
int ksceKblGetMinimumLogLevel(void);
void *ksceKblGetPutcharHandler(void);

// SceSysroot
typedef struct SceSysrootHardwareFlags {
	uint32_t data[4];
} __attribute__((packed)) SceSysrootHardwareFlags;

int ksceKblGetHardwareFlags(SceSysrootHardwareFlags *data);
int ksceKblSysrootIofilemgrStart(void);
int ksceKblSysrootProcessmgrStart2(void);
int ksceKblSysrootThreadMgrStartAfterProcess(void);
void ksceKblSysrootCpuCoreSyncAll(void);
void ksceKblSysrootCpuCoreSyncWait(int core);

// SceKbl
typedef struct SceModuleLoadList {
	const char *filename;
} __attribute__((packed)) SceModuleLoadList;

int ksceKblLoadModule(const SceModuleLoadList *list, SceUID *uid, int count, int some_flag);
int ksceKblStartModule(SceUID *uid_list, int count, SceSize args, void *argp);
int ksceKblFreeFileSystemCtx(void);
int ksceKblSetNonSyncModuleStart(void);
int ksceKblAuthMgrClose(void);
void ksceKblCpuDisableIrqInterrupts(void);

// SceSblAIMgr
int ksceKblIsCEX(void);
int ksceKblIsCEXJpFat(void);
int ksceKblIsDEX(void);
int ksceKblIsTool(void);
int ksceKblIsTest(void);

int ksceKblIsVITA(void);
int ksceKblIsDolce(void);
int ksceKblIsGenuineDolce(void);

// SceDipsw
int ksceKblCheckDipsw(int a1);

// SceQafMgr
int ksceKblIsAllowKernelDebug(void);

#endif	/* _KBL_H_ */
