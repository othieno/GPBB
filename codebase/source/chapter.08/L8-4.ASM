; Heavily optimized assembly language version of FindIDAverage.
; Features an unrolled loop and more efficient pointer use.
       
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
        mov     di,ds           ;prepare for SCASW
        mov     es,di
        cld
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
        mov     cx,[si+BlockCount] ;# of elements in this block
        jcxz    DoNextBlock     ;skip this block if it's empty
        mov     bp,cx           ;***stack frame no longer available***
        add     cx,7
        shr     cx,1            ;# of repetitions of the unrolled
        shr     cx,1            ; loop = (BlockCount + 7) / 8
        shr     cx,1
        and     bp,7            ;generate the entry point for the
        shl     bp,1            ; first, possibly partial pass through
        jmp     cs:[LoopEntryTable+bp] ; the unrolled loop and
                                ; vector to that entry point
        align   2
LoopEntryTable  label   word
        dw      LoopEntry8,LoopEntry1,LoopEntry2,LoopEntry3
        dw      LoopEntry4,LoopEntry5,LoopEntry6,LoopEntry7

M_IBL   macro   P1
        local   NoMatch
LoopEntry&P1&:
        scasw                   ;do we have an ID match?
        jnz     NoMatch         ;no match
                                ;we have a match
        inc     bx              ;IDMatchCount++;
        add     dx,[di]         ;IDMatchSum += DataPointer->Value;
NoMatch:
        add     di,DATA_ELEMENT_SIZE-2 ;point to the next element
                                ; (SCASW advanced 2 bytes already)
        endm

        align   2
IntraBlockLoop:
        M_IBL   8
        M_IBL   7
        M_IBL   6
        M_IBL   5
        M_IBL   4
        M_IBL   3
        M_IBL   2
        M_IBL   1
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