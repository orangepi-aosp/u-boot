#ifndef _CONFIG_ANDROID_BOOTCMD_H
#define _CONFIG_ANDROID_BOOTCMD_H

#define MMC_DEVNUMS "0 1"

#define BOOTENV \
    ANDROID_PART_INFO(boot, 4) \
    ANDROID_PART_INFO(dtb, 5) \
    "mmc_devnums=" MMC_DEVNUMS "\0" \
    "bootcmd=" \
        "for devnum in ${mmc_devnums}; do " \
            "echo Boot from MMC${devnum}; " \
            "run mmc_android_boot; " \
        "done\0" \
    "mmc_android_boot=" \
        "env set part_loadaddr ${kernel_addr_r}; " \
        "env set part_num ${part_boot_num}; " \
        "run mmc_load_part; " \
        "abootimg get dtb --index=0 dtb_start dtb_size; " \
        "cp.b ${dtb_start} ${fdt_addr_r} ${dtb_size}; " \
        "bootm ${kernel_addr_r} ${kernel_addr_r} ${fdt_addr_r}; " \
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

#define ANDROID_PARTS \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=loader1,start=32K,size=4000K,uuid=${uuid_gpt_loader1};" \
	"name=loader2,start=8M,size=4M,uuid=${uuid_gpt_loader2};" \
	"name=trust,size=4M,uuid=${uuid_gpt_atf};" \
	"name=boot,size=16M,bootable,uuid=${uuid_gpt_boot};" \
    "name=init_boot,size=4M,uuid=${uuid_gpt_init_boot};" \
    "name=system,size=1536M,uuid=${uuid_gpt_system};" \
    "name=system_ext,size=64M,uuid=${uuid_gpt_system_ext};" \
    "name=vendor,size=256M,uuid=${uuid_gpt_vendor};" \
    "name=vendor_dlkm,size=32M,uuid=${uuid_gpt_vendor_dlkm};" \
	"name=userdata,size=-,uuid=${uuid_gpt_userdata}\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
    ENV_MEM_LAYOUT_SETTINGS \
    BOOTENV \
    "partitions=" ANDROID_PARTS \
	"altbootcmd=fastboot usb 0\0"

#endif // _CONFIG_ANDROID_BOOTCMD_H