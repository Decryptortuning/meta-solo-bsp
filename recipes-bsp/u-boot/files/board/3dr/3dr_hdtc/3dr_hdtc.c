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
#ifdef CONFIG_HDTC_CONTROLLER_POWER_HOLD
#include <asm/arch/imx-regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/io.h>
#include <asm/mach-imx/gpio.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = imx_ddr_size();
	return 0;
}

int board_early_init_f(void)
{
#ifdef CONFIG_HDTC_CONTROLLER_POWER_HOLD
	struct gpio_regs *gpio1 = (struct gpio_regs *)GPIO1_BASE_ADDR;
	struct gpio_regs *gpio2 = (struct gpio_regs *)GPIO2_BASE_ADDR;
	const u32 holds = BIT(19) | BIT(21);
	const iomux_v3_cfg_t pads[] = {
		MX6_PAD_SD1_DAT2__GPIO1_IO19 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_SD1_DAT3__GPIO1_IO21 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_SD4_DAT5__GPIO2_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL),
		MX6_PAD_SD4_DAT6__GPIO2_IO14 | MUX_PAD_CTRL(NO_PAD_CTRL),
	};

	/* Driver model is not available here. Preserve every unrelated bit.
	 * Latch high before enabling outputs; select the GPIO mux last so an
	 * inherited low latch cannot glitch either power hold during handoff.
	 * BOOT0/reset stay inputs until the loader deliberately claims them.
	 */
	setbits_le32(&gpio1->gpio_dr, holds);
	setbits_le32(&gpio1->gpio_dir, holds);
	clrbits_le32(&gpio2->gpio_dir, BIT(13) | BIT(14));
	imx_iomux_v3_setup_multiple_pads(pads, ARRAY_SIZE(pads));
#endif
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

int board_mmc_get_env_dev(int devno)
{
	/*
	 * i.MX6 Boot ROM reports the USDHC instance number, but U-Boot numbers
	 * available MMC devices from 0. Map the ROM instance to U-Boot's MMC
	 * device index so env/fastboot target the actual boot device.
	 */
	return devno > 0 ? devno - 1 : devno;
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
