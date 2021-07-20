#include <chipsets/q35.h>
#include <chipsets/q35/acpi.h>
#include <chipsets/q35/dram.h>
#include <chipsets/q35/lpc.h>
#include <cpu/pio.h>
#include <cpu/smm.h>
#include <drivers/pci.h>
#include <drivers/pic.h>
#include <drivers/ps2.h>
#include <tools/print.h>
#include <tools/string.h>

static void memory_init() {
    // Shadow and unshadow BIOS data, to avoid exploits
    q35_dram_pam_lock(5);
    q35_dram_pam_lock(6);
    q35_dram_pam_unlock(5, 1);
    q35_dram_pam_unlock(6, 1);
}

static void early_acpi_init() {
    // Enable ACPI registers
    q35_lpc_acpi_base(Q35_ACPI_BASE);
    q35_lpc_acpi_enable();
}

static void smm_init() {
    // SMRAM setup
    q35_dram_smram_open();
    memcpy((void *) (SMM_DEFAULT_SMBASE + SMM_SMBASE_HANDLER_OFFSET), smm_trampoline_start, smm_trampoline_end - smm_trampoline_start);
    memcpy((void *) (SMM_NEW_SMBASE + SMM_SMBASE_HANDLER_OFFSET), smm_trampoline_start, smm_trampoline_end - smm_trampoline_start);
    q35_dram_smram_close();
    q35_acpi_smi_enable(Q35_ACPI_SMI_EN_APMC | Q35_ACPI_SMI_EN_GLB);
    outb(0xb2, 0x01);
    memset((void *) SMM_DEFAULT_SMBASE, 0, 0x20000);
}

static void irqs_init() {
    pic_init(8, 0xa0);
    // PIRQs
    for (int i = 0; i < 8; i++) {
        q35_lpc_pirq_disable(i);
    }
    q35_lpc_pirq_route(0, Q35_LPC_PIRQ_A_IRQ);
    pic_set_level(Q35_LPC_PIRQ_A_IRQ);
    q35_lpc_pirq_route(1, Q35_LPC_PIRQ_B_IRQ);
    pic_set_level(Q35_LPC_PIRQ_B_IRQ);
    q35_lpc_pirq_route(2, Q35_LPC_PIRQ_C_IRQ);
    pic_set_level(Q35_LPC_PIRQ_C_IRQ);
    q35_lpc_pirq_route(3, Q35_LPC_PIRQ_D_IRQ);
    pic_set_level(Q35_LPC_PIRQ_D_IRQ);
    q35_lpc_pirq_route(4, Q35_LPC_PIRQ_E_IRQ);
    pic_set_level(Q35_LPC_PIRQ_E_IRQ);
    q35_lpc_pirq_route(5, Q35_LPC_PIRQ_F_IRQ);
    pic_set_level(Q35_LPC_PIRQ_F_IRQ);
    q35_lpc_pirq_route(6, Q35_LPC_PIRQ_G_IRQ);
    pic_set_level(Q35_LPC_PIRQ_G_IRQ);
    q35_lpc_pirq_route(7, Q35_LPC_PIRQ_H_IRQ);
    pic_set_level(Q35_LPC_PIRQ_H_IRQ);
    for (int i = 0; i < 8; i++) {
        q35_lpc_pirq_enable(i);
    }
    // SCI
    q35_lpc_acpi_sci_irq(9);
}

static void isa_init() {
    // PS/2 init
    if (ps2_init() != 0) {
        print("lakebios: could not initialize PS/2. Halting.");
        for (;;) {}
    }
}

static void pci_init() {
    pci_enumerate(
        Q35_PCI_MMIO_BASE, Q35_PCI_IO_BASE,
        Q35_LPC_PIRQ_A_IRQ, Q35_LPC_PIRQ_B_IRQ, Q35_LPC_PIRQ_C_IRQ, Q35_LPC_PIRQ_D_IRQ);
}

void q35_init() {
    memory_init();
    early_acpi_init();
    smm_init();
    irqs_init();
    isa_init();
    pci_init();
}
