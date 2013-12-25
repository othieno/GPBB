;--------------------------------------------
; Find the greatest or smallest unsigned int.
; C callable (small model).
; 24 bytes.
; By David Stafford.
; unsigned hi( int num, unsigned a[] );
; unsigned lo( int num, unsigned a[] );
;--------------------------------------------
 
                public _hi, _lo
 
_hi:            db      0b9h            ;mov cx,immediate
_lo:            xor     cx,cx
 
                pop     ax              ;get return address
                pop     dx              ;get count
                pop     bx              ;get pointer
                push    bx              ;restore pointer
                push    dx              ;restore count
                push    ax              ;restore return address
 
save:           mov     ax,[bx]
top:            cmp     ax,[bx]
                jcxz    around
                cmc
around:         ja      save
                inc     bx
                inc     bx
                dec     dx
                jnz     top
 
                ret

