MAIN: add r3, LIST
.entry LOOP
LOOP: prn #48
STR: .string "abcd"
macr smacr1
 cmp r3, #-6
 bne END
endmacr
 lea STR, r6
 inc r6
 mov *r6,K
 sub r1, r4
 smacr1
dec K
 jmp LOOP
END: stop
LIST: .data 6, -9,+2
 .data -100
K: .data 31
; note
