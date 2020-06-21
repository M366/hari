; naskfunc
; TAB=4

[FORMAT "WCOFF"]        ; set mode to make object
[BITS 32]               ; set 32bit machine lang

; for obj file

[FILE "naskfunc.nas"]   ; source file name

        GLOBAL  _io_hlt    ; Make global symbol

; function definition

[SECTION .text]         ; begin .text section

_io_hlt:                ; function label
        HLT
        RET