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

void HariMain(void) {
    char *vram;
    int xsize, ysize;

    init_palette();

    vram = (char *) 0xa0000; // VRAM address.

	xsize = 320;
	ysize = 200;

	boxfill8(vram, xsize, COL8_FFFFFF,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);

	boxfill8(vram, 320, COL8_FF0000,  20,  20, 120, 120);
	boxfill8(vram, 320, COL8_00FF00,  70,  50, 170, 150);
	boxfill8(vram, 320, COL8_0000FF, 120,  80, 220, 180);

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
