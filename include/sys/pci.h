#ifndef _PCI_H
#define _PCI_H
#include<sys/defs.h>
#include<sys/kprintf.h>

struct pci_read
{
    uint8_t zeroB:2;
    uint8_t registerOffset:6;
    uint8_t funcNum:3;
    uint8_t deviceNum:5;
    uint8_t busNum:8;
    uint8_t reserved:7;
    uint8_t enableBit:1;
}__attribute__((packed));

int32_t inb_32(uint16_t port);
void outb_32(uint32_t data,uint16_t port);

void init_pci();

#endif
