// Graphic utilities

#include "bootpack.h"

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

void init_screen8(char *vram, int x, int y) {
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

void init_mouse_cursor8(char *mouse, char bc) {
	static char cursor[16][16] = {
		"*...............",
		"***.............",
		"*****...........",
		"*******.........",
		"*********.......",
		"***********.....",
		"*************...",
		"*********.......",
		"********........",
		"********........",
		"***.*****.......",
		"**..*****.......",
		"**...*****......",
		"*....*****......",
		"*.....*****.....",
		"......*****....."
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize) {
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

