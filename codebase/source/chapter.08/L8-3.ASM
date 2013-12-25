; Typically optimized assembly language version of FindIDAverage.
          
SearchedForID   equ     4       ;passed parameter offsets in the
BlockPointer    equ     6       ; stack frame (skip over pushed BP
                                ; and the return address)
NextBlock       equ     0       ;field offsets in struct BlockHeader
BlockCount      equ     2
BLOCK_HEADER_SIZE equ   4       ;# of bytes in struct BlockHeader
ID              equ     0       ;struct DataElement field offsets
Value           equ     2
DATA_ELEMENT_SIZE equ   4       ;# of bytes in struct DataElement

        .model  small   
        .code
        public  _FindIDAverage
_FindIDAverage  proc    near
        push    bp              ;save caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    di              ;preserve C register variables
        push    si
        sub     dx,dx           ;IDMatchSum = 0
        mov     bx,dx           ;IDMatchCount = 0
        mov     si,[bp+BlockPointer]    ;pointer to first block
        mov     ax,[bp+SearchedForID]   ;ID we're looking for
; Search through all of the linked blocks until the last block
; (marked with a NULL pointer to the next block) has been searched.
BlockLoop:
; Point to the first DataElement entry within this block.
        lea     di,[si+BLOCK_HEADER_SIZE]
; Search through all the DataElement entries within this block
; and accumulate data from all that match the desired ID.
        mov     cx,[si+BlockCount]
        jcxz    DoNextBlock     ;no data in this block
IntraBlockLoop:
        cmp     [di+ID],ax      ;do we have an ID match?
        jnz     NoMatch         ;no match
        inc     bx              ;we have a match; IDMatchCount++;
        add     dx,[di+Value]   ;IDMatchSum += DataPointer->Value;
NoMatch:
        add     di,DATA_ELEMENT_SIZE ;point to the next element
        loop    IntraBlockLoop
; Point to the next block and continue if that pointer isn't NULL.
DoNextBlock:
        mov     si,[si+NextBlock] ;get pointer to the next block
        and     si,si           ;is it a NULL pointer?
        jnz     BlockLoop       ;no, continue
; Calculate the average of all matches.
        sub     ax,ax           ;assume we found no matches
        and     bx,bx
        jz      Done            ;we didn't find any matches, return 0
        xchg    ax,dx           ;prepare for division
        div     bx              ;return IDMatchSum / IDMatchCount
Done:   pop     si              ;restore C register variables
        pop     di
        pop     bp              ;restore caller's stack frame
        ret     
_FindIDAverage  ENDP
        end
