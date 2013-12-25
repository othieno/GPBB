; *** Listing 20.4 ***
;
; Contains 3 C-callable routines: GenerateEOctant, DrawVOctant, and
;       DrawHOctant. See individual routines for comments.
;
; Assembled with TASM 4.0.  Link with L20-2.C and L20-3.C.
; Checked by Jim Mischel  11/30/94.
;
ISVGA   equ     0       ;set to 1 to use VGA write mode 3
                        ; keep synchronized with Listing 3
        .model small
        .code
;********************************************************************
; Generates an octant of the specified ellipse, placing the results in
; PixList, with a 0 in PixList meaning draw pixel & move only along
; major axis, and a 1 in PixList meaning draw pixel & move along both
; axes.
; C near-callable as:
;  int GenerateEOctant(unsigned char *PixList, long MinorAdjust,
;       long Threshold, long MajorSquared, long MinorSquared);
;
; Return value = PixelCount (# of points)
;
; Passed parameters:
;
GenerateOctantParms     struc
        dw      ?       ;pushed BP
        dw      ?       ;return address pushed by call
PixList dw      ?       ;pointer to list to store draw control data in
MinorAdjust dd  ?       ;initially MajorAxis**2 * 2 * MinorAxis, used
                        ; to adjust threshold after minor axis move
Threshold dd    ?       ;initially MajorAxis**2 / 4 + MajorAxis**2 *
                        ; MinorAxis, used to determine when to advance
                        ; along the minor axis
MajorSquared dd ?       ;MajorAxis**2
MinorSquared dd ?       ;MinorAxis**2
GenerateOctantParms     ends
;
; Local variables (offsets relative to BP in stack frame):
;
PixelCount      equ     -2      ;running major axis coordinate
                                ; relative to center
MajorAdjust     equ     -6      ;used to adjust threshold after major
                                ; axis move
MajorSquaredTimes2 equ  -10     ;MajorSquared * 2
MinorSquaredTimes2 equ  -14     ;MinorSquared * 2
;
        public _GenerateEOctant
_GenerateEOctant proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        add     sp,MinorSquaredTimes2
                                ;allocate room for local vars
        push    si              ;preserve C register variables
        push    di
;Initialize local variables.
        mov     word ptr [bp+PixelCount],0 ;initialize count of pixels
                                        ; to zero
        mov     ax,word ptr [bp+MajorSquared] ;set MajorSquaredTimes2
        shl     ax,1                    ;lower word times 2
        mov     word ptr [bp+MajorSquaredTimes2],ax
        mov     ax,word ptr [bp+MajorSquared+2]
        rcl     ax,1                    ;upper word times 2
        mov     word ptr [bp+MajorSquaredTimes2+2],ax
        
        mov     ax,word ptr [bp+MinorSquared] ;set MinorSquaredTimes2
        shl     ax,1                    ;lower word times 2
        mov     word ptr [bp+MinorSquaredTimes2],ax
        mov     ax,word ptr [bp+MinorSquared+2]
        rcl     ax,1                    ;upper word times 2
        mov     word ptr [bp+MinorSquaredTimes2+2],ax
;Set up registers for loop.     
        mov     di,[PixList+bp]         ;point DI to PixList
; Set MajorAdjust to 0.
        sub     cx,cx
        mov     si,cx                   ;SI:CX = MajorAdjust

        mov     bx,word ptr [bp+Threshold]      ;DX:BX = threshold
        mov     dx,word ptr [bp+Threshold+2]
; At this point:
;  DX:BX = threshold
;  SI:CX = MajorAdjust
;  DI = PixList pointer
GenLoop:
; Advance the threshold by MajorAdjust + MinorAxis**2.
        add     bx,cx
        adc     dx,si
        add     bx,word ptr [bp+MinorSquared]
        adc     dx,word ptr [bp+MinorSquared+2]
; If the threshold has passed 0, then the minor coordinate has
; advanced more than halfway to the next pixel and it's time to
; advance the minor coordinate by 1 and set the next threshold
; accordingly.
        mov     byte ptr [di],0 ;assume we won't move along the
                                ; minor axis
        js      MoveMajor       ;and, in fact, we won't move minor
; Minor coordinate has advanced.
; Adjust the minor axis adjust value.
        mov     ax,word ptr [bp+MajorSquaredTimes2]
        sub     word ptr [bp+MinorAdjust],ax
        mov     ax,word ptr [bp+MajorSquaredTimes2+2]
        sbb     word ptr [bp+MinorAdjust+2],ax
; Adjust the threshold for the minor axis move
        sub     bx,word ptr [bp+MinorAdjust]
        sbb     dx,word ptr [bp+MinorAdjust+2]
        mov     byte ptr [di],1
MoveMajor:
        inc     di
; Count this point.
        inc     word ptr [bp+PixelCount]
; Adjust the major adjust for the new point.
        add     cx,word ptr [bp+MinorSquaredTimes2]
        adc     si,word ptr [bp+MinorSquaredTimes2+2]
; Stop if the major axis has switched (the arc has passed the
; 45-degree point).
        cmp     si,word ptr [bp+MinorAdjust+2]
        ja      Done
        jb      GenLoop
        cmp     cx,word ptr [bp+MinorAdjust]
        jb      GenLoop
Done:
        mov     ax,[bp+PixelCount]      ;return # of points
        pop     di              ;restore C register variables
        pop     si
        mov     sp,bp           ;deallocate local vars
        pop     bp              ;restore caller's stack frame
        ret
_GenerateEOctant endp
;********************************************************************
; Draws the arc for an octant in which Y is the major axis. (X,Y) is the
; starting point of the arc. HorizontalMoveDirection selects whether the
; arc advances to the left or right horizontally (0=left, 1=right).
; RowOffset contains the offset in bytes from one scan line to the next,
; controlling whether the arc is drawn up or down. DrawLength is the
; vertical length in pixels of the arc, and DrawList is a list
; containing 0 for each point if the next point is vertically aligned,
; and 1 if the next point is 1 pixel diagonally to the left or right.
;
; The Graphics Controller Index register must already point to the Bit
; Mask register.
;
; C near-callable as:
;  void DrawVOctant(int X, int Y, int DrawLength, int RowOffset,
;       int HorizontalMoveDirection, unsigned char *DrawList);
;
DrawParms       struc
        dw      ?       ;pushed BP
        dw      ?       ;return address
X       dw      ?       ;initial coordinates
Y       dw      ?
DrawLength dw   ?       ;vertical length
RowOffset dw    ?       ;distance from one scan line to the next
HorizontalMoveDirection dw ? ;1 to move right, 0 to move left
DrawList dw     ?       ;pointer to list containing 1 to draw
DrawParms       ends    ; diagonally, 0 to draw vertically for
                                ; each point
SCREEN_SEGMENT  equ     0a000h  ;display memory segment in mode 12h
SCREEN_WIDTH_IN_BYTES equ 80    ;distance from one scan line to next
GC_INDEX        equ     3ceh    ;GC Index register address
;
        public _DrawVOctant
_DrawVOctant    proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    si              ;preserve C register variables
        push    di
;Point ES:DI to the byte the initial pixel is in.
        mov     ax,SCREEN_SEGMENT
        mov     es,ax
        mov     ax,SCREEN_WIDTH_IN_BYTES
        mul     [bp+Y]  ;Y*SCREEN_WIDTH_IN_BYTES
        mov     di,[bp+X] ;X
        mov     cx,di   ;set X aside in CX
        shr     di,1
        shr     di,1
        shr     di,1    ;X/8
        add     di,ax   ;screen offset = Y*SCREEN_WIDTH_IN_BYTES+X/8
        and     cl,07h  ;X modulo 8
if ISVGA                ;---VGA---
        mov     ah,80h  ;keep VGA bit mask in AH
        shr     ah,cl   ;initial bit mask = 80h shr (X modulo 8);
        cld             ;for LODSB, used below
else                    ;---EGA---
        mov     al,80h  ;keep EGA bit mask in AL
        shr     al,cl   ;initial bit mask = 80h shr (X modulo 8);
        mov     dx,GC_INDEX+1 ;point DX to GC Data reg/bit mask
endif                   ;---------
        mov     si,[bp+DrawList] ;SI points to list to draw from
        sub     bx,bx           ;so we have the constant 0 in a reg
        mov     cx,[bp+DrawLength] ;CX=# of pixels to draw
        jcxz    VDrawDone       ;skip this if no pixels to draw
        cmp     [bp+HorizontalMoveDirection],0 ;draw right or left
        mov     bp,[bp+RowOffset] ;BP=offset to next row
        jz      VGoLeft         ;draw from right to left
VDrawRightLoop:                 ;draw from left to right
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3,
                                ; set/reset provides color
        lodsb                   ;get next draw control byte
        and     al,al           ;move right?
        jz      VAdvanceOneLineRight ;no move right
        ror     ah,1            ;move right
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter (set/reset provides
                                ; color); just force read then write
        cmp     [si],bl         ;check draw control byte; move right?
        jz      VAdvanceOneLineRight ;no move right
        ror     al,1            ;move right
endif                           ;---------
        adc     di,bx   ;move one byte to the right if mask wrapped
VAdvanceOneLineRight:
ife ISVGA                       ;---EGA---
        inc     si              ;advance draw control list pointer
endif                           ;---------
        add     di,bp           ;move to the next scan line up or down
        loop    VDrawRightLoop  ;do next pixel, if any
        jmp     short VDrawDone ;done
VGoLeft:                        ;draw from right to left
VDrawLeftLoop:
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
        lodsb                   ;get next draw control byte
        and     al,al           ;move left?
        jz      VAdvanceOneLineLeft ;no move left
        rol     ah,1            ;move left
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
        cmp     [si],bl         ;check draw control byte; move left?
        jz      VAdvanceOneLineLeft ;no move left
        rol     al,1            ;move left
endif                           ;---------
        sbb     di,bx           ;move one byte to the left if mask wrapped
VAdvanceOneLineLeft:
ife ISVGA                       ;---EGA---
        inc     si              ;advance draw control list pointer
endif                           ;---------
        add     di,bp           ;move to the next scan line up or down
        loop    VDrawLeftLoop   ;do next pixel, if any
VDrawDone:
        pop     di              ;restore C register variables
        pop     si
        pop     bp
        ret
_DrawVOctant    endp
;********************************************************************
; Draws the arc for an octant in which X is the major axis. (X,Y) is the
; starting point of the arc. HorizontalMoveDirection selects whether the
; arc advances to the left or right horizontally (0=left, 1=right).
; RowOffset contains the offset in bytes from one scan line to the next,
; controlling whether the arc is drawn up or down. DrawLength is the
; horizontal length in pixels of the arc, and DrawList is a list
; containing 0 for each point if the next point is horizontally aligned,
; and 1 if the next point is 1 pixel above or below diagonally.
;
; Graphics Controller Index register must already point to the Bit Mask
; register.
;
; C near-callable as:
;  void DrawHOctant(int X, int Y, int DrawLength, int RowOffset,
;       int HorizontalMoveDirection, unsigned char *DrawList)
;
; Uses same parameter structure as DrawVOctant().
;
        public _DrawHOctant
_DrawHOctant    proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    si              ;preserve C register variables
        push    di
;Point ES:DI to the byte the initial pixel is in.
        mov     ax,SCREEN_SEGMENT
        mov     es,ax
        mov     ax,SCREEN_WIDTH_IN_BYTES
        mul     [bp+Y]  ;Y*SCREEN_WIDTH_IN_BYTES
        mov     di,[bp+X] ;X
        mov     cx,di   ;set X aside in CX
        shr     di,1
        shr     di,1
        shr     di,1    ;X/8
        add     di,ax   ;screen offset = Y*SCREEN_WIDTH_IN_BYTES+X/8
        and     cl,07h  ;X modulo 8
        mov     bh,80h
        shr     bh,cl   ;initial bit mask = 80h shr (X modulo 8);
if ISVGA                ;---VGA---
        cld             ;for LODSB, used below
else                    ;---EGA---
        mov     dx,GC_INDEX+1 ;point DX to GC Data reg/bit mask
endif                    ;---------
        mov     si,[bp+DrawList] ;SI points to list to draw from
        sub     bl,bl           ;so we have the constant 0 in a reg
        mov     cx,[bp+DrawLength] ;CX=# of pixels to draw
        jcxz    HDrawDone       ;skip this if no pixels to draw
if ISVGA                        ;---VGA---
        sub     ah,ah           ;clear bit mask accumulator
else                            ;---EGA---
        sub     al,al           ;clear bit mask accumulator
endif                           ;---------
        cmp     [bp+HorizontalMoveDirection],0 ;draw right or left
        mov     bp,[bp+RowOffset] ;BP=offset to next row
        jz      HGoLeft         ;draw from right to left
HDrawRightLoop:                 ;draw from left to right
if ISVGA                        ;---VGA---
        or      ah,bh           ;put this pixel in bit mask accumulator
        lodsb                   ;get next draw control byte
        and     al,al           ;move up/down?
else                            ;---EGA---
        or      al,bh           ;put this pixel in bit mask accumulator
        cmp     [si],bl         ;check draw control byte; move up/down?
endif                           ;---------
        jz      HAdvanceOneLineRight ;no move up/down
                                ;move up/down; first draw accumulated pixels
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
        sub     ah,ah           ;clear bit mask accumulator
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
        sub     al,al           ;clear bit mask accumulator
endif                           ;---------
        add     di,bp           ;move to the next scan line up or down
HAdvanceOneLineRight:
ife ISVGA                       ;---EGA---
        inc     si              ;advance draw control list pointer
endif                           ;---------
        ror     bh,1            ;move to right; shift mask
        jnc     HDrawLoopRightBottom ;didn't wrap to the next byte
                                ;move to next byte; 1st draw accumulated pixels
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
        sub     ah,ah           ;clear bit mask accumulator
else
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
        sub     al,al           ;clear bit mask accumulator
endif                           ;---------
        inc     di              ;move 1 byte to the right
HDrawLoopRightBottom:
        loop    HDrawRightLoop  ;draw next pixel, if any
        jmp     short HDrawDone ;done
HGoLeft:                        ;draw from right to left
HDrawLeftLoop:
if ISVGA                        ;---VGA---
        or      ah,bh           ;put this pixel in bit mask accumulator
        lodsb                   ;get next draw control byte
        and     al,al           ;move up/down?
else                            ;---EGA---
        or      al,bh           ;put this pixel in bit mask accumulator
        cmp     [si],bl         ;check draw control byte; move up/down?
endif                           ;---------
        jz      HAdvanceOneLineLeft ;no move up/down
                                ;move up/down; first draw accumulated pixels
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
        sub     ah,ah           ;clear bit mask accumulator
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
        sub     al,al           ;clear bit mask accumulator
endif                           ;---------
        add     di,bp           ;move to the next scan line up or down
HAdvanceOneLineLeft:
ife ISVGA                       ;---EGA---
        inc     si              ;advance draw control list pointer
endif                           ;---------
        rol     bh,1            ;move to left; shift mask
        jnc     HDrawLoopLeftBottom ;didn't wrap to next byte
                                ;move to next byte; 1st draw accumulated pixels
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
        sub     ah,ah           ;clear bit mask accumulator
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
        sub     al,al           ;clear bit mask accumulator
endif                           ;---------
        dec     di              ;move 1 byte to the left
HDrawLoopLeftBottom:
        loop    HDrawLeftLoop   ;draw next pixel, if any
HDrawDone:
                                ;draw any remaining accumulated pixels
if ISVGA                        ;---VGA---
        and     es:[di],ah      ;AH becomes bit mask in write mode 3
else                            ;---EGA---
        out     dx,al           ;set the desired bit mask
        and     es:[di],al      ;data doesn't matter; force read/write
endif                           ;---------
        pop     di              ;restore C register variables
        pop     si
        pop     bp
        ret
_DrawHOctant    endp
        end

