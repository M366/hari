TOOLPATH = ../z_tools/
INCPATH = ../z_tools/haribote/

MAKE = $(TOOLPATH)make -r
NASK = $(TOOLPATH)nask
CC1      = $(TOOLPATH)gocc1 -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask -a
OBJ2BIM  = $(TOOLPATH)obj2bim
BIM2HRB  = $(TOOLPATH)bim2hrb
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG = $(TOOLPATH)edimg
HARITOL = $(TOOLPATH)haritol

default :
	$(MAKE) img

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

bootpack.bim : bootpack.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

hari.sys : asmhead.bin bootpack.hrb Makefile
	$(HARITOL) concat hari.sys asmhead.bin bootpack.hrb

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
	rm -rf hari.sys hari.img *.bin *.lst *.gas *.obj
	rm -rf bootpack.nas bootpack.map bootpack.bim bootpack.hrb

