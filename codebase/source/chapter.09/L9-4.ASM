;-----------------------------------------------------------
; Sorts an array of ints.  C callable (small model).  25 bytes.
; void sort( int num, int a[] );
;
; Courtesy of David Stafford.
;-----------------------------------------------------------
                 
	.model small
	.code
        public _sort
 
top:    mov     dx,[bx]         ;swap two adjacent integers
        xchg    dx,[bx+2]
        xchg    dx,[bx]

        cmp     dx,[bx]         ;did we put them in the right order?
        jl      top             ;no, swap them back

        inc     bx              ;go to next integer
        inc     bx
        loop    top

_sort:  pop     dx              ;get return address (entry point)
        pop     cx              ;get count
        pop     bx              ;get pointer
        push    bx              ;restore pointer
        dec     cx              ;decrement count
        push    cx              ;save count
        push    dx              ;restore return address
        jg      top             ;if cx > 0

        ret

	end
