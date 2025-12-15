// SPDX-License-Identifier: GPL-2.0+
/*
 * Minimal board support for 3DR HDTC-based i.MX6 Solo boards.
 *
 * This is intentionally small while we finish porting the full Solo/Controller
 * hardware details into U-Boot device trees and environment.
 */

#include <env.h>
#include <init.h>
#include <input.h>
#include <mmc.h>
#include <asm/global_data.h>
#include <asm/arch/sys_proto.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = imx_ddr_size();
	return 0;
}

int board_early_init_f(void)
{
	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;
	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", "3DRHDTC");
	env_set("board_rev", "MX6S");
#endif

	return 0;
}

int overwrite_console(void)
{
	return 1;
}

#ifdef CONFIG_LDO_BYPASS_CHECK
void ldo_mode_set(int ldo_bypass)
{
	(void)ldo_bypass;
}
#endif

int mmc_map_to_kernel_blk(int dev_no)
{
	return dev_no;
}
