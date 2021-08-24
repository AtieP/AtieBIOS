#ifndef __CHIPSETS_I440FX_PMC_H__
#define __CHIPSETS_I440FX_PMC_H__

#define I440FX_PMC_BUS 0
#define I440FX_PMC_SLOT 0
#define I440FX_PMC_FUNCTION 0

#define I440FX_PCI_MMIO_BASE 0xc0000000
#define I440FX_PCI_IO_BASE 0x1000

#define I440FX_PMC_PAM0 0x59
#define I440FX_PMC_PAM_UNLOCK 0x03

#define I440FX_PMC_SMRAM 0x72
#define I440FX_PMC_SMRAM_OPEN (1 << 6)
#define I440FX_PMC_SMRAM_CLOSE (1 << 5)
#define I440FX_PMC_SMRAM_LOCK (1 << 4)
#define I440FX_PMC_SMRAM_EN (1 << 3)
#define I440FX_PMC_SMRAM_DEFBASE 0x02

void i440fx_pmc_pam_lock(int pam);
void i440fx_pmc_pam_unlock(int pam, int copy_to_ram);

void i440fx_pmc_smram_open();
void i440fx_pmc_smram_close();
void i440fx_pmc_smram_lock();

#endif
