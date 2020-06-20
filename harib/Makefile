TOOLPATH = ../z_tools/
MAKE = $(TOOLPATH)make -r
NASK = $(TOOLPATH)nask
EDIMG = $(TOOLPATH)edimg
HARITOL = $(TOOLPATH)haritol

default :
	$(MAKE) img

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

helloos.img : ipl.bin Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

asm :
	$(MAKE) ipl.bin

img :
	$(MAKE) helloos.img

run :
	$(MAKE) img
	$(HARITOL) concat ../z_tools/qemu/fdimage0.bin helloos.img
	$(MAKE) -C ../z_tools/qemu

clean :
	rm ipl.bin
	rm ipl.lst

src_only :
	$(MAKE) clean
	rm helloos.img
