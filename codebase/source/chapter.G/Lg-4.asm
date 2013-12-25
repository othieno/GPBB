; Draws all pixels in the list of horizontal lines passed in, in
; Hicolor (32K color) mode on an ET4000-based SuperVGA. Uses REP STOSW
; to fill each line. C near-callable as: 
;      void DrawHCLineList(struct HLineList * HLineListPtr, int Color);
; Tested with TASM 4.0 by Jim Mischel 12/16/94.

SCREEN_SEGMENT  equ     0a000h
GC_SEGMENT_SELECT equ   03cdh

HLine   struc
XStart  dw      ?       ;X coordinate of leftmost pixel in line
XEnd    dw      ?       ;X coordinate of rightmost pixel in line
HLine   ends

HLineList struc
Lngth   dw      ?       ;# of horizontal lines
YStart  dw      ?       ;Y coordinate of topmost line
HLinePtr dw     ?       ;pointer to list of horz lines
HLineList ends

Parms   struc
                dw      2 dup(?) ;return address & pushed BP
HLineListPtr    dw      ?       ;pointer to HLineList structure
Color           dw      ?       ;color with which to fill
Parms   ends

; Advances both the read and write windows to the next 64K bank.
; Note: Theoretically, a delay between IN and OUT may be needed under
; some circumstances to avoid accessing the VGA chip too quickly, but
; in actual practice, I haven't found any delay to be required.
INCREMENT_BANK  macro
        push    ax              ;preserve fill color
        push    dx              ;preserve scan line start pointer
        mov     dx,GC_SEGMENT_SELECT
        in      al,dx           ;get the current segment select
        add     al,11h          ;increment both the read & write banks
        out     dx,al           ;set the new bank #
        pop     dx              ;restore scan line start pointer
        pop     ax              ;restore fill color
        endm

        .model small
        .data
        extrn   _BitmapWidthInBytes:word
        .code
        public _DrawHCLineList
        align   2
_DrawHCLineList proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;point to our stack frame
        push    si              ;preserve caller's register variables
        push    di
        cld                     ;make string instructions inc pointers
        mov     ax,SCREEN_SEGMENT
        mov     es,ax   ;point ES to display memory for REP STOS
        mov     si,[bp+HLineListPtr] ;point to the line list
        mov     ax,[_BitmapWidthInBytes] ;point to the start of the
        mul     [si+YStart]     ; first scan line on which to draw
        mov     di,ax           ;ES:DI points to first scan line to
        mov     al,dl           ; draw; AL is the initial bank #
                                ;upper nibble of AL is read bank #,
        mov     cl,4            ; lower nibble is write bank # (only
        shl     dl,cl           ; the write bank is really needed for
        or      al,dl           ; this module, but it's less confusing
                                ; to point both to the same place)
        mov     dx,GC_SEGMENT_SELECT
        out     dx,al           ;set the initial bank
        mov     dx,di           ;ES:DX points to first scan line
        mov     bx,[si+HLinePtr] ;point to the XStart/XEnd descriptor
                                ; for the first (top) horizontal line
        mov     si,[si+Lngth]   ;# of scan lines to draw
        and     si,si           ;are there any lines to draw?
        jz      FillDone        ;no, so we're done
        mov     ax,[bp+Color]   ;color with which to fill
        mov     bp,[_BitmapWidthInBytes] ;so we can keep everything
                                ; in registers inside the loop
                                ;***stack frame pointer destroyed!***
FillLoop:
        mov     di,[bx+XStart]  ;left edge of fill on this line
        mov     cx,[bx+XEnd]    ;right edge of fill
        sub     cx,di
        jl      LineFillDone    ;skip if negative width
        inc     cx              ;# of pixels to fill on this line
        add     di,di           ;*2 because pixels are 2 bytes in size
        add     dx,bp           ;do we cross a bank during this line?
        jnc     NormalFill      ;no
        jz      NormalFill      ;no
                                ;yes, there is a bank crossing on this
                                ; line; figure out where
        sub     dx,bp           ;point back to start of line
        add     di,dx           ;offset of left edge of fill
        jc      CrossBankBeforeFilling ;raster splits before the left
                                ; edge of fill
        add     cx,cx           ;fill width in bytes (pixels * 2)
        add     di,cx           ;do we split during the fill area?
        jnc     CrossBankAfterFilling ;raster splits after the right
        jz      CrossBankAfterFilling ; edge of fill
                                ;bank boundary falls within fill area;
                                ; draw in two parts, one in each bank
        sub     di,cx           ;point back to start of fill area
        neg     di              ;# of bytes left before split
        sub     cx,di           ;# of bytes to fill to the right of
                                ; the bank split
        push    cx              ;remember right-of-split fill width
        mov     cx,di           ;# of left-of-split bytes to fill
        shr     cx,1            ;# of left-of-split words to fill
        neg     di              ;offset at which to start filling
        rep     stosw           ;fill left-of-split portion of line
        pop     cx              ;get back right-of-split fill width
        shr     cx,1            ;# of right-of-split words to fill
                                ;advance to the next bank       
        INCREMENT_BANK          ;point to the next bank (DI already
                                ; points to offset 0, as desired)
        rep     stosw           ;fill right-of-split portion of line
        add     dx,bp           ;point to the next scan line
        jmp     short CountDownLine ; (already advanced the bank)
;======================================================================
        align   2               ;dfill area is entirely to the left of
CrossBankAfterFilling:          ; the bank boundary
        sub     di,cx           ;point back to start of fill area
        shr     cx,1            ;CX = fill width in pixels
        jmp     short FillAndAdvance ;doesn't split until after the
                                ; fill area, so handle normally
;======================================================================
        align   2               ;fill area is entirely to the right of
CrossBankBeforeFilling:         ; the bank boundary
        INCREMENT_BANK          ;first, point to the next bank, where
                                ; the fill area resides
        rep     stosw           ;fill this scan line
        add     dx,bp           ;point to the next scan line
        jmp     short CountDownLine ; (already advanced the bank)
;======================================================================
        align   2               ;no bank boundary problems; just fill
NormalFill:                     ; normally
        sub     dx,bp           ;point back to start of line
        add     di,dx           ;offset of left edge of fill
FillAndAdvance:
        rep     stosw           ;fill this scan line
LineFillDone:
        add     dx,bp           ;point to the next scan line
        jnc     CountDownLine   ;didn't cross a bank boundary
        INCREMENT_BANK          ;did cross, so point to the next bank 
CountDownLine:
        add     bx,size HLine   ;point to the next line descriptor
        dec     si              ;count off lines to fill
        jnz     FillLoop
FillDone:
        pop     di              ;restore caller's register variables
        pop     si
        pop     bp              ;restore caller's stack frame
        ret
;======================================================================
_DrawHCLineList endp
        end

