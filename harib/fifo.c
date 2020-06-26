// FIFO library

#include  "bootpack.h"

#define FLAGS_OVERRUN   0x0001

// Initialize FIFO buffer
void fifo8_init(FIFO8 *fifo, int size, unsigned char *buf) {
    fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; /* 空き */
	fifo->flags = 0;
	fifo->p = 0; /* 書き込み位置 */
	fifo->q = 0; /* 読み込み位置 */
	return;
}

int fifo8_put(FIFO8 *fifo, unsigned char data) {
	if (fifo->free == 0) {
		/* 空きがなくてあふれた */
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
	fifo->free--;
	return 0;
}

int fifo8_get(FIFO8 *fifo) {
	int data;
	if (fifo->free == fifo->size) {
		/* バッファが空っぽのときは、とりあえず-1が返される */
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

// return fifo->size - fifo->free
int fifo8_status(FIFO8 *fifo) {
	return fifo->size - fifo->free;
}
