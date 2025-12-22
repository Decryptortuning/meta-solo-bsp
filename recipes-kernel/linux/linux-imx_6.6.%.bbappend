FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}/files:"

# Solo/Artoo DTS starting points; modernize bindings as porting continues.
SRC_URI:append:imx-solo = " \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-drone.dts;subdir=git \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-controller.dts;subdir=git \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-3dr.dts;subdir=git \
    file://arch/arm/boot/dts/nxp/imx/imx6solo-3dr-analogtvin.dts;subdir=git \
    file://solo-netfilter.cfg \
"

# Solo kernel config fragments
SRC_URI:append:imx-solo = " \
    file://solo-media.cfg \
    file://solo-usb-gadget.cfg \
    file://solo-wifi.cfg \
    file://solo-rfkill.cfg \
    file://solo-serial.cfg \
    file://solo-squashfs.cfg \
    file://solo-netfilter.cfg \
"

# NXP's linux-imx recipe copies its defconfig after do_kernel_configme, which
# overrides KERNEL_CONFIG_FRAGMENTS. Use DELTA_KERNEL_DEFCONFIG so our fragments
# are merged in do_merge_delta_config after the defconfig copy.
DELTA_KERNEL_DEFCONFIG:append:imx-solo = " \
    solo-media.cfg \
    solo-usb-gadget.cfg \
    solo-wifi.cfg \
    solo-rfkill.cfg \
    solo-serial.cfg \
    solo-squashfs.cfg \
    solo-netfilter.cfg \
"
