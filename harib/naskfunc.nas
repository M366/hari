; naskfunc
; TAB=4

[FORMAT "WCOFF"]        ; set mode to make object
[instrset "i486p"]
[BITS 32]               ; set 32bit machine lang

[FILE "naskfunc.nas"]   ; source file name

        GLOBAL  _io_hlt,_write_mem8    ; Make global symbol

[SECTION .text]         ; begin .text section

_io_hlt:                ; void io_hlt(void);
        HLT
        RET

_write_mem8:            ; void write_mem8(int addr, int data);
        mov     ecx, [esp + 4]  ; store the addr to ecx
        mov     al, [esp + 8]   ; store the data to al
        mov     [ecx], al
        ret