; Draws all pixels in the horizontal line segment passed in, from
;  (LeftX,Y) to (RightX,Y), in the specified color in mode 13h, the
;  VGA's 320x200 256-color mode. No drawing will take place if
;  LeftX > RightX.
; C near-callable as:
;       void DrawHorizontalLineSeg(Y, LeftX, RightX, Color);
;
; Link with L23-4.C and L23-1.C in small model.
; Tested with TASM 4.0 and Borland C++ 4.02 by Jim Mischel 12/16/94.

SCREEN_WIDTH    equ     320
SCREEN_SEGMENT  equ     0a000h

Parms   struc
        dw      2 dup(?) ;return address & pushed BP
Y       dw      ?       ;Y coordinate of line segment to draw
LeftX   dw      ?       ;left endpoint of the line segment
RightX  dw      ?       ;right endpoint of the line segment
Color   dw      ?       ;color in which to draw the line segment
Parms   ends

        .model small
        .code
        public _DrawHorizontalLineSeg
        align   2
_DrawHorizontalLineSeg  proc
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    di              ;preserve caller's register variable
        cld                     ;make string instructions inc pointers
        mov     ax,SCREEN_SEGMENT
        mov     es,ax           ;point ES to display memory
        mov     di,[bp+LeftX]
        mov     cx,[bp+RightX]
        sub     cx,di           ;width of line
        jl      DrawDone        ;RightX < LeftX; no drawing to do
        inc     cx              ;include both endpoints
        mov     ax,SCREEN_WIDTH
        mul     [bp+Y]          ;offset of scan line on which to draw
        add     di,ax           ;ES:DI points to start of line seg
        mov     al,byte ptr [bp+Color] ;color in which to draw
        mov     ah,al           ;put color in AH for STOSW
        shr     cx,1            ;# of words to fill
        rep     stosw           ;fill a word at a time
        adc     cx,cx
        rep     stosb           ;draw the odd byte, if any
DrawDone:
        pop     di              ;restore caller's register variable
        pop     bp              ;restore caller's stack frame
        ret
_DrawHorizontalLineSeg  endp
        end

