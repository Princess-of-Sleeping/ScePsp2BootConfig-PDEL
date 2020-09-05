/*
 * PS Vita Kernel bootconfig
 * Copyright (C) 2019, Princess of Sleeping
 */

#include <psp2kern/kernel/modulemgr.h>
#include "kbl.h"

#define BASE_KERNEL_ENTRY_NUM  (0xD)
#define BASE_KERNEL_ENTRY_NUM2 (0xF)

const SceModuleLoadList list_base_kernel[BASE_KERNEL_ENTRY_NUM] = {	// size 0x34, 0xD
	{"sysmem.skprx"},
	{"excpmgr.skprx"},
	{"intrmgr.skprx"},
	{"buserror.skprx"},
	{"systimer.skprx"},
	{"acmgr.skprx"},
	{"threadmgr.skprx"},
	{"dmacmgr.skprx"},
	{"smsc_proxy.skprx"},
	{"authmgr.skprx"},
	{"processmgr.skprx"},
	{"iofilemgr.skprx"},
	{"modulemgr.skprx"}
};

SceUID base_kernel_uid[BASE_KERNEL_ENTRY_NUM];

const SceModuleLoadList list_base_kernel2[BASE_KERNEL_ENTRY_NUM2] = {	// size 0x3C, 0xF
	{"lowio.skprx"},
	{"syscon.skprx"},
	{NULL},		// "lcd.skprx" or "oled.skprx"
	{NULL},		// "hdmi.skprx"
	{"display.skprx"},
	{"sm_comm.skprx"},
	{"ss_mgr.skprx"},
	{"sdif.skprx"},
	{"msif.skprx"},
	{"gcauthmgr.skprx"},
	{"sdstor.skprx"},
	{"rtc.skprx"},
	{"exfatfs.skprx"},
	{"bsod.skprx"},
	{"sysstatemgr.skprx"}
};

SceUID base_kernel_uid2[BASE_KERNEL_ENTRY_NUM2];
SceModuleLoadList list_base_kernel2_rw[BASE_KERNEL_ENTRY_NUM2];

const SceModuleLoadList list_devkit_module3[] = {
	{"sdbgsdio.skprx"},
	{"deci4p_sdfmgr.skprx"},
	{"deci4p_sttyp.skprx"}
};

SceUID list_devkit_module3_uid[0x3];

const SceModuleLoadList list_deci4p_sdbgp = {
	"deci4p_sdbgp.skprx"
};

SceUID deci4p_sdbgp_uid;

const SceModuleLoadList list_Invalid = {
	NULL
};

SceUID Invalid_uid;

SceSysrootHardwareFlags hw_flags;

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize args, void *argp){

	int flag;

	ksceKblLoadModule(list_base_kernel, base_kernel_uid, BASE_KERNEL_ENTRY_NUM, 0);

	if((ksceKblCheckDipsw(0xC1) != 0) && (ksceKblIsCEX() == 0) && (ksceKblIsDEX() == 0) && (ksceKblIsCEXJpFat() == 0)){
		flag = (ksceKblCheckDipsw(0xD2) != 0) ? 1 : 0;

		ksceKblLoadModule(list_devkit_module3, list_devkit_module3_uid, 3, flag);

		if(ksceKblIsAllowKernelDebug() != 0){
			ksceKblLoadModule(&list_deci4p_sdbgp, &deci4p_sdbgp_uid, 1, flag);
			ksceKblLoadModule(&list_Invalid, &Invalid_uid, 1, flag);
		}
	}

	for(int i=0;i<0xF;i++)
		list_base_kernel2_rw[i].filename = list_base_kernel2[i].filename;

	if(ksceKblIsGenuineDolce() == 0){
		ksceKblGetHardwareFlags(&hw_flags);
		list_base_kernel2_rw[2].filename = ((*(uint8_t *)(&hw_flags.data[0]) & 9) != 0) ? "lcd.skprx" : "oled.skprx";
	}else{
		list_base_kernel2_rw[2].filename = NULL;
	}

	if((ksceKblIsVITA() == 0) || ((ksceKblIsCEX() == 0) && (ksceKblIsDEX() == 0)))
		list_base_kernel2_rw[3].filename = "hdmi.skprx";

	ksceKblLoadModule(list_base_kernel2_rw, base_kernel_uid2, BASE_KERNEL_ENTRY_NUM2, 0);

	ksceKblFreeFileSystemCtx();
	ksceKblAuthMgrClose();

	return SCE_KERNEL_START_SUCCESS;
}

int module_bootstart(SceSize args, void *argp){

	ksceKblCpuDisableIrqInterrupts();

	*(int   *)(argp + 0x2FC) = 0;
	*(int   *)(argp + 0x300) = 0;
	*(void **)(argp + 0x304) = ksceKblGetPutcharHandler();
	*(int   *)(argp + 0x308) = ksceKblGetMinimumLogLevel();

	ksceKblStartModule(base_kernel_uid, BASE_KERNEL_ENTRY_NUM, sizeof(argp), argp);

	if((ksceKblCheckDipsw(0xC1) != 0) && (ksceKblIsCEX() == 0) && (ksceKblIsDEX() == 0) && (ksceKblIsCEXJpFat() == 0)){
		ksceKblStartModule(list_devkit_module3_uid, 3, 0, NULL);

		if(ksceKblIsAllowKernelDebug() != 0){
			ksceKblStartModule(&deci4p_sdbgp_uid, 1, 0, NULL);
		}
	}

	ksceKblSysrootThreadMgrStartAfterProcess();
	ksceKblSetNonSyncModuleStart();
	ksceKblSysrootProcessmgrStart2();
	ksceKblSysrootIofilemgrStart();

	if(ksceKblIsAllowKernelDebug() != 0 && ksceKblCheckDipsw(0xC1) != 0 && ksceKblIsCEX() == 0 && ksceKblIsDEX() == 0 && ksceKblIsCEXJpFat() == 0)
		ksceKblStartModule(&Invalid_uid, 1, 0, NULL);

	ksceKblStartModule(base_kernel_uid2, BASE_KERNEL_ENTRY_NUM2, sizeof(argp), argp);

	return SCE_KERNEL_START_SUCCESS;
}
