; naskfunc
; TAB=4

[FORMAT "WCOFF"]        ; set mode to make object
[instrset "i486p"]
[BITS 32]               ; set 32bit machine lang

[FILE "naskfunc.nas"]   ; source file name

        GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
        GLOBAL	_io_in8,  _io_in16,  _io_in32
        GLOBAL	_io_out8, _io_out16, _io_out32
        GLOBAL	_io_load_eflags, _io_store_eflags

[SECTION .text]         ; begin .text section

_io_hlt:                ; void io_hlt(void);
        HLT
        RET


_io_cli:	; void io_cli(void);
		CLI             ; clear interrupt flag. In other words, set interrupt flag to 0.
		RET

_io_sti:	; void io_sti(void);
		STI             ; set interrupt flag to 1.
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port 
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; set port
		MOV		AL,[ESP+8]		; set data
		OUT		DX,AL           ; send data to port
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; Push flags double-word
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; Pop flags double-word
		RET
