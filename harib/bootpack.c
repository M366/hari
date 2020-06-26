// bootpack.c

#include "bootpack.h"
#include <stdio.h>

extern KEYBUF keybuf;

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *) ADR_BOOTINFO;
    char s[40], mcursor[256];
    int mx, my, i;

    init_gdtidt();
    init_pic();
    io_sti(); // enable CPU interrupt. this process is done after the PIC initialization.

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, 7);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, 0, s);

    io_out8(PIC0_IMR, 0xf9); // enable PIC1 and keyboard (11111001)
    io_out8(PIC1_IMR, 0xef); // enable mouse (11101111)

    for (;;) {
        io_cli(); // disenable interrupt
        if (keybuf.flag == 0) {
            io_stihlt();
        } else {
            i = keybuf.data;
            keybuf.flag = 0;
            io_sti();
			sprintf(s, "%02X", i);
			boxfill8(binfo->vram, binfo->scrnx, 7, 0, 16, 15, 31);
			putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, 0, s);
        }
    }
}


