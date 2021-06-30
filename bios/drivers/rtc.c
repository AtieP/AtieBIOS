#include <cpu/pio.h>
#include <drivers/rtc.h>

uint8_t rtc_read(uint8_t index) {
    outb(RTC_INDEX, index | NMI_BIT);
    return inb(RTC_DATA);
}

void rtc_write(uint8_t index, uint8_t data) {
    outb(RTC_INDEX, index | NMI_BIT);
    outb(RTC_DATA, data);
}

int rtc_get_low_mem() {
    return (int) (((uint16_t) rtc_read(CMOS_LOWMEM_HI) << 8) | rtc_read(CMOS_LOWMEM_LO)) * 1024;
}

int rtc_get_ext1_mem() {
    return (int) (((uint16_t) rtc_read(CMOS_EXTMEM1_HI) << 8) | rtc_read(CMOS_EXTMEM1_LO)) * 1024;
}

int rtc_get_ext2_mem() {
    // Values together have 64 KiB granularity
    return (int) (((uint16_t) rtc_read(CMOS_EXTMEM2_HI) << 8) | rtc_read(CMOS_EXTMEM2_LO)) * 65536;
}
