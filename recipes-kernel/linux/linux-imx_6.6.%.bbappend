FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# Solo-specific DTS placeholders; replace with fully ported DTS when ready.
SRC_URI:append:solo-imx6 = " \
    file://arch/arm/boot/dts/imx6solo-3dr.dts \
    file://arch/arm/boot/dts/imx6solo-3dr-analogtvin.dts \
"

# Solo kernel config fragments
SRC_URI:append:solo-imx6 = " \
    file://solo-media.cfg \
    file://solo-usb-gadget.cfg \
    file://solo-wifi.cfg \
    file://solo-serial.cfg \
"

KERNEL_CONFIG_FRAGMENTS:append:solo-imx6 = " \
    ${WORKDIR}/solo-media.cfg \
    ${WORKDIR}/solo-usb-gadget.cfg \
    ${WORKDIR}/solo-wifi.cfg \
    ${WORKDIR}/solo-serial.cfg \
"
