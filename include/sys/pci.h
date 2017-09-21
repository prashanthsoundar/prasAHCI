#ifndef _PCI_H
#defing _PCI_H
#include<sys/defs.h>

struct pci_read
{
    uint8_t zeroB:2;
    unint8_t registerOffset:6;
    unint8_t funcNum:3;
    unint8_t deviceNum:5;
    unint8_t busNum:8;
    unint8_t served:8;
    uint8_t enableBit:1;
};

void init_pci();

#endif
