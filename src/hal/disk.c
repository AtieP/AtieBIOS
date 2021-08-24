#include <stddef.h>
#include <drivers/ata_common.h>
#include <hal/disk.h>
#include <tools/alloc.h>
#include <tools/print.h>
#include <tools/string.h>

static struct disk_abstract disk_inventory[256] = {0};
static size_t disk_flp_dl = 0x00;
static size_t disk_hdd_dl = 0x80;

static const char *disk_type_to_name(int type) {
    switch (type) {
        case HAL_DISK_AHCI:
            return "AHCI disk";
        case HAL_DISK_NVME:
            return "NVME disk";
        case HAL_DISK_FLP:
            return "Floppy disk";
        default:
            return "Unknown disk";
    }
}

void hal_disk_submit(struct disk_abstract *disk, int flp) {
    print("HAL: Submitting a: %s", disk_type_to_name(disk->interface));
    if (!flp) {
        memcpy(&disk_inventory[disk_hdd_dl], disk, sizeof(struct disk_abstract));
        disk_inventory[disk_hdd_dl].present = 1;
        disk_hdd_dl++;
    } else {
        memcpy(&disk_inventory[disk_flp_dl], disk, sizeof(struct disk_abstract));
        disk_inventory[disk_flp_dl].present = 1;
        disk_flp_dl++;
    }
}

int hal_disk_rw(uint8_t bios_dl, void *buf, uint64_t lba, int len, int write) {
    if (!disk_inventory[bios_dl].present) {
        return -1;
    }
    struct disk_abstract *disk_abstract = &disk_inventory[bios_dl];
    int (*rw)(struct disk_abstract *this, void *buf, uint64_t lba, int len, int write) =
        disk_abstract->ops.rw;
    if (!rw) {
        return HAL_DISK_ENOIMPL;
    }
    return rw(disk_abstract, buf, lba, len, write);
}

struct disk_abstract *hal_disk_get(uint8_t bios_dl) {
    return &disk_inventory[bios_dl].present ? &disk_inventory[bios_dl] : NULL;
}
