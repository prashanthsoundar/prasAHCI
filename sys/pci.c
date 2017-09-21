#include<sys/pci.h>
#include<sys/defs.h>

void init_pci(){
    
    kprintf("%d",sizeof(struct pci_read));
    struct pci_read a={0,1,0,0,0,0,1};
    struct pci_read *p =&a ;
   // p->enableBit = 1;
    
    kprintf("\n%d\n",*p);
    
}
