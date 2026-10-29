// SPDX-License-Identifier: GPL-2.0-only
/* Retain the legacy controller holds across MCU reset and Linux reboot. */
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>

struct opensolo_power {
	struct gpio_descs *holds;
};

static int opensolo_power_off(struct sys_off_data *data)
{
	struct opensolo_power *power = data->cb_data;
	unsigned int i;

	/* Final power-off only, after userspace/filesystem shutdown. Restore the
	 * tested input state so the STM32 can withdraw PC14/PC15 power enables.
	 * No shutdown callback: device_shutdown() also runs on warm reboot.
	 */
	for (i = 0; i < power->holds->ndescs; i++)
		gpiod_direction_input(power->holds->desc[i]);
	return NOTIFY_DONE;
}

static int opensolo_power_probe(struct platform_device *pdev)
{
	struct opensolo_power *power;
	unsigned int i;

	power = devm_kzalloc(&pdev->dev, sizeof(*power), GFP_KERNEL);
	if (!power)
		return -ENOMEM;
	power->holds = devm_gpiod_get_array(&pdev->dev, "hold", GPIOD_OUT_HIGH);
	if (IS_ERR(power->holds))
		return dev_err_probe(&pdev->dev, PTR_ERR(power->holds), "power hold GPIOs\n");
	if (power->holds->ndescs != 2)
		return -EINVAL;
	/* SYS_OFF_MODE_POWER_OFF is atomic; these are SoC GPIOs, not expanders. */
	for (i = 0; i < power->holds->ndescs; i++)
		if (gpiod_cansleep(power->holds->desc[i]))
			return -EINVAL;
	return devm_register_sys_off_handler(&pdev->dev, SYS_OFF_MODE_POWER_OFF,
			SYS_OFF_PRIO_HIGH, opensolo_power_off, power);
}

static const struct of_device_id opensolo_power_of_match[] = {
	{ .compatible = "opensolo,controller-power-hold" },
	{ }
};
MODULE_DEVICE_TABLE(of, opensolo_power_of_match);

static struct platform_driver opensolo_power_driver = {
	.probe = opensolo_power_probe,
	.driver = {
		.name = "opensolo-controller-power",
		.of_match_table = opensolo_power_of_match,
		.suppress_bind_attrs = true,
	},
};
builtin_platform_driver(opensolo_power_driver);
MODULE_DESCRIPTION("OpenSolo controller power holds and final power-off");
MODULE_LICENSE("GPL");
