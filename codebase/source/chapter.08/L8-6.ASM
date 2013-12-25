; Alternative optimized assembly language version of FindIDAverage
; that requires data organized as two arrays within each block rather
; than as an array of two-value element structures. This allows the
; use of REP SCASW for ID searching.
              
SearchedForID   equ     4       ;passed parameter offsets in the
BlockPointer    equ     6       ; stack frame (skip over pushed BP
                                ; and the return address)
NextBlock       equ     0       ;field offsets in struct BlockHeader
BlockCount      equ     2
BLOCK_HEADER_SIZE equ   4       ;# of bytes in struct BlockHeader

        .model  small   
        .code
        public  _FindIDAverage2
_FindIDAverage2 proc    near
        push    bp              ;save caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    di              ;preserve C register variables
        push    si
        mov     di,ds           ;prepare for SCASW
        mov     es,di
        cld
        mov     si,[bp+BlockPointer]    ;pointer to first block
        mov     ax,[bp+SearchedForID]   ;ID we're looking for
        sub     dx,dx           ;IDMatchSum = 0
        mov     bp,dx           ;IDMatchCount = 0
                                ;***stack frame no longer available***
; Search through all of the linked blocks until the last block
; (marked with a NULL pointer to the next block) has been searched.
BlockLoop:
; Search through all the DataElement entries within this block
; and accumulate data from all that match the desired ID.
        mov     cx,[si+BlockCount]
        jcxz    DoNextBlock     ;skip this block if there's no data
                                ; to search through
        mov     bx,cx           ;we'll use BX to point to the
        shl     bx,1            ; corresponding value entry in the
                                ; case of an ID match (BX is the
                                ; length in bytes of the ID array)
; Point to the first DataElement entry within this block.
        lea     di,[si+BLOCK_HEADER_SIZE]
IntraBlockLoop:
        repnz   scasw           ;search for the ID
        jnz     DoNextBlock     ;no match, the block is done
        inc     bp              ;we have a match; IDMatchCount++;
        add     dx,[di+bx-2]    ;IDMatchSum += DataPointer->Value;
                                ; (SCASW has advanced DI 2 bytes)
        and     cx,cx           ;is there more data to search through?
        jnz     IntraBlockLoop  ;yes
; Point to the next block and continue if that pointer isn't NULL.
DoNextBlock:
        mov     si,[si+NextBlock] ;get pointer to the next block
        and     si,si           ;is it a NULL pointer?
        jnz     BlockLoop       ;no, continue
; Calculate the average of all matches.
        sub     ax,ax           ;assume we found no matches
        and     bp,bp
        jz      Done            ;we didn't find any matches, return 0
        xchg    ax,dx           ;prepare for division
        div     bp              ;return IDMatchSum / IDMatchCount
Done:   pop     si              ;restore C register variables
        pop     di
        pop     bp              ;restore caller's stack frame
        ret     
_FindIDAverage2 ENDP
        end
