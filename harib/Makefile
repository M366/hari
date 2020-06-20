TOOLPATH = ../z_tools/
MAKE = $(TOOLPATH)make -r
NASK = $(TOOLPATH)nask
EDIMG = $(TOOLPATH)edimg
HARITOL = $(TOOLPATH)haritol

default :
	$(MAKE) img

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

hari.sys : hari.nas Makefile
	$(NASK) hari.nas hari.sys hari.lst

hari.img : ipl.bin hari.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:hari.sys to:@: \
		imgout:hari.img

asm :
	$(MAKE) ipl.bin

img :
	$(MAKE) hari.img

run :
	$(MAKE) img
	$(HARITOL) concat ../z_tools/qemu/fdimage0.bin hari.img
	$(MAKE) -C ../z_tools/qemu

clean :
	rm -rf ipl.bin ipl.lst hari.sys hari.lst hari.img

