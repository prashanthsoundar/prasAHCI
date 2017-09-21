#ifndef _PCI_H
#defing _PCI_H
#include<sys/defs.h>
#include<sys/kprintf.h>

struct pci_read
{
    uint8_t zeroB:2;
    uint8_t registerOffset:6;
    uint8_t funcNum:3;
    uint8_t deviceNum:5;
    uint8_t busNum:8;
    uint8_t served:8;
    uint8_t enableBit:1;
};

void init_pci();

#endif
