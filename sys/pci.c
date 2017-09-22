#include<sys/pci.h>
#include<sys/defs.h>

uint16_t dataPort = 0xCFC;
uint16_t commandPort = 0xCF8;

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
    outb_32(command,commandPort);
    uint32_t result = inb_32(dataPort);
    //return result >> (8*(offset%4));
    return result;
}

int ifMultiFunction(uint16_t bus,uint16_t device)
{
    return readPIC(bus,device,0,0x0E) & (1<<7);
}

void writePIC(uint16_t bus,uint16_t device,uint16_t function,uint32_t offset,uint32_t value)
{
    uint32_t command = (uint32_t)((0x01<<31)|(bus&0xFF)<<16|(device&0x1F)<<11|(function&0x07)<<8|(offset&0xFC));
    outb_32(command,commandPort);
    outb_32(value,dataPort);
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
                
                if((vendorID&0xFFFF)==0xFFFF) break;
                kprintf("Bus %d Device %d Function %d \n",bus,device,function);
                kprintf("%x\n",(readPIC(bus,device,function,0x00)));
                kprintf("%x\n",(readPIC(bus,device,function,0x08)));
                }
            }
        }
        
}
