; *** Listing 18.4 ***
;
; Contains 3 C-callable routines: GenerateOctant, DrawVOctant, and
;       DrawHOctant. See individual routines for comments.
;
; Assembled with TASM 4.0.  Link with L18-2.C and L18-3.C.
; Checked by Jim Mischel  11/30/94.
;
ISVGA   equ     0       ;set to 1 to use VGA write mode 3
                        ; keep synchronized with Listing 3
        .model small
        .code
;********************************************************************
; Generates an octant of the specified circle, placing the results in
; PixList, with a 0 in PixList meaning draw pixel & move only along
; major axis, and a 1 in PixList meaning draw pixel & move along both
; axes.
; C near-callable as:
;  int GenerateOctant(unsigned char *PixList, int MajorAxis,
;       int MinorAxis, unsigned long RadiusSqMinusMajorAxisSq,
;       unsigned long MinorAxisSquaredThreshold);
;
; Return value = MajorAxis
;
GenerateOctantParms     struc
        dw      ?       ;pushed BP
        dw      ?       ;return address
PixList dw      ?       ;pointer to list to store draw control data in
MajorAxis dw    ?       ;initial major/minor axis coords relative to
MinorAxis dw    ?       ; to the center of the circle
RadiusSqMinusMajorAxisSq dd ?  ;initial Radius**2 - MajorAxis**2
MinorAxisSquaredThreshold dd ? ;initial threshhold for minor axis
GenerateOctantParms     ends   ; movement is MinorAxis**2 - MinorAxis
;
        public _GenerateOctant
_GenerateOctant proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    si              ;preserve C register variables
        push    di
;                                       ;get all parms into registers
        mov     di,[PixList+bp]         ;point DI to PixList
        mov     ax,[MajorAxis+bp]       ;AX=MajorAxis
        mov     bx,[MinorAxis+bp]       ;BX=MinorAxis
        mov     cx,word ptr [RadiusSqMinusMajorAxisSq+bp]
        mov     dx,word ptr [RadiusSqMinusMajorAxisSq+bp+2]
                                        ;DX:CX=RadiusSqMinusMajorAxisSq
        mov     si,word ptr [MinorAxisSquaredThreshold+bp]
        mov     bp,word ptr [MinorAxisSquaredThreshold+bp+2]
                                        ;BP:SI=MinorAxisSquaredThreshold
GenLoop:
        sub     cx,1            ;subtract MajorAxis + MajorAxis + 1 from
        sbb     dx,0            ; RadiusSqMinusMajorAxisSq
        sub     cx,ax
        sbb     dx,0
        sub     cx,ax
        sbb     dx,0
        cmp     dx,bp           ;if RadiusSqMinusMajorAxisSq <=
        jb      IsMinorMove     ; MinorAxisSquaredThreshold, move along
        ja      NoMinorMove     ; minor as well as major, otherwise move
        cmp     cx,si           ; only along major
        ja      NoMinorMove
IsMinorMove:                    ;move along minor as well as major
        dec     bx              ;decrement MinorAxis
        sub     si,bx           ;subtract MinorAxis + MinorAxis from
        sbb     bp,0            ; MinorAxisSquaredThreshold
        sub     si,bx
        sbb     bp,0
        mov     byte ptr [di],1 ;enter 1 (move both axes) in PixList
        inc     di              ;advance PixList pointer
        inc     ax              ;increment MajorAxis
        cmp     ax,bx           ;done if MajorAxis > MinorAxis, else
        jbe     GenLoop         ; continue generating PixList entries
        jmp     short Done
NoMinorMove:
        mov     byte ptr [di],0 ;enter 0 (move only major) in PixList
        inc     di              ;advance PixList pointer
        inc     ax              ;increment MajorAxis
        cmp     ax,bx           ;done if MajorAxis > MinorAxis, else
        jbe     GenLoop         ; continue generating PixList entries
Done:
        pop     di              ;restore C register variables
        pop     si
        pop     bp
        ret
_GenerateOctant endp
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

