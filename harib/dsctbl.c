// Desctiptor table utilities. e.g. GDT, IDT

#include "bootpack.h"

void init_gdtidt(void) {
    SEGMENT_DESCRIPTOR *gdt = (SEGMENT_DESCRIPTOR *) ADR_GDT;
    GATE_DESCRIPTOR *idt = (GATE_DESCRIPTOR *) ADR_IDT;
    int i;

	// GDTの初期化
	for (i = 0; i <= LIMIT_GDT / 8; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
	set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER); // segment for bootpack.hrb
	load_gdtr(LIMIT_GDT, ADR_GDT);

	// IDTの初期化
	for (i = 0; i <= LIMIT_IDT / 8; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);

	/* IDTの設定 */
    set_gatedesc(idt + 0x20, (int) asm_inthandler20, 2 * 8, AR_INTGATE32); // timer
	set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32); // keyboard
	set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32); // mouse
	set_gatedesc(idt + 0x40, (int) asm_hrb_api,      2 * 8, AR_INTGATE32); // cons_putchar

	return;
}

void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; // G_bit = 1. G bit is flag. if G bit is 1, set limit unit to `page`. 1 page equal 4KB
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff; //access_right is 12 bits. this line load 8 bits. upper 4 bits is loaded next line.
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}
