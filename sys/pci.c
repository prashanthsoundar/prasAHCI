#include<sys/pci.h>
#include<sys/defs.h>

uint32_t calConfigAddressSpace(struct pci_read pciRead)
{
    return (uint32_t)(1<<31|(0xFF&pciRead.busNum)<<16|(0x1F&pciRead.deviceNum)<<11|(0x07&pciRead.funcNum)<<8|(pciRead.registerOffset&0xFC));
   
}

int32_t inb_32(uint16_t port)
{
    uint32_t val;
    __asm__ __volatile__("inl %1,%0;":"=a" (val): "Nd" (port));
    return val;
}


void outb_32(uint32_t data,uint16_t port)
{
    __asm__ __volatile__("outl %0,%1;": :"a" (data),"Nd" (port));
}

int deviceHasFunctions(uint8_t device,uint8_t bus)
{
    struct pci_read pciRead={0,0x0E,0,device,bus,0,1};
    outb_32(calConfigAddressSpace(pciRead),0xCF8);
    
    kprintf("\n%d\n",inb_32(0xCFC)>>(8*(pciRead.registerOffset%4)));
    
    return (inb_32(0xCFC)>>(8*(pciRead.registerOffset%4))) & (1<<7);
}

void init_pci(){
    
    
    
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<32;j++)
        {
            kprintf("\n%d\n",deviceHasFunctions(j,i));
            break;
            //kprintf("\n%d\n",inb_32(0xCFC)>>(8*(pciRead.registerOffset%4)));
            
        }
        
    }
    
}
