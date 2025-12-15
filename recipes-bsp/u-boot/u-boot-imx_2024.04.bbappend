FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# Drone/controller U-Boot configs (WIP) + boot environments.
SRC_URI:append:imx-solo = " \
    file://0001-imx6-add-3dr-hdtc-board-target.patch \
    file://0002-mx6sabre-common-3dr-hdtc-default-fdt.patch \
    file://configs/imx6_drone_defconfig;subdir=git \
    file://configs/imx6_controller_defconfig;subdir=git \
    file://arch/arm/dts/imx6solo-drone.dts;subdir=git \
    file://arch/arm/dts/imx6solo-drone-u-boot.dtsi;subdir=git \
    file://arch/arm/dts/imx6solo-controller.dts;subdir=git \
    file://arch/arm/dts/imx6solo-controller-u-boot.dtsi;subdir=git \
    file://board/3dr/3dr_hdtc/Kconfig;subdir=git \
    file://board/3dr/3dr_hdtc/Makefile;subdir=git \
    file://board/3dr/3dr_hdtc/3dr_hdtc.c;subdir=git \
    file://board/3dr/3dr_hdtc/mx6solo_2x_mt41j128.cfg;subdir=git \
    file://include/configs/3dr_hdtc.h;subdir=git \
    file://drone-boot.env \
    file://controller-boot.env \
"

do_install:append:imx_drone () {
    install -d ${D}${sysconfdir}/u-boot
    install -m 0644 ${WORKDIR}/drone-boot.env ${D}${sysconfdir}/u-boot/boot.env
}

do_install:append:imx_controller () {
    install -d ${D}${sysconfdir}/u-boot
    install -m 0644 ${WORKDIR}/controller-boot.env ${D}${sysconfdir}/u-boot/boot.env
}
