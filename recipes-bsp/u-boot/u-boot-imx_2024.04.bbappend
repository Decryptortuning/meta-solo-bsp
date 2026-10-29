FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# Drone/controller U-Boot configs (WIP) + boot environments.
SRC_URI:append:imx-solo = " \
    file://0001-imx6-add-3dr-hdtc-board-target.patch \
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
"

# u-boot.inc generates this from the compiled header and defconfig. Never
# maintain a second handwritten boot command/environment in this layer.
do_install:append:imx-solo () {
    test -n "${UBOOT_INITIAL_ENV}" || bbfatal "Solo needs the generated initial environment"
    install -d ${D}${sysconfdir}/u-boot
    install -m 0644 ${D}${sysconfdir}/${UBOOT_INITIAL_ENV} ${D}${sysconfdir}/u-boot/boot.env
}
FILES:${PN}:append:imx-solo = " ${sysconfdir}/u-boot/boot.env"

# Preserve variant-specific deploy naming if multiple configs are built.
do_deploy:append:imx-solo() {
    if [ -n "${UBOOT_CONFIG}" ]; then
        unset i j
        for config in ${UBOOT_MACHINE}; do
            i=$(expr $i + 1);
            for type in ${UBOOT_CONFIG}; do
                j=$(expr $j + 1);
                if [ $j -eq $i ]; then
                    install -m 644 \
                        ${B}/${config}/${UBOOT_BINARYNAME}-${type}.${UBOOT_SUFFIX} \
                        ${DEPLOYDIR}/${UBOOT_IMAGE}_${type}
                fi
            done
            unset j
        done
        unset i
    fi
}
