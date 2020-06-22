#include <stdio.h>

//
// naskfunc.nas
//

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

//
// bootpack.c
// 

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
    char s[40];

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	putfont8_asc(binfo->vram, binfo->scrnx,  8, 8, 0, "ABC 123");
	putfont8_asc(binfo->vram, binfo->scrnx,  31, 31, 0, "HariOS");
	putfont8_asc(binfo->vram, binfo->scrnx,  30, 30, 8, "HariOS");
    sprintf(s, "scrnx = %d", binfo->scrnx);
    putfont8_asc(binfo->vram, binfo->scrnx, 16, 64, 0, s);

    for (;;) {
        io_hlt();
    }
}

void init_palette(void) {
    static unsigned char table_rgb[16 * 3] = {
		0x77, 0x77, 0x77,	/*  0:黒 */
		0xf7, 0x37, 0x54,	/*  1:明るい赤 */
		0x73, 0x91, 0x01,	/*  2:明るい緑 */
		0x50, 0x3d, 0x15,	/*  3:明るい黄色 */
		0x53, 0x80, 0x91,	/*  4:明るい青 */
		0x5b, 0x34, 0x5e,	/*  5:明るい紫 */
		0x53, 0x80, 0x91,	/*  6:明るい水色 */
		0xef, 0xef, 0xef,	/*  7:白 */
		0xba, 0xba, 0xba,	/*  8:明るい灰色 */
		0xd6, 0x5e, 0x76,	/*  9:暗い赤 */
		0x9c, 0xc7, 0x02,	/* 10:暗い緑 */
		0x94, 0x75, 0x55,	/* 11:暗い黄色 */
		0x9d, 0xb3, 0xcd,	/* 12:暗い青 */
		0xa1, 0x84, 0xa4,	/* 13:暗い紫 */
		0x85, 0xb2, 0xaa,	/* 14:暗い水色 */
		0x99, 0x99, 0x99	/* 15:暗い灰色 */
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb) {
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void init_screen(char *vram, int x, int y) {
	boxfill8(vram, x, COL8_FFFFFF,  0,         0,          x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,         y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,         y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,         y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,         y - 24, 59,         y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,         y - 24,  2,         y -  4);
	boxfill8(vram, x, COL8_848484,  3,         y -  4, 59,         y -  4);
	boxfill8(vram, x, COL8_848484, 59,         y - 23, 59,         y -  5);
	boxfill8(vram, x, COL8_000000,  2,         y -  3, 59,         y -  3);
	boxfill8(vram, x, COL8_000000, 60,         y - 24, 60,         y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);

	// boxfill8(vram, x, COL8_FF0000,  20,  30, 120, 130);
	// boxfill8(vram, x, COL8_00FF00,  70,  60, 170, 160);
	// boxfill8(vram, x, COL8_0000FF, 120,  90, 220, 190);

    return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font) {
    int i;
    char *p, d;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfont8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s) {
    extern char hankaku[4096];
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;
    }
    return;
}

