; hari-ipl
; TAB=4

CYLS	EQU 	10				; Constant for cylinder number.

; On an IBM PC compatible machine, the BIOS selects a boot device,
; then copies the first sector from the device, into physical memory at memory address 0x7C00.
		ORG		0x7c00			; ORG defines where the program to be loaded into memory.

; Following lines are setup for standard FAT 12 formated floppy disk setup.

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
		DW		512				; 1セクタの大きさ（512にしなければいけない）
		DB		1				; クラスタの大きさ（1セクタにしなければいけない）
		DW		1				; FATがどこから始まるか（普通は1セクタ目からにする）
		DB		2				; FATの個数（2にしなければいけない）
		DW		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
		DW		2880			; このドライブの大きさ（2880セクタにしなければいけない）
		DB		0xf0			; メディアのタイプ（0xf0にしなければいけない）
		DW		9				; FAT領域の長さ（9セクタにしなければいけない）
		DW		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
		DW		2				; ヘッドの数（2にしなければいけない）
		DD		0				; パーティションを使ってないのでここは必ず0
		DD		2880			; このドライブ大きさをもう一度書く
		DB		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
		DD		0xffffffff		; たぶんボリュームシリアル番号
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

		MOV		AX,0x0820		; `ES:BX` is buffer address. Buffer address = ES * 16 + BX. So, in the entry, `ES:BX` is 0x8200. 
		MOV		ES,AX			; The data on the floppy disk is loaded to the buffer address.
		MOV		CH,0			; Set cylinder number to 0
		MOV		DH,0			; Set head number to 0. 0: front-side
		MOV		CL,2			; Set sector number to 2.
readloop:
		MOV		SI, 0			; SI: error counter ( in the context)
retry:
		MOV		AH,0x02			; Set mode for INT 0x13. AH=0x02: read sectors from drive.
		MOV		AL,1			; Set sector size to 1.
		MOV		BX,0			; Set lower bit of buffer address (BX) to 0.
		MOV		DL,0x00			; Set drive number to 0. 0: `A` drive = floppy disk drive.
		INT		0x13			; BIOS interrupt call 0x13. INT 0x13 writes a return valude 0 or error code to the CF.
		JNC		next			; If CF has not 0, then jump to next. JNC: jump if not carry

		ADD		SI, 1			; 
		CMP		SI, 5			; 
		JAE		error			; If SI >= 5, then jump to error. JAE: jump if above or equal.

		MOV		AH, 0x00		; Set mode for INT 0x13. AH=0x00: Reset disk system.
		MOV		DL, 0x00		; Set drive number to 0. 0: `A` drive = floppy disk drive.
		INT		0x13			; Reset drive.
		JMP		retry
next:
		MOV		AX, ES			; Add 0x200 to buffer address stored `ES:BX`.
		ADD		AX, 0x0020		; Because we can't `ADD ES, 0x020`, we use the 3 lines.
		MOV		ES, AX			; 
		ADD		CL, 1
		CMP		CL, 18
		JBE		readloop		; if CL <= 18, then jump to readloop. JBE: jump if below or equal.
		MOV		CL, 1
		ADD		DH, 1
		CMP		DH, 2
		JB 		readloop		; if DH < 2 then jump to readloop
		MOV 	DH, 0			; else assign 0 to DH
		ADD		CH, 1
		CMP		CH, CYLS
		JB		readloop		; if CH < CYLS then jump to readloop

		JMP		0xc200 			; jump to 0xc200 for run hariOS. 0xc200 = 0x8000 + 0x4200

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SIに1を足す
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 一文字表示ファンクション
		MOV		BX,15			; カラーコード
		INT		0x10			; ビデオBIOS呼び出し
		JMP		putloop
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
