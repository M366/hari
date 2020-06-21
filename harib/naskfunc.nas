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
