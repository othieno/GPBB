;
;          Opt2         Final optimization word count
;          Written by   Michael Abrash
;          Modified by  Willem Clements
;                       C/ Moncayo 5,  Laurel de la Reina
;                       18140 La Zubia
;                       Granada
;                       Spain
;                       Tel 34-58-890398
;                       Fax 34-58-224102
;
parms          struc
               dw         2 dup(?)
buffer         dw         ?
bufferlength   dw         ?
charflag       dw         ?
wordcount      dw         ?
parms          ends
               .model     small
               .data
charstatustable label byte
               rept       2
               db         39 dup(0)
               db         1
               db         8 dup(0)
               db         10 dup(1)
               db         7 dup(0)
               db         26 dup(1)
               db         6 dup(0)
               db         26 dup(1)
               db         5 dup(0)
               endm
               .code
               public     _ScanBuffer
_ScanBuffer    proc       near
               push       bp
               mov        bp,sp
               push       si
               push       di
               mov        si,[bp+buffer]
               mov        bx,[bp+charflag]
               mov        al,[bx]
               mov        cx,[bp+bufferlength]
               mov        bx,offset charstatustable
               xor        di,di                 ; set wordcount to zero
               shr        cx,1                  ; change count to wordcount
               jc         oddentry              ; odd number of bytes to process
               cmp        al,01h                ; check if last one is char
               jne        scanloop4             ; if not so, search for char
               jmp        scanloop1             ; if so, search for zero
oddentry:      xchg       al,ah                 ; last one in ah
               lodsb                            ; get first byte
               inc        cx
               cmp        ah,01h                ; check if last one was char
               jne        scanloop5             ; if not so, search for char
               jmp        scanloop2             ; if so, search for zero
;
;              locate the end of a word
;
scanloop1:     lodsw                            ; get two chars
               xlat                             ; translate first
               xchg       al,ah                 ; first in ah
scanloop2:     xlat                             ; translate second
               dec        cx                    ; count down
               jz         done1                 ; no more bytes left
               cmp        ax,0101h              ; check if two chars
               je         scanloop1             ; go for next two bytes
               inc        di                    ; increase wordcount
               cmp        al,01h                ; check if new word started
               je         scanloop1             ; locate end of word
;
;              locate the begin of a word
;
scanloop4:     lodsw                            ; get two chars
               xlat                             ; translate first
               xchg       al,ah                 ; first in ah
scanloop5:     xlat                             ; translate second
               dec        cx                    ; count down
               jz         done2                 ; no more bytes left
               cmp        ax,0                  ; check if word started
               je         scanloop4             ; if not, locate begin
               cmp        al,01h                ; check one-letter word
               je         scanloop1             ; if not, locate end of word
               inc        di                    ; increase wordcount
               jmp        scanloop4             ; locate begin of next word
done1:         cmp        ax,0101h              ; check if end-of-word
               je         done                  ; if not, we have finished
               inc        di                    ; increase wordcount
               jmp        done
done2:         cmp        ax,0100h              ; check for one-letter word
               jne        done                  ; if not, we have finished
               inc        di                    ; increase wordcount
done:          mov        si,[bp+charflag]
               mov        [si],al
               mov        bx,[bp+wordcount]
               mov        ax,[bx]
               mov        dx,[bx+2]
               add        di,ax
               adc        dx,0
               mov        [bx],di
               mov        [bx+2],dx
               pop        di
               pop        si
               pop        bp
               ret
_ScanBuffer    endp
               end

