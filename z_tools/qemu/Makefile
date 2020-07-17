QEMU		= qemu-system-i386
QEMU_ARGS	= -m 32 -drive file=fdimage0.bin,format=raw,if=floppy -boot a -enable-kvm


default:
	$(QEMU) $(QEMU_ARGS)
