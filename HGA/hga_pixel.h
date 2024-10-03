graphics mode row table macro

n = 0
rept 87				; 384 / 4

dw n * 90			; 90 bytes per row 
dw 2000h + n * 90
dw 4000h + n * 90
dw 6000h + n * 90

n++
loop

HGA does not require await vertical retrace 
