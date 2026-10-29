# 3DR Solo/Artoo: retain NXP SoC policy without inheriting a SABRE board.
# Derived from rel_imx_6.6.52_2.2.0 imx6solosabresd.conf and its entire
# imx6sabresd-common.inc wrapper; imx-base still owns vendor SoC defaults.
MACHINEOVERRIDES =. "mx6:mx6dl:"
require conf/machine/include/imx-base.inc
require conf/machine/include/arm/armv7a/tune-cortexa9.inc
MACHINEOVERRIDES =. "imx-solo:"

PREFERRED_PROVIDER_u-boot = "u-boot-imx"
PREFERRED_PROVIDER_virtual/bootloader = "u-boot-imx"
UBOOT_CONFIG ??= "sd"
# SDIO/Bluetooth/OP-TEE/touchscreen and ath6k belong to SABRE, not Solo.
MACHINE_FEATURES = "usbgadget usbhost vfat alsa screen pci wifi"

SOLOLINK_CONFIG_DIR = "/etc"
KERNEL_IMAGETYPE = "uImage"
INITRAMFS_FSTYPES ?= "cpio.gz"
INITRAMFS_IMAGE = "3dr-initramfs"
INITRAMFS_IMAGE_NAME = "${INITRAMFS_IMAGE}-${MACHINE}.rootfs"
INITRAMFS_IMAGE_BUNDLE = "1"
USE_VT = "0"
SERIAL_CONSOLES = ""
