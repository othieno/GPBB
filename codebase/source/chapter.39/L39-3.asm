; Draws all pixels in the list of horizontal lines passed in, in
; mode 13h, the VGA's 320x200 256-color mode. Uses REP STOS to fill
; each line.
; C near-callable as:
;     void DrawHorizontalLineList(struct HLineList * HLineListPtr,
;          int Color);
;
; Link with L21-1.C, L21-3.C, and L22-2.C in Small model
; Tested with TASM 4.0 and Borland C++ 4.02 by Jim Mischel 12/16/94.
;
SCREEN_WIDTH    equ   320
SCREEN_SEGMENT  equ   0a000h

HLine   struc
XStart  dw      ?  ;X coordinate of leftmost pixel in line
XEnd    dw      ?  ;X coordinate of rightmost pixel in line
HLine  ends

HLineList struc
Lngth  dw       ?   ;# of horizontal lines
YStart dw       ?   ;Y coordinate of topmost line
HLinePtr dw     ?   ;pointer to list of horz lines
HLineList ends

Parms   struc
        dw      2 dup(?)        ;return address & pushed BP
HLineListPtr  dw   ?    ;pointer to HLineList structure
Color   dw      ?       ;color with which to fill
Parms   ends

        .model small
        .code
        public _DrawHorizontalLineList
        align   2
_DrawHorizontalLineList   proc
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    si              ;preserve caller's register variables
        push    di
        cld             ;make string instructions inc pointers

        mov     ax,SCREEN_SEGMENT
        mov     es,ax           ;point ES to display memory for REP STOS

        mov     si,[bp+HLineListPtr] ;point to the line list
        mov     ax,SCREEN_WIDTH ;point to the start of the first scan
        mul     [si+YStart]     ; line in which to draw
        mov     dx,ax           ;ES:DX points to first scan line to
                        ; draw
        mov     bx,[si+HLinePtr] ;point to the XStart/XEnd descriptor
                        ; for the first (top) horizontal line
        mov     si,[si+Lngth]   ;# of scan lines to draw
        and     si,si           ;are there any lines to draw?
        jz      FillDone        ;no, so we're done
        mov     al,byte ptr [bp+Color] ;color with which to fill
        mov     ah,al           ;duplicate color for STOSW
FillLoop:
        mov     di,[bx+XStart]  ;left edge of fill on this line
        mov     cx,[bx+XEnd]    ;right edge of fill
        sub     cx,di
        js      LineFillDone    ;skip if negative width
        inc     cx              ;width of fill on this line
        add     di,dx           ;offset of left edge of fill
        test    di,1            ;does fill start at an odd address?
        jz      MainFill        ;no
        stosb                   ;yes, draw the odd leading byte to
                                ; word-align the rest of the fill
        dec     cx              ;count off the odd leading byte
        jz      LineFillDone    ;done if that was the only byte
MainFill:
        shr     cx,1            ;# of words in fill
        rep     stosw           ;fill as many words as possible
        adc     cx,cx           ;1 if there's an odd trailing byte to
                                ; do, 0 otherwise
        rep     stosb           ;fill any odd trailing byte
LineFillDone:
        add     bx,size HLine   ;point to the next line descriptor
        add     dx,SCREEN_WIDTH ;point to the next scan line
        dec     si              ;count off lines to fill
        jnz     FillLoop
FillDone:
        pop     di              ;restore caller's register variables
        pop     si
        pop     bp              ;restore caller's stack frame
        ret
_DrawHorizontalLineList   endp
        end
