; hari-os boot asm
; TAB=4

BOTPAK	EQU		0x00280000		; bootpackのロード先
DSKCAC	EQU		0x00100000		; ディスクキャッシュの場所
DSKCAC0	EQU		0x00008000		; ディスクキャッシュの場所（リアルモード）

; BOOT_INFO
CYLS    EQU     0x0ff0          ; Address stored current floppy disk cylinder number
LEDS    EQU     0x0ff1          ; 
VMODE   EQU     0x0ff2          ; Address stored bit color size
SCRNX   EQU     0x0ff4          ; Address stored video resolution x
SCRNY   EQU     0x0ff6          ; Address stored video resolution y
VRAM    EQU     0x0ff8          ; Address stored starting address of graphic buffer

        ORG     0xc200          ; the program to be loaded at 0xc200

; Set video mode
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
        
; PICが一切の割り込みを受け付けないようにする
;	AT互換機の仕様では、PICの初期化をするなら、
;	こいつをCLI前にやっておかないと、たまにハングアップする
;	PICの初期化はあとでやる

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; OUT命令を連続させるとうまくいかない機種があるらしいので
		OUT		0xa1,AL

		CLI						; さらにCPUレベルでも割り込み禁止

; CPUから1MB以上のメモリにアクセスできるように、A20GATEを設定

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20 to use over 1 MB of memory unit.
		OUT		0x60,AL
		CALL	waitkbdout      ; wait enable A20

; プロテクトモード移行

[INSTRSET "i486p"]				; 486の命令まで使いたいという記述

		LGDT	[GDTR0]			; 暫定GDTを設定
		MOV		EAX,CR0         ; CR: Control register
		AND		EAX,0x7fffffff	; bit31を0にする（ページング禁止のため）
		OR		EAX,0x00000001	; bit0を1にする（プロテクトモード移行のため）
		MOV		CR0,EAX
		JMP		pipelineflush   ; we must jump after changing protected virtual address mode
pipelineflush:
		MOV		AX,1*8			;  読み書き可能セグメント32bit 0x0008 represent `gdt + 1` segment.
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootpackの転送

		MOV		ESI,bootpack	; 転送元
		MOV		EDI,BOTPAK		; 転送先
		MOV		ECX,512*1024/4
		CALL	memcpy          ; memcpy( src, dist, double-word size)

; ついでにディスクデータも本来の位置へ転送

; まずはブートセクタから

		MOV		ESI,0x7c00		; 転送元
		MOV		EDI,DSKCAC		; 転送先
		MOV		ECX,512/4
		CALL	memcpy

; 残り全部

		MOV		ESI,DSKCAC0+512	; 転送元
		MOV		EDI,DSKCAC+512	; 転送先
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; シリンダ数からバイト数/4に変換
		SUB		ECX,512/4		; IPLの分だけ差し引く
		CALL	memcpy

; asmheadでしなければいけないことは全部し終わったので、
;	あとはbootpackに任せる

; bootpackの起動

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 転送するべきものがない
		MOV		ESI,[EBX+20]	; 転送元
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 転送先
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; スタック初期値
		JMP		DWORD 2*8:0x0000001b

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; ANDの結果が0でなければwaitkbdoutへ
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 引き算した結果が0でなければmemcpyへ
		RET
; memcpyはアドレスサイズプリフィクスを入れ忘れなければ、ストリング命令でも書ける

		ALIGNB	16
GDT0:
		RESB	8				; ヌルセレクタ
		DW		0xffff,0x0000,0x9200,0x00cf	; 読み書き可能セグメント32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 実行可能セグメント32bit（bootpack用）

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
