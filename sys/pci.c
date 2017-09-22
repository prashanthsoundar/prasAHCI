#include<sys/pci.h>
#include<sys/defs.h>

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

uint32_t readPIC(uint16_t bus,uint16_t device,uint16_t function,uint32_t offset)
{
    uint32_t command = (uint32_t)((0x01<<31)|(bus&0xFF)<<16|(device&0x1F)<<11|(function&0x07)<<8|(offset&0xFC));
    outb_32(command,PCIcommandPort);
    uint32_t result = inb_32(PCIdataPort);
    return result >> (8*(offset%4));
}

int ifMultiFunction(uint16_t bus,uint16_t device)
{
    return readPIC(bus,device,0,0x0E) & (1<<7);
}

void writePIC(uint16_t bus,uint16_t device,uint16_t function,uint32_t offset,uint32_t value)
{
    uint32_t command = (uint32_t)((0x01<<31)|(bus&0xFF)<<16|(device&0x1F)<<11|(function&0x07)<<8|(offset&0xFC));
    outb_32(command,PCIcommandPort);
    outb_32(value,PCIdataPort);
}

void printALLDrivers()
{
    for (int bus=0;bus<8;bus++)
    {
        for(int device=0;device<32;device++)
        {
            int multiFunction = ifMultiFunction(bus,device)>0?8:1;
            for(int function =0;function<multiFunction;function++)
            {
                uint32_t vendorID = readPIC(bus,device,function,0x00);
                
                if(vendorID==0xFFFF||vendorID==0x0000) break;
                    kprintf("Vendor ID: %x",(readPIC(bus,device,function,0x00)&&0xFF)>>8);
                    kprintf("%x ",readPIC(bus,device,function,0x02)&&0xFF);
                    kprintf("Device ID: %x",(readPIC(bus,device,function,0x02)&&0xFF)>>8);
                    kprintf("%x ",readPIC(bus,device,function,0x02)&&0xFF);
//                    kprintf("Class ID: %x\n",readPIC(bus,device,function,0x0B));
//                    kprintf("Subclass ID: %x\n",readPIC(bus,device,function,0x0A));
//                    kprintf("Interface ID: %x\n",readPIC(bus,device,function,0x09));
//                    kprintf("Revision: %x\n",readPIC(bus,device,function,0x08));
//                    kprintf("Interrupt: %x\n",readPIC(bus,device,function,0x3C));
//                    kprintf("Vendor ID: %x\n",readPIC(bus,device,function,0x00));
//                    kprintf("Vendor ID: %x\n",readPIC(bus,device,function,0x00));
                }
            }
        }
        
}
