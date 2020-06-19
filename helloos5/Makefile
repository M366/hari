
# デフォルト動作

default :
	../z_tools/make -r img

# ファイル生成規則

ipl.bin : ipl.nas Makefile
	../z_tools/nask ipl.nas ipl.bin ipl.lst

helloos.img : ipl.bin Makefile
	../z_tools/edimg   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

# コマンド

asm :
	../z_tools/make -r ipl.bin

img :
	../z_tools/make -r helloos.img

run :
	../z_tools/make -r img
	../z_tools/haritol concat ../z_tools/qemu/fdimage0.bin helloos.img
	../z_tools/make -r -C ../z_tools/qemu

install :
	../z_tools/make -r img
	../z_tools/imgtol.com w a: helloos.img

clean :
	-del ipl.bin
	-del ipl.lst

src_only :
	../z_tools/make -r clean
	-del helloos.img
