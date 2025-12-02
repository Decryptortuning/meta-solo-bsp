FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# Solo-specific DTS placeholders; replace with fully ported DTS when ready.
SRC_URI:append:solo-imx6 = " \
    file://arch/arm/boot/dts/imx6solo-3dr.dts \
    file://arch/arm/boot/dts/imx6solo-3dr-analogtvin.dts \
"
