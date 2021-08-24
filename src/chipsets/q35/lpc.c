#include <chipsets/q35.h>
#include <chipsets/q35/lpc.h>
#include <drivers/pci.h>

void q35_lpc_acpi_base(uint16_t base) {
    pci_cfg_write_dword(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_BASE, base & ~0x7f);
}

void q35_lpc_acpi_enable() {
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL,
        pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL) | Q35_LPC_ACPI_CNTL_EN);
}

void q35_lpc_acpi_disable() {
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL,
        pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL) & ~Q35_LPC_ACPI_CNTL_EN);
}

void q35_lpc_acpi_sci_irq(uint8_t irq) {
    if (irq == 20) {
        irq = 0x04;
    } else if (irq == 21) {
        irq = 0x05;
    } else {
        irq = irq - 9;
    }
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL,
        (pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, Q35_LPC_ACPI_CNTL) & ~0x07) | irq);
}

void q35_lpc_pirq_enable(int pirq) {
    int pirq_base = pirq > 3 ? Q35_LPC_PIRQ_E : Q35_LPC_PIRQ_A;
    pirq = pirq > 3 ? pirq - 4 : pirq;
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq,
        (pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq) | Q35_LPC_PIRQ_EN));
}

void q35_lpc_pirq_disable(int pirq) {
    int pirq_base = pirq > 3 ? Q35_LPC_PIRQ_E : Q35_LPC_PIRQ_A;
    pirq = pirq > 3 ? pirq - 4 : pirq;
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq,
        (pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq) & ~Q35_LPC_PIRQ_EN));
}

void q35_lpc_pirq_route(int pirq, uint8_t irq) {
    int pirq_base = pirq > 3 ? Q35_LPC_PIRQ_E : Q35_LPC_PIRQ_A;
    pirq = pirq > 3 ? pirq - 4 : pirq;
    pci_cfg_write_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq,
        (pci_cfg_read_byte(Q35_LPC_BUS, Q35_LPC_SLOT, Q35_LPC_FUNCTION, pirq_base + pirq) & ~0x0f) | irq);
}
