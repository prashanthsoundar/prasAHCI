#include<sys/pci.h>
#include<sys/defs.h>

void init_pci(){
    
    kprintf("%d",sizeof(struct pci_read));
    
    struct pci_read p;
    p->enableBit = 1;
    
    kprintf("\n%d\n",p);
    
}
