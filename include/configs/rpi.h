/*
 * (C) Copyright 2012-2016 Stephen Warren
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "kubos-common.h"
#include <asm/arch/timer.h>
#include <linux/sizes.h>
#include <config_distro_defaults.h>
#include <config_distro_bootcmd.h>

/* Undo things we don't want to include from the base rpi configuration */
#undef CONFIG_BOOTCOMMAND
#undef BOOT_TARGET_DEVICES
#undef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_ENV_IS_IN_MMC
#undef CONFIG_ENV_IS_IN_FAT
#undef CONFIG_ENV_IS_NOWHERE
#undef CONFIG_ENV_SIZE
#undef DFU_ALT_INFO_MMC
#undef DFU_ALT_INFO_NOR
#undef CONFIG_BOOTCOUNT_AM33XX
/* End of undefs */

#if defined(CONFIG_TARGET_RPI_2) || defined(CONFIG_TARGET_RPI_3_32B)
#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

/* Architecture, CPU, etc.*/
#define CONFIG_ARCH_CPU_INIT
#define CONFIG_BOARD_EARLY_INIT_F

/* Use SoC timer for AArch32, but architected timer for AArch64 */
#ifndef CONFIG_ARM64
#define CONFIG_SYS_TIMER_RATE 1000000
#define CONFIG_SYS_TIMER_COUNTER (&((struct bcm2835_timer_regs *)BCM2835_TIMER_PHYSADDR)->clo)
#endif

#undef DFU_ALT_INFO_MMC
#undef DFU_ALT_INFO_NOR

/*
 * 2835 is a SKU in a series for which the 2708 is the first or primary SoC,
 * so 2708 has historically been used rather than a dedicated 2835 ID.
 *
 * We don't define a machine type for bcm2709/bcm2836 since the RPi Foundation
 * chose to use someone else's previously registered machine ID (3139, MX51_GGC)
 * rather than obtaining a valid ID:-/
 *
 * For the bcm2837, hopefully a machine type is not needed, since everything
 * is DT.
 */
#ifdef CONFIG_BCM2835
#define CONFIG_MACH_TYPE MACH_TYPE_BCM2708
#endif

/* Memory layout */
#define CONFIG_NR_DRAM_BANKS 1
#define CONFIG_SYS_SDRAM_BASE 0x00000000
#ifdef CONFIG_ARM64
#define CONFIG_SYS_TEXT_BASE 0x00080000
#else
#define CONFIG_SYS_TEXT_BASE 0x00008000
#endif
#define CONFIG_SYS_UBOOT_BASE CONFIG_SYS_TEXT_BASE
/*
 * The board really has 256M. However, the VC (VideoCore co-processor) shares
 * the RAM, and uses a configurable portion at the top. We tell U-Boot that a
 * smaller amount of RAM is present in order to avoid stomping on the area
 * the VC uses.
 */
#define CONFIG_SYS_SDRAM_SIZE SZ_128M
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_MALLOC_LEN SZ_4M
#define CONFIG_SYS_MEMTEST_START 0x00100000
#define CONFIG_SYS_MEMTEST_END 0x00200000
#define CONFIG_LOADADDR 0x00200000

/* Flash */
#define CONFIG_SYS_NO_FLASH

/* Devices */
/* GPIO */
#define CONFIG_BCM2835_GPIO
/* LCD */
#define CONFIG_LCD
#define CONFIG_LCD_DT_SIMPLEFB
#define LCD_BPP LCD_COLOR16
/*
 * Prevent allocation of RAM for FB; the real FB address is queried
 * dynamically from the VideoCore co-processor, and comes from RAM
 * not owned by the ARM CPU.
 */
#define CONFIG_FB_ADDR 0
#define CONFIG_VIDEO_BCM2835
#define CONFIG_SYS_WHITE_ON_BLACK
#define CONFIG_CONSOLE_SCROLL_LINES 10

/* SD/MMC configuration */
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_MMC_SDHCI_IO_ACCESSORS
#define CONFIG_BCM2835_SDHCI

#ifdef CONFIG_CMD_USB
#define CONFIG_USB_DWC2
#ifndef CONFIG_BCM2835
#define CONFIG_USB_DWC2_REG_ADDR 0x3f980000
#else
#define CONFIG_USB_DWC2_REG_ADDR 0x20980000
#endif
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_TFTP_TSIZE
#define CONFIG_MISC_INIT_R
#define CONFIG_USB_KEYBOARD
#define CONFIG_SYS_USB_EVENT_POLL
#define CONFIG_SYS_STDIO_DEREGISTER
#endif

/* Console UART */
#ifdef CONFIG_BCM2837
#define CONFIG_BCM283X_MU_SERIAL
#else
#define CONFIG_PL01X_SERIAL
#endif
#define CONFIG_CONS_INDEX 0
#define CONFIG_BAUDRATE 115200

/* Console configuration */
#define CONFIG_SYS_CBSIZE 1024
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* File updates */
#ifdef CONFIG_UPDATE_KUBOS
#define CONFIG_SYS_DFU_DATA_BUF_SIZE 500 * SZ_1K /* File transfer chunk size */
#define CONFIG_SYS_DFU_MAX_FILE_SIZE 4 * SZ_1M   /* Maximum size for a single file.  Currently zImage (~2.5M) */

#define KUBOS_UPGRADE_DEVICE 0
#define KUBOS_UPGRADE_PART 1
#define KUBOS_UPGRADE_STORAGE CONFIG_SYS_SDRAM_BASE + 0x200 /* Temporary SDRAM storage location */

/* DFU Configuration */
#define DFU_ALT_INFO_MMC ""
#define DFU_ALT_INFO_NOR ""
#else
#define DFU_ALT_INFO_MMC ""
#define DFU_ALT_INFO_NOR ""
#endif

/* Environment */
/* #define CONFIG_ENV_SIZE SZ_16K */
#define CONFIG_EXT4_WRITE
#define CONFIG_ENV_IS_IN_EXT4    1
#define EXT4_ENV_INTERFACE       "mmc"
#define EXT4_ENV_DEVICE_AND_PART "0:3"
#define EXT4_ENV_FILE            "/uboot.env"
#define CONFIG_ENV_SIZE         10 * 1024 /* Assume sector size of 1024 */
/* #define CONFIG_ENV_VARS_UBOOT_CONFIG */
#define CONFIG_SYS_LOAD_ADDR 0x1000000
/* #define CONFIG_CONSOLE_MUX */
/* #define CONFIG_SYS_CONSOLE_IS_IN_ENV */
/* #define CONFIG_PREBOOT "usb start" */

/* Shell */
#define CONFIG_SYS_MAXARGS 16
#define CONFIG_COMMAND_HISTORY

/* Commands */
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_PART

/* ATAGs support for bootm/bootz */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

/* Environment */
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define ENV_DEVICE_SETTINGS                                                                                            \
    "stdin=serial\0"                                                                                                   \
    "stdout=serial\0"                                                                                                  \
    "stderr=serial\0"

/*
 * Memory layout for where various images get loaded by boot scripts:
 *
 * I suspect address 0 is used as the SMP pen on the RPi2, so avoid this.
 *
 * fdt_addr_r simply shouldn't overlap anything else. However, the RPi's
 *   binary firmware loads a DT to address 0x100, so we choose this address to
 *   match it. This allows custom boot scripts to pass this DT on to Linux
 *   simply by not over-writing the data at this address. When using U-Boot,
 *   U-Boot (and scripts it executes) typicaly ignore the DT loaded by the FW
 *   and loads its own DT from disk (triggered by boot.scr or extlinux.conf).
 *
 * pxefile_addr_r can be pretty much anywhere that doesn't conflict with
 *   something else. Put it low in memory to avoid conflicts.
 *
 * kernel_addr_r must be within the first 128M of RAM in order for the
 *   kernel's CONFIG_AUTO_ZRELADDR option to work. Since the kernel will
 *   decompress itself to 0x8000 after the start of RAM, kernel_addr_r
 *   should not overlap that area, or the kernel will have to copy itself
 *   somewhere else before decompression. Similarly, the address of any other
 *   data passed to the kernel shouldn't overlap the start of RAM. Pushing
 *   this up to 16M allows for a sizable kernel to be decompressed below the
 *   compressed load address.
 *
 * scriptaddr can be pretty much anywhere that doesn't conflict with something
 *   else. Choosing 32M allows for the compressed kernel to be up to 16M.
 *
 * ramdisk_addr_r simply shouldn't overlap anything else. Choosing 33M allows
 *   for any boot script to be up to 1M, which is hopefully plenty.
 */
#define ENV_MEM_LAYOUT_SETTINGS                                                                                        \
    "fdt_high=ffffffff\0"                                                                                              \
    "initrd_high=ffffffff\0"                                                                                           \
    "fdt_addr_r=0x00000100\0"                                                                                          \
    "pxefile_addr_r=0x00100000\0"                                                                                      \
    "kernel_addr_r=0x01000000\0"                                                                                       \
    "scriptaddr=0x02000000\0"                                                                                          \
    "ramdisk_addr_r=0x02100000\0"

#define BOOT_TARGET_DEVICES(func) func(MMC, mmc, 0) func(USB, usb, 0) func(PXE, pxe, na) func(DHCP, dhcp, na)

#define CONFIG_BOOTCOMMAND                                                                                             \
    "if mmc rescan; then "                                                                                             \
    "run mmc_boot; "                                                                                                   \
    "else "                                                                                                            \
    "echo ERROR: Failed to boot. Unable to communicate with SD card; "                                                 \
    "fi;"

/* Define the initial console connection and rootfs location */
#define CONFIG_BOOTARGS                                                                                                \
    "console=ttyAMA0,115200 "                                                                                          \
    "root=/dev/mmcblk0p2 rootwait"

/* (bootstrap + u-boot + dtb (+ altOS) in flash) + (env + linux in mmc) */
/* Copy .dtb file (NORFLASH @ 0x70000, size = 0x10000) and kernel (SD card, partition 5) into SDRAM, then boot them */
#define MMC_BOOT                                                                                                       \
    "mmc_boot=mmc dev 0; "                                                                                             \
    "fatload mmc 0:1 ${fdt_addr_r} bcm2708-rpi-zero.dtb; "                                                             \
    "fatload mmc 0:1 ${kernel_addr_r} kernel; "                                                                        \
    "bootm ${kernel_addr_r} - ${fdt_addr_r}\0"

#define CONFIG_EXTRA_ENV_SETTINGS                                                                                      \
    ENV_MEM_LAYOUT_SETTINGS                                                                                            \
    MMC_BOOT                                                                                                           \
    KUBOS_UPDATE_ARGS

#endif
