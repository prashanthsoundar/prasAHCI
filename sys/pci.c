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
    return result >> (8*(offset%4));
   // return result;
}

int ifMultiFunction(uint16_t bus,uint16_t device)
{
    return readPIC(bus,device,0,0x0E) & (1<<7);
}

void decToHexa(int n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[100];
    
    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;
        
        // storing remainder in temp variable.
        temp = n % 16;
        
        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
        
        n = n/16;
    }
    
    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        kprintf("%c",hexaDeciNum[j]);
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
                decToHexa((readPIC(bus,device,function,0x00)&0xFF00)>>8);
                decToHexa((readPIC(bus,device,function,0x00)&0xFF));
                kprintf("\t");
                decToHexa((readPIC(bus,device,function,0x02)&0xFF00)>>8);
                decToHexa((readPIC(bus,device,function,0x02)&0xFF));
                kprintf("\n");
              //  decToHexa(readPIC(bus,device,function,0x00));
                   // decToHexa((readPIC(bus,device,function,0x00)&&0xFF)>>8);
                   // decToHexa(readPIC(bus,device,function,0x00)&&0xFF);
//                    decToHexa((readPIC(bus,device,function,0x02)&&0xFFFF)>>8);
//                    decToHexa(readPIC(bus,device,function,0x02)&&0xFF);
//                    kprintf("\n");
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
