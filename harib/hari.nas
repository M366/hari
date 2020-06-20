; hari-os
; TAB=4

; BOOT_INFO
CYLS    EQU     0x0ff0          ; Address stored current floppy disk cylinder number
LEDS    EQU     0x0ff1          ; 
VMODE   EQU     0x0ff2          ; Address stored bit color size
SCRNX   EQU     0x0ff4          ; Address stored video resolution x
SCRNY   EQU     0x0ff6          ; Address stored video resolution y
VRAM    EQU     0x0ff8          ; Address stored starting address of graphic buffer

        ORG     0xc200          ; the program to be loaded at 0xc200

        MOV     AL, 0x13        ; Set video mode parameter. 0x13 is VGA graphics, 320x200x8bit color, packed pixel.
        MOV     AH, 0x00        ; Set video mode.
        INT     0x10            ; BIOS interrupt code 0x10. BIOS provides video sevices.
        MOV     BYTE [VMODE], 8
        MOV     WORD [SCRNX], 320
        MOV     BYTE [SCRNY], 200
        MOV     DWORD [VRAM], 0x000a0000

; get the state of the keyboard

        MOV     AH, 0x02        ; for INT 0x16, set mode: Get the State of the keyboard.
        INT     0x16            ; BIOS interrupt code 0x16. BIOS provides keyboard services.
        MOV     [LEDS], AL      ; Return AL = shift flags (keyboard state, e.g. 6 = caps lock active
        
fin:
        HLT
        JMP     fin