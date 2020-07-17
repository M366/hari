// mouse library

#include "bootpack.h"

struct FIFO32 *mousefifo;
int mousedata0;

// Interrupt from PS/2 mouse
void inthandler2c(int *esp) {
    int data;
    io_out8(PIC1_OCW2, 0x64); // notify PIC1 that IRQ-12 is accepted.
    io_out8(PIC0_OCW2, 0x62); // notify PIC0 that IRQ-02 is accepted.
    data = io_in8(PORT_KEYDAT);
    fifo32_put(&mousefifo, data);
    return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec) {
    // store the FIFO buffer
    mousefifo = fifo;
    mousedata0 = data0;
	/* マウス有効 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

    // Initialize mdec->phase. It will be receive ACK(0xfa) when success connecting to mouse
    mdec->phase = 0; // 
	return;
}


int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat) {
    if (mdec->phase == 0) {
        // check first byte. The mouse send 0xfa when connecting mouse
        if (dat == 0xfa) {
            mdec->phase = 1;
        }
        return 0;
    }
    if (mdec->phase == 1) {
        // check first byte of motion
        if ((dat & 0xc8) == 0x08) { // c8 = 1100 1000 : 1100 for not correct bit, 1000 always on.
            mdec->buf[0] = dat;
            mdec->phase = 2;
        }
        return 0;
    }
    if (mdec->phase == 2) {
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 0;
    }
    if (mdec->phase == 3) {
        mdec->buf[2] = dat;
        mdec->phase = 1;
        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];
        if ((mdec->buf[0] & 0x10) != 0) {
            mdec->x |= 0xffffff00;
        }
        if ((mdec->buf[0] & 0x20) != 0) {
            mdec->y |= 0xffffff00;
        }
        mdec->y = - mdec->y; // The Y direction of the mouse is opposite of the Y direction of the display.
        return 1;
    }
    return -1; // error
}
