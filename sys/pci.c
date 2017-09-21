#include<sys/pci.h>
#include<sys/defs.h>

int32_t inb_32(uint32_t port)
{
    uint32_t val;
    __asm__ __volatile__("inb %1,%0;":"=a" (val): "Nd" (port));
    return val;
}


void outb_32(uint32_t data,uint32_t port)
{
    __asm__ __volatile__("outb %0,%1;": :"a" (data),"Nd" (port));
}

void init_pci(){
    
    
    
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<32;j++)
        {
            struct pci_read pciRead={0,0x0C,0,j,i,0,1};
            outb_32(&pciRead,0xCF8);
            kprintf("%d",inb_32(0xCFC));
            
        }
        
    }
    
}
