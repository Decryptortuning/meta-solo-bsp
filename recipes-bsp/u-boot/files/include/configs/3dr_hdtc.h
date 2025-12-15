/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration settings for 3DR HDTC-based i.MX6 Solo boards.
 */

#ifndef __3DR_HDTC_CONFIG_H
#define __3DR_HDTC_CONFIG_H

#define CFG_MXC_UART_BASE	UART1_BASE
#define CONSOLE_DEV		"ttymxc0"

#if defined(CONFIG_MX6S)
#define PHYS_SDRAM_SIZE		(512u * 1024 * 1024)
#endif

#include "mx6sabre_common.h"

/* 3DR legacy boot flow: uImage+initramfs, squashfs rootfs on GOLDEN/LATEST. */
#undef CFG_EXTRA_ENV_SETTINGS
#define CFG_EXTRA_ENV_SETTINGS \
	CFG_MFG_ENV_SETTINGS \
	TEE_ENV \
	"script=boot.scr\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"console=" CONSOLE_DEV "\0" \
	"smp=" SYS_NOSMP "\0" \
	"factoryReset=0\0" \
	"uimage=uImage\0" \
	"fdt_file=" CONFIG_DEFAULT_DEVICE_TREE ".dtb\0" \
	"fdt_addr=0x18000000\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"splashimage=0x28000000\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcpart=2\0" \
	"mmcroot=/dev/mmcblk0p2 rootwait ro\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=${mmcroot} video=mxcfb0:dev=hdmi,1280x720M@60 " \
		"consoleblank=0 vt.global_cursor_default=0 factoryreset=${factoryReset}\0" \
	"loaduimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${uimage}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=run mmcargs; " \
		"if run loadfdt; then " \
			"bootm ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"bootm ${loadaddr}; " \
		"fi\0" \
	"boot_latest=setenv mmcpart 2; " \
		"setenv mmcroot /dev/mmcblk0p2 rootwait ro; " \
		"if run loaduimage; then run mmcboot; fi\0" \
	"boot_golden=setenv mmcpart 1; " \
		"setenv mmcroot /dev/mmcblk0p1 rootwait ro; " \
		"run loaduimage; run mmcboot\0"

#define CFG_SYS_FSL_USDHC_NUM	3

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CFG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CFG_MXC_USB_FLAGS		0
#endif

#endif /* __3DR_HDTC_CONFIG_H */
