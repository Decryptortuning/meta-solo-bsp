FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# Solo boot environment: two-slot (LATEST/GOLDEN) MMC fallback and console args
SRC_URI:append:solo-imx6 = " file://solo-boot.env "

do_install:append:solo-imx6 () {
    install -d ${D}${sysconfdir}/u-boot
    install -m 0644 ${WORKDIR}/solo-boot.env ${D}${sysconfdir}/u-boot/solo-boot.env
}
