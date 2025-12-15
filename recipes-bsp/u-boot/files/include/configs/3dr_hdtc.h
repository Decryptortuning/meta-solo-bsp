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

#define CFG_SYS_FSL_USDHC_NUM	3

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CFG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CFG_MXC_USB_FLAGS		0
#endif

#endif /* __3DR_HDTC_CONFIG_H */

