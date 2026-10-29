# OpenSolo active meta-solo-bsp layer

Current build/evidence authority: /home/jresp/solo/workspace/docs/README.md.
The integration lock owns exact repository and compiled recipe revisions.
Vendor u-boot-imx checkout inventory differs from the compiled u-boot-imx recipe
pin; do not overwrite one to conceal the distinction.

# Solo BSP, NXP Scarthgap 6.6.52

This layer owns the two board machines, kernel/DTS and U-Boot. Product image,
init/update/recovery and distro policy belong to meta-3dr. Historical DTS are
under reference/ and are deliberately not in any recipe SRC_URI.

## Machine inheritance review

The previous imx6solosabresd.conf contributes mx6:mx6dl overrides, requires
imx6sabresd-common.inc, selects two SABRE DTBs, u-boot-imx providers, zImage,
OP-TEE, sd/sd-optee configs and OPTEE_BIN_EXT=6solosdb. Its complete common
wrapper requires imx-base.inc and tune-cortexa9.inc, sets tty, appends ath6k
firmware and adds pci/wifi/bluetooth/three SDIO feature tokens. The owned
solo-imx6-common.inc now requires the same SoC and tune includes directly in
that order, preserving NXP defaults and override expansion. Solo already
replaced DTB, console, uImage and SD defconfig and removed OP-TEE. Unused
sd-optee and OPTEE_BIN_EXT are intentionally absent. Board features explicitly
retain USB, vfat, ALSA, screen, PCI and Wi-Fi. Neither board has SABRE SDIO,
Bluetooth or touchscreen identity; ath6k firmware is no longer board-added
(the boards use PCIe ath9k). This is source equivalence review, not a claim that
post-change BitBake effective metadata has already passed comparison.

After source freeze, compare both machine environments for tune/ABI, extended
SoC overrides, providers/versions, UBOOT_CONFIG/suffix/entry point/load address,
DTB, initramfs and firmware closure. Only these documented board deltas are
intentional. Kernel remains the NXP DELTA_KERNEL_DEFCONFIG merge pipeline;
KERNEL_LOCALVERSION=-lts-next now also applies to drone external modules.
Controller pci=nomsi and CMDLINE_EXTEND are unchanged.

## Boot contract

The header include/configs/3dr_hdtc.h is authoritative for slot commands; each
board defconfig owns bootcmd and board identity. Poky's u-boot.inc compiles
u-boot-initial-env from those sources and installs/deploys it. The BSP copies
that generated environment to /etc/u-boot/boot.env. No hand-maintained second
script exists. A slot boot loads its own kernel and DTB successfully before
bootm; neither slot may boot stale RAM or omit DTB. Failure of LATEST permits
a fresh GOLDEN attempt. MMC selection must succeed before either attempt.
Existing saved raw environments can override new defaults: qualification must
inspect/migrate them intentionally. Building this source does not migrate a
physical device. Preserve MBR, uImage, raw env 0xC0000/0x2000 and ext3 flow.

## Drone capture candidate and application gate

Pinned adv7604.c adv76xx_reset() writes descriptor logical 0 then 1 while its
comment specifies a physical low reset pulse. GPIO_ACTIVE_HIGH matches that
specific driver's contract; changing to ACTIVE_LOW holds physical reset low.
The drone fragment selects the existing media-controller graph:
ADV7610 -> video-mux/MMIO mux -> IPUv3 CSI -> capture. VIDEO_IMX_MEDIA builds
imx-media-common, imx6-media and imx6-media-csi in this NXP tree. The formerly
requested VIDEO_IMX_MEDIA_CAMERA, VIDEO_IMX_PARALLEL_CSI and VIDEO_IMX_CSI do
not exist. VIDEO_IMX_CAPTURE here gates i.MX8 components, not the i.MX6 CSI.
Staging, VIDEO_DEV/subdev, IPUv3 and mux prerequisites are explicit. The legacy
MXC IPU and capture stack is disabled for drone to avoid two drivers owning the
same IPU; its fsl,imx6q-v4l2-capture node is removed. Controller media selection
is untouched apart from removing ineffective drone-only fragment requests.

This does NOT qualify the legacy application pipeline. vidlaunch.cpp invokes
mxc_v4l2_capture, opens /dev/video0 and uses mfw_v4lsrc/mfw_ipucsc/vpuenc;
hdmiout.c uses vpudec/mfw_isink. The pinned legacy MXC sensor drivers have no
ADV7610 integration with the modern adv7604 subdevice. A coordinated userspace
media graph/format setup and GStreamer port is required before drone video
acceptance. Do not claim an image build alone establishes capture. Qualify
8-bit wiring, BT.656 timings, EDID and the UART1/CSI shared-pin limitation on
hardware; 1080p is not asserted by this board description.

## DRAM acceptance gate

Physical drone capacity has not been measured. Retain the historical drone
DTS 1 GiB placeholder, header 512 MiB default and DCD geometry without inventing
a new size. Board dram_init() uses imx_ddr_size(); U-Boot ARM arch_fixup_fdt()
uses bd->bi_dram and fdt_fixup_memory_banks with CONFIG_OF_LIBFDT explicitly
selected. The runtime detected banks, not either static number, are authoritative.
Before hardware promotion, assert DCD geometry matches the actual fitted parts,
U-Boot bdinfo bank range equals detected RAM, and the handed-off Linux memory
node equals that bank range (account for explicitly configured carveouts).
Assert kernel/DT/initramfs/bootm regions fit the detected range. Retain boot logs
and /proc/iomem evidence. Until those assertions pass, DRAM is unqualified.

## Deferred verification

No build or test was run during this edit wave. After the combined source pin
and contract review: parse both machines; check append/provider closure; merge
and check every requested drone media symbol and controller PCI policy; compile
DTS/U-Boot; inspect generated boot.env failure branches; compare generated
kernel release, module vermagic and symbol CRCs for both boards. Exercise slot
load failures without hardware writes, then independently qualify both boards
on authorized spare media. No automatic flashing or promotion is implied.
