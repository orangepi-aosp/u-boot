#ifndef _CONFIG_ANDROID_BOOTCMD_H
#define _CONFIG_ANDROID_BOOTCMD_H

#define MMC_DEVNUMS "0 1"

#define BOOTENV \
    ANDROID_PART_INFO(boot, 4) \
    ANDROID_PART_INFO(init_boot, 5) \
    ANDROID_PART_INFO(vendor_boot, 6) \
    ANDROID_PART_INFO(misc, 12) \
    "bootimg_addr=0x0A000000\0" \
    "initbootimg_addr=0x0C000000\0" \
    "mmc_devnums=" MMC_DEVNUMS "\0" \
    "bootcmd=" \
        "for devnum in ${mmc_devnums}; do " \
            "echo Boot from MMC${devnum}; " \
            "run mmc_android_boot; " \
        "done\0" \
    "mmc_android_boot=" \
        "env set part_loadaddr ${bootimg_addr}; " \
        "env set part_num ${part_boot_num}; " \
        "run mmc_load_part; " \
        "abootimg addr ${bootimg_addr}; " \
        "abootimg get dtb --index=0 dtb_start dtb_size; " \
        "cp.b ${dtb_start} ${fdt_addr_r} ${dtb_size}; " \
        "env set part_loadaddr ${initbootimg_addr}; " \
        "env set part_num ${part_init_boot_num}; " \
        "run mmc_load_part; " \
        "bootm ${bootimg_addr} ${initbootimg_addr} ${fdt_addr_r}; " \
        "\0" \
    "mmc_load_part=" \
        "part start mmc ${devnum} ${part_num} part_start; " \
        "part size mmc ${devnum} ${part_num} part_size; " \
        "mmc read ${part_loadaddr} ${part_start} ${part_size}; " \
        "if test $? -ne 0; then " \
            "echo ERROR: cannot load mmc partition ${part_num} to addr ${part_loadaddr}; " \
            "false; " \
        "fi\0"
        
#define ANDROID_PART_INFO(name, number) \
    "part_" #name "_num=" #number "\0"

#define BOOTLOADER_PARTS \
    "name=loader1,start=32K,size=4000K,uuid=${uuid_gpt_loader1};" \
	"name=loader2,start=8M,size=4M,uuid=${uuid_gpt_loader2};" \
	"name=trust,size=4M,uuid=${uuid_gpt_atf};"

#define ANDROID_PARTS \
	"name=boot,size=16M,bootable,uuid=${uuid_gpt_boot};" \
    "name=init_boot,size=8M,uuid=${uuid_gpt_init_boot};" \
    "name=vendor_boot,size=32M,uuid=${uuid_gpt_vendor_boot};" \
    "name=system,size=1536M,uuid=${uuid_gpt_system};" \
    "name=system_ext,size=64M,uuid=${uuid_gpt_system_ext};" \
    "name=vendor,size=256M,uuid=${uuid_gpt_vendor};" \
    "name=vendor_dlkm,size=32M,uuid=${uuid_gpt_vendor_dlkm};" \
    "name=cache,size=64M,uuid=${uuid_gpt_cache};" \
    "name=misc,size=8K,uuid=${uuid_gpt_misc};" \
	"name=userdata,size=-,uuid=${uuid_gpt_userdata}"

#define PARTITION_TABLE \
    "uuid_disk=${uuid_gpt_disk};" \
    BOOTLOADER_PARTS \
    ANDROID_PARTS \
    "\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
    ENV_MEM_LAYOUT_SETTINGS \
    BOOTENV \
    "partitions=" PARTITION_TABLE \
	"altbootcmd=fastboot usb 0\0"

#endif // _CONFIG_ANDROID_BOOTCMD_H