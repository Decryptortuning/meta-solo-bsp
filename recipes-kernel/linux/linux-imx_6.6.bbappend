FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}/files:"

# Solo/Artoo DTS starting points; modernize bindings as porting continues.
SRC_URI:append:imx-solo = " \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-drone.dts;subdir=git \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-controller.dts;subdir=git \
"

# One ordered list drives both fetch and NXP's post-defconfig merge path.
OPENSOLO_KERNEL_FRAGMENTS = "solo-usb-gadget.cfg solo-wifi.cfg solo-rfkill.cfg solo-serial.cfg solo-squashfs.cfg solo-netfilter.cfg"
SRC_URI:append:imx-solo = " ${@' '.join('file://' + f for f in d.getVar('OPENSOLO_KERNEL_FRAGMENTS').split())}"
# NXP copies defconfig after kernel_configme; DELTA is the effective merge.
DELTA_KERNEL_DEFCONFIG:append:imx-solo = " ${OPENSOLO_KERNEL_FRAGMENTS}"

# Controller-only PCI interrupt policy; leave the drone candidate unchanged.
SRC_URI:append:imx_controller = " file://controller-pci-legacy.cfg"
DELTA_KERNEL_DEFCONFIG:append:imx_controller = " controller-pci-legacy.cfg"

# Match NXP LOCALVERSION when staging headers for make-mod-scripts/external
# modules; otherwise helpers regenerate a different kernel release suffix.
KERNEL_LOCALVERSION:imx-solo = "-lts-next"

# The drone's ADV7610 is a media-controller subdevice, not a legacy MXC sensor.
SRC_URI:append:imx_drone = " file://solo-media.cfg"
DELTA_KERNEL_DEFCONFIG:append:imx_drone = " solo-media.cfg"
