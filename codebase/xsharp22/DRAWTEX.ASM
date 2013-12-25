; Draws all pixels in the specified column, with the pixel colors
; taken from the specified texture map.  Uses approach of pre-stepping
; 1/2 pixel into the source image and rounding to the nearest source
; pixel at each step, so that texture maps will appear reasonably similar
; at all angles.  This routine is specific to 320-pixel-wide planar
; (non-Chain4) 256-color modes, such as mode X, which is a planar
; (non-chain4) 256-color mode with a resolution of 320x240.
;
; SS must == DS so that the texture map can be accessed via SS:BP.
;
; C near-callable as:
;     void ScanOutLine(EdgeScan * TopEdge, EdgeScan * BottomEdge);
;
; Tested with TASM 4.0.
;

SC_INDEX equ    03c4h   ;Sequence Controller Index
MAP_MASK equ    02h     ;index in SC of Map Mask register
SCREEN_SEG equ  0a000h  ;segment of display memory in mode X
SCREEN_WIDTH equ 80     ;width of screen in bytes from one scan line
                        ; to the next

        .model  small
        .386
        .data
        extrn   _TexMapBits:word, _TexMapWidth:word, _DestX:word
        extrn   _CurrentPageBase:word, _ClipMinX:word
        extrn   _ClipMinY:word, _ClipMaxX:word, _ClipMaxY:word

; Describes the current location and stepping, in both the source and
; the destination, of an edge. Mirrors structure in DRAWTEXP.C.
EdgeScan struc
Direction dw    ?       ;through edge list; 1 for a right edge (forward
                        ; through vertex list), -1 for a left edge (backward
                        ; through vertex list)
RemainingScans dw ?     ;height left to scan out in dest
CurrentEnd dw   ?       ;vertex # of end of current edge
SourceX dd      ?       ;X location in source for this edge
SourceY dd      ?       ;Y location in source for this edge
SourceStepX dd  ?       ;X step in source for Y step in dest of 1
SourceStepY dd  ?       ;Y step in source for Y step in dest of 1
                        ;variables used for all-integer Bresenham's-type
                        ; X stepping through the dest, needed for precise
                        ; pixel placement to avoid gaps
DestY   dw      ?       ;current Y location in dest for this edge
DestYIntStep dw ?       ;whole part of dest Y step per column X step
DestYDirection dw ?     ;-1 or 1 to indicate which way Y steps (left/right)
DestYErrTerm dw ?       ;current error term for dest Y stepping
DestYAdjUp dw   ?       ;amount to add to error term per scan line move
DestYAdjDown dw ?       ;amount to subtract from error term when the
                        ; error term turns over
EdgeScan ends

Parms   struc
          dw      2 dup(?) ;return address & pushed BP
TopEdge  dw      ?        ;pointer to EdgeScan structure for left edge
BottomEdge dw      ?        ;pointer to EdgeScan structure for right edge
Parms   ends

;Offsets from BP in stack frame of local variables.
lSourceX        equ     -4      ;current X coordinate in source image
lSourceY        equ     -8      ;current Y coordinate in source image
lSourceStepX    equ     -12     ;X step in source image for X dest step of 1
lSourceStepY    equ     -16     ;Y step in source image for X dest step of 1
lXAdvanceByOne  equ     -18     ;used to step source pointer 1 pixel
                                ; incrementally in X
lBaseAdvance   equ      -20     ;use to step source pointer minimum number of
                                ; pixels incrementally in X+Y simultaneously
lYAdvanceByOne  equ     -22     ;used to step source pointer 1 pixel
                                ; incrementally in Y
LOCAL_SIZE      equ     22      ;total size of local variables

        .code
        extrn   _FixedMul:near, _FixedDiv:near

        align   2
ToScanDone:
        jmp     ScanDone

        public  _ScanOutLine
        align   2
_ScanOutLine    proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        sub     sp,LOCAL_SIZE   ;allocate space for local variables
        push    si              ;preserve caller's register variables
        push    di

; Nothing to do if destination is fully Y clipped.

        mov     di,[bp].BottomEdge
        mov     si,[di].DestY
        cmp     si,[_ClipMinY]
        jle     short ToScanDone ;bottom edge is to left of clip rect, so done
        mov     bx,[bp].TopEdge
        mov     dx,[bx].DestY
        cmp     dx,[_ClipMaxY]
        jge     short ToScanDone ;top edge is to right of clip rect, so done
        sub     si,dx            ;destination fill height
        jle     short ToScanDone ;null or negative full height, so done

        mov     ax,word ptr [bx].SourceX        ;initial source X coordinate
        mov     word ptr [bp].lSourceX,ax
        mov     ax,word ptr [bx].SourceX+2
        mov     word ptr [bp].lSourceX+2,ax

        mov     ax,word ptr [bx].SourceY        ;initial source Y coordinate
        mov     word ptr [bp].lSourceY,ax
        mov     ax,word ptr [bx].SourceY+2
        mov     word ptr [bp].lSourceY+2,ax

; Calculate source steps that correspond to each 1-pixel destination X step
; (across the destination scan line).

        push    si              ;push dest Y height, in fixedpoint form
        sub     ax,ax
        push    ax              ;push 0 as fractional part of dest Y height
        mov     ax,word ptr [di].SourceX
        sub     ax,word ptr [bp].lSourceX       ;low word of source X width
        mov     dx,word ptr [di].SourceX+2
        sbb     dx,word ptr [bp].lSourceX+2     ;high word of source X width
        push    dx              ;push source X width, in fixedpoint form
        push    ax
        call    _FixedDiv       ;scale source X width to dest Y height
        add     sp,8            ;clear parameters from stack
        mov     word ptr [bp].lSourceStepX,ax   ;remember source X step for
        mov     word ptr [bp].lSourceStepX+2,dx ; 1-pixel destination Y height
        mov     cx,1            ;assume source X advances non-negative
        and     dx,dx           ;which way does source X advance?
        jns     short SourceXNonNeg ;non-negative
        neg     cx              ;negative
        cmp     ax,0            ;is the whole step exactly an integer?
        jz      short SourceXNonNeg ;yes
        inc     dx              ;no, truncate to integer in the direction of
                                ; 0, because otherwise we'll end up with a
                                ; whole step of 1-too-large magnitude
SourceXNonNeg:
        mov     [bp].lXAdvanceByOne,cx  ;amount to add to source pointer to
                                        ; move by one in X
        mov     [bp].lBaseAdvance,dx    ;minimum amount to add to source
                                        ; pointer to advance in X each time
                                        ; the dest advances one in Y

        push    si              ;push dest Y height, in fixedpoint form
        sub     ax,ax
        push    ax              ;push 0 as fractional part of dest Y height
        mov     ax,word ptr [di].SourceY
        sub     ax,word ptr [bp].lSourceY       ;low word of source Y height
        mov     dx,word ptr [di].SourceY+2
        sbb     dx,word ptr [bp].lSourceY+2     ;high word of source Y height
        push    dx              ;push source Y height, in fixedpoint form
        push    ax
        call    _FixedDiv       ;scale source Y height to dest Y height
        add     sp,8            ;clear parameters from stack
        mov     word ptr [bp].lSourceStepY,ax   ;remember source Y step for
        mov     word ptr [bp].lSourceStepY+2,dx ; 1-pixel destination X step
        mov     cx,[_TexMapWidth] ;assume source Y advances non-negative
        and     dx,dx           ;which way does source Y advance?
        jns     short SourceYNonNeg ;non-negative
        neg     cx              ;negative
        cmp     ax,0            ;is the whole step exactly an integer?
        jz      short SourceYNonNeg ;yes
        inc     dx              ;no, truncate to integer in the direction of
                                ; 0, because otherwise we'll end up with a
                                ; whole step of 1-too-large magnitude
SourceYNonNeg:
        mov     [bp].lYAdvanceByOne,cx  ;amount to add to source pointer to
                                        ; move by one in Y
        mov     ax,[_TexMapWidth]       ;minimum distance skipped in source
        imul    dx                      ; image bitmap when Y steps (ignoring
        add     [bp].lBaseAdvance,ax    ; carry from the fractional part);
                                        ; add into total minimum source
                                        ; advance amount

; Advance 1/2 step in the stepping direction, to space scanned pixels evenly
; between the left and right edges. (There's a slight inaccuracy in dividing
; negative numbers by 2 by shifting rather than dividing, but the inaccuracy
; is in the least significant bit, and we'll just live with it.)

        mov     ax,word ptr [bp].lSourceStepX
        mov     dx,word ptr [bp].lSourceStepX+2
        sar     dx,1
        rcr     ax,1
        add     word ptr [bp].lSourceX,ax
        adc     word ptr [bp].lSourceX+2,dx

        mov     ax,word ptr [bp].lSourceStepY
        mov     dx,word ptr [bp].lSourceStepY+2
        sar     dx,1
        rcr     ax,1
        add     word ptr [bp].lSourceY,ax
        adc     word ptr [bp].lSourceY+2,dx

; Clip bottom edge if necessary.

        mov     bx,[di].DestY
        cmp     bx,[_ClipMaxY]
        jl      short BottomEdgeClipped
        mov     bx,[_ClipMaxY]
BottomEdgeClipped:

; Clip top edge if necssary

        mov     si,[bp].TopEdge
        mov     di,[si].DestY
        cmp     di,[_ClipMinY]
        jge     short TopEdgeClipped

; Top clipping is necessary; advance the source accordingly

        neg     di
        add     di,[_ClipMinY]  ;ClipMinY - DestY
                                ;first, advance the source in X
        push    di              ;push ClipMinX - DestX, in fixedpoint form
        sub     ax,ax
        push    ax              ;push 0 as fractional part of ClipMinX-DestX
        push    word ptr [bp].lSourceStepX+2
        push    word ptr [bp].lSourceStepX
        call    _FixedMul       ;total source X stepping in clipped area
        add     sp,8            ;clear parameters from stack
        add     word ptr [bp].lSourceX,ax   ;step the source X past clipping
        adc     word ptr [bp].lSourceX+2,dx
                                ;now advance the source in Y
        push    di              ;push ClipMinX - DestX, in fixedpoint form
        sub     ax,ax
        push    ax              ;push 0 as fractional part of ClipMinX-DestX
        push    word ptr [bp].lSourceStepY+2
        push    word ptr [bp].lSourceStepY
        call    _FixedMul       ;total source Y stepping in clipped area
        add     sp,8            ;clear parameters from stack
        add     word ptr [bp].lSourceY,ax   ;step the source Y past clipping
        adc     word ptr [bp].lSourceY+2,dx

        mov     di,[_ClipMinY]  ;start Y coordinate in dest after clipping
TopEdgeClipped:

; Calculate actual clipped destination drawing height.

        sub     bx,di

; Scan down the destination column, updating the source image position
; accordingly.

; Point to the initial source image pixel, adding 0.5 to both X and Y so that
; we can truncate to integers from now on but effectively get rounding.

        add     word ptr [bp].lSourceY,8000h    ;add 0.5
        mov     ax,word ptr [bp].lSourceY+2
        adc     ax,0
        mul     [_TexMapWidth]   ;initial scan line in source image
        add     word ptr [bp].lSourceX,8000h    ;add 0.5
        mov     si,word ptr [bp].lSourceX+2 ;offset into source scan line
        adc     si,ax            ;initial source offset in source image
        add     si,[_TexMapBits] ;SI points to the initial image pixel

; Point to initial destination pixel.

        mov     ax,SCREEN_WIDTH
        mul     di      ;offset of initial dest scan line
        mov     di,[_DestX]
        mov     cx,di   ;initial destination X
        shr     di,1
        shr     di,1    ;X/4 = offset of pixel in scan line
        add     di,ax   ;offset of pixel in page
        add     di,[_CurrentPageBase] ;offset of pixel in display memory
                        ;DI now points to the first destination pixel

        and     cl,011b ;CL = pixel's plane
        mov     al,MAP_MASK
        mov     dx,SC_INDEX
        mov     ah,01h  ;one plane bit in each nibble, so we'll get carry
                        ; automatically when going from plane 3 to plane 0
        shl     ah,cl   ;set the bit for the first pixel's plane to 1
        out     dx,ax

; If source Y step is negative, change over to working with non-negative
; values.

        cmp     word ptr [bp].lYAdvanceByOne,0
        jge     short SYStepSet
        neg     word ptr [bp].lSourceStepY
        not     word ptr [bp].lSourceY
SYStepSet:

; If source X step is negative, handle with a separate inner loop.

        cmp     word ptr [bp].lXAdvanceByOne,0
        jl      short TexScanRToL

; Handles edges that go left to right through the source texture.

        mov     cx,word ptr [bp].lSourceStepX
        shl     ecx,16          ;upper word of ECX is fractional X advance
        mov     cx,word ptr [bp].lSourceStepY
        shr     cx,1            ;lower 15 bits of ECX are fractional Y
                                ; advance, bit 15 is 0
        mov     dx,[bp].lSourceX
        shl     edx,16
        mov     dx,[bp].lSourceY
        shr     dx,1            ;upper 16 bits are X fractional coord, bit 15
                                ; is 0, lower 15 bits are Y fractional coord
        push    ds              ;preserve normal data segment
        mov     ax,SCREEN_SEG
        mov     ds,ax           ;DS:DI->initial destination pixel
        mov     ax,[bp].lYAdvanceByOne
        push    bp              ;preserve stack frame pointer
                                ;***stack frame not available***
        mov     bp,[bp].lBaseAdvance
        xchg    bp,si           ;SS:BP->initial source texture pixel
                                ;SI = minimum source advance per destination
                                ; step
        inc     bx              ;round pixel-pair count up
        shr     bx,1            ;# of pixel-pairs to texture map
        jc      short TexScanLToRLoop ;even # of pixels
        push    bx              ;preserve pixel-pair count
        sub     di,SCREEN_WIDTH ;preadjust back to compensate for
                                ; built-in offset in odd-pixel code
                                ; being jumped to
        jmp     short TexScanLToRLoopOddEntry ;odd # of pixels

; Inner loop to draw a single texture-mapped vertical column,
; rather than a horizontal scanline. Maxed-out 16-bit version.
;
; At this point:
;       AX = source pointer increment to advance one in Y
;       BX = # of pixel-pairs to draw
;       ECX = fractional Y advance in lower 15 bits of CX,
;             fractional X advance in high word of ECX, bit
;             15 set to 0
;       EDX = fractional source texture Y coordinate in lower
;             15 bits of CX, fractional source texture X coord
;             in high word of ECX, bit 15 set to 0
;       SI = sum of integral X & Y source pointer advances
;       DS:DI = initial destination pointer
;       SS:BP = initial source texture pointer
;       Plane mask set to point to the plane containing this scan

        align   2
TexScanLToRLoop:
        push    bx      ;remember pixel-pair count

        mov     bl,ss:[bp]      ;get texture pixel
        mov     ds:[di],bl      ;set screen pixel

        add     edx,ecx ;advance frac Y in DX,
                        ; frac X in high word of EDX
        adc     bp,si   ;advance source pointer by integral
                        ; X & Y amount, also accounting for
                        ; carry from X fractional addition
        test    dh,80h  ;carry from Y fractional addition?
        jz      short TSLToRL1 ;no
        add     bp,ax   ;yes, advance Y by one
        and     dh,not 80h ;reset the Y fractional carry bit
TSLToRL1:

TexScanLToRLoopOddEntry:

        mov     bl,ss:[bp]              ;get texture pixel
        mov     ds:[di+SCREEN_WIDTH],bl ;set screen pixel

        add     edx,ecx ;advance frac Y in DX,
                        ; frac X in high word of EDX
        adc     bp,si   ;advance source pointer by integral
                        ; X & Y amount, also accounting for
                        ; carry from X fractional addition
        test    dh,80h  ;carry from Y fractional addition?
        jz      short TSLToRL2 ;no
        add     bp,ax   ;yes, advance Y by one
        and     dh,not 80h ;reset the Y fractional carry bit
TSLToRL2:

; Point to the next destination pixel.

        add     di,SCREEN_WIDTH*2

; Continue if there are any more dest pixels to draw.

        pop     bx
        dec     bx
        jnz     short TexScanLToRLoop

        pop     bp              ;restore stack frame pointer
                                ;***stack frame available***
        pop     ds              ;restore normal data segment
        jmp     short ScanDone

; Handles edges that go right to left through the source texture.
        align   2
TexScanRToL:
        mov     cx,word ptr [bp].lSourceStepX
        neg     cx              ;make fractional X advance positive, so we can
                                ; add both X and Y simultaneously
        shl     ecx,16          ;upper word of ECX is fractional X advance
        mov     cx,word ptr [bp].lSourceStepY
        shr     cx,1            ;lower 15 bits of ECX are fractional Y
                                ; advance, bit 15 is 0
        mov     dx,[bp].lSourceX
        not     dx              ;convert fraction X advance to counting-up
                                ; mode, so we can add both X and Y
                                ; simultaneously (since X goes right to left,
                                ; fractional X would normally be advanced
                                ; with subtraction)
        shl     edx,16
        mov     dx,[bp].lSourceY
        shr     dx,1            ;upper 16 bits are X fractional coord, bit 15
                                ; is 0, lower 15 bits are Y fractional coord
        push    ds              ;preserve normal data segment
        mov     ax,SCREEN_SEG
        mov     ds,ax           ;DS:DI->initial destination pixel
        mov     ax,[bp].lYAdvanceByOne
        push    bp              ;preserve stack frame pointer
                                ;***stack frame not available***
        mov     bp,[bp].lBaseAdvance
        xchg    bp,si           ;SS:BP->initial source texture pixel
        neg     si              ;SI = minimum source advance per destination
                                ; step, negated so we can use SBB in the inner
                                ; loop
        inc     bx              ;round pixel-pair count up
        shr     bx,1            ;# of pixel-pairs to texture map
        jc      short TexScanRToLLoop ;even # of pixels
        push    bx              ;preserve pixel-pair count
        sub     di,SCREEN_WIDTH ;preadjust back to compensate for
                                ; built-in offset in odd-pixel code
                                ; being jumped to
        jmp     short TexScanRToLLoopOddEntry ;odd # of pixels

; Inner loop to draw a single texture-mapped vertical column,
; rather than a horizontal scanline. Maxed-out 16-bit version.
;
; At this point:
;       AX = source pointer increment to advance one in Y
;       BX = # of pixel-pairs to draw
;       ECX = fractional Y advance in lower 15 bits of CX,
;             fractional X advance in high word of ECX, bit
;             15 set to 0
;       EDX = fractional source texture Y coordinate in lower
;             15 bits of CX, fractional source texture X coord
;             in high word of ECX, bit 15 set to 0
;       SI = sum of integral X & Y source pointer advances
;       DS:DI = initial destination pointer
;       SS:BP = initial source texture pointer
;       Plane mask set to point to the plane containing this scan

        align   2
TexScanRToLLoop:
        push    bx      ;remember pixel-pair count

        mov     bl,ss:[bp]      ;get texture pixel
        mov     ds:[di],bl      ;set screen pixel

        add     edx,ecx ;advance frac Y in DX,
                        ; frac X in high word of EDX
        sbb     bp,si   ;advance source pointer by integral
                        ; X & Y amount, also accounting for
                        ; carry from X fractional addition
        test    dh,80h  ;carry from Y fractional addition?
        jz      short TSRToLL1 ;no
        add     bp,ax   ;yes, advance Y by one
        and     dh,not 80h ;reset the Y fractional carry bit
TSRToLL1:

TexScanRToLLoopOddEntry:

        mov     bl,ss:[bp]              ;get texture pixel
        mov     ds:[di+SCREEN_WIDTH],bl ;set screen pixel

        add     edx,ecx ;advance frac Y in DX,
                        ; frac X in high word of EDX
        sbb     bp,si   ;advance source pointer by integral
                        ; X & Y amount, also accounting for
                        ; carry from X fractional addition
        test    dh,80h  ;carry from Y fractional addition?
        jz      short TSRToLL2 ;no
        add     bp,ax   ;yes, advance Y by one
        and     dh,not 80h ;reset the Y fractional carry bit
TSRToLL2:

; Point to the next destination pixel.

        add     di,SCREEN_WIDTH*2

; Continue if there are any more dest pixels to draw.

        pop     bx
        dec     bx
        jnz     short TexScanRToLLoop

        pop     bp              ;restore stack frame pointer
                                ;***stack frame available***
        pop     ds              ;restore normal data segment

ScanDone:
        pop     di              ;restore caller's register variables
        pop     si
        mov     sp,bp           ;deallocate local variables
        pop     bp              ;restore caller's stack frame
        ret
_ScanOutLine    endp

        end

