; hari-os
; TAB=4

        ORG     0xc200          ; the program to be loaded at 0xc200

        MOV     AL, 0x13        ; Set video mode parameter. 0x13 is VGA graphics, 320x200x8bit color, packed pixel.
        MOV     AH, 0x00        ; Set video mode.
        INT     0x10            ; BIOS interrupt code 0x10. BIOS provides video sevices.
fin:
        HLT
        JMP     fin