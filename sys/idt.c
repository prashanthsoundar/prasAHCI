#include <sys/defs.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/keyboard.h>
#include <sys/kprintf.h>


struct idt_descriptor {
    uint16_t id_base_lo;
    uint16_t id_sel;
    uint8_t id_always0;
    uint8_t id_flags;
    uint16_t id_base_middle;
    uint32_t id_base_high;
    uint32_t id_always00;
}__attribute__((packed));


struct idtr_t {
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));


struct idt_descriptor idt[256];
static const struct idtr_t idtp = {(sizeof(struct idt_descriptor)*256)-1,(uint64_t)idt};

void id_set_gate(uint8_t intr_num,uint64_t base_addr, uint8_t sel,uint8_t flags)
{
    idt[intr_num].id_flags = flags;
    idt[intr_num].id_always0 = 0;
    idt[intr_num].id_always00 = 0;
    idt[intr_num].id_sel = sel;
    
    //ADDRESS TO THE INTERRUPT SERVICE ROUTINE METHOD
    
    idt[intr_num].id_base_lo = (base_addr & 0xFFFF);
    idt[intr_num].id_base_middle = (base_addr >> 16) & 0xFFFF;
    idt[intr_num].id_base_high = (base_addr >> 32) & 0xFFFFFFFF;
    
}

static int shift=0,control=0;

void _key_press_handler(){
    
    unsigned char a;
    
    a = inb(0x60);
    
    kprintf("\n%d\n",a);
    
    if(a<128) {
        
        switch(a)
        {
            case 42:
                shift=1;break;
            case 29:
                control = 1;break;
            case 28:
                kprintf("Enter");break;
            case 77:
                kprintf("Right Arrow");break;
            case 75:
                kprintf("Left Arrow");break;
            case 72:
                kprintf("Up Arrow");break;
            case 80:
                kprintf("Down Arrow");break;
            case 57:
                kprintf("Space");break;
            case 58:
                kprintf("Alt");break;
            case 15:
                kprintf("Tab");break;
            case 1:
                kprintf("Esc");break;
            case 59:
                kprintf("F1");break;
            case 60:
                kprintf("F2");break;
            case 61:
                kprintf("F3");break;
            case 62:
                kprintf("F4");break;
            case 63:
                kprintf("F5");break;
            case 64:
                kprintf("F6");break;
            case 65:
                kprintf("F7");break;
            case 66:
                kprintf("F8");break;
            case 67:
                kprintf("F9");break;
            case 68:
                kprintf("F10");break;
            case 69:
                kprintf("F11");break;
            case 70:
                kprintf("F12");break;
                
            default:
                if (shift==1)
                {
                    shift = 0;
                    kprintf("shift+'%c'='%c'",kbdus[a],CAPS_kbdus[a]);
                }
                else if (control==1)
                {
                    control=0;
                    kprintf("ctrl^%c",kbdus[a]);
                    
                }
                else
                {
                    kprintf("%c",kbdus[a]);
                }
        }
    }
    
    outb(0x20,0x20);
    outb(0x20,0xA0);
}

static int i = 0;
static int j =0;

void _timer_intr_hdlr(){
    
    i++;
    
    if(i%19==0) { i = 0; kprintf("%d ",j); j++;}
    
    
    // kprintf("hi");
    outb(0x20,0x20);
    outb(0x20,0xA0);
}


void _key_board_intr();


void _timer_intr();

void init_idt()
{
    id_set_gate(33,(uint64_t)_key_board_intr,8,0x8E);
    id_set_gate(32,(uint64_t)_timer_intr,8,0x8E);
    __asm__ __volatile__("lidt %0" : : "m" (idtp));
    
}
