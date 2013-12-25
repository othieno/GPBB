; Searches a buffer for a specified pattern. In case of a mismatch,
; uses the value of the mismatched byte to skip across as many
; potential match locations as possible (partial Boyer-Moore).
; Returns start offset of first match searching forward, or NULL if
; no match is found.
; Requires that the pattern be no longer than 255 bytes, and that
; there be a match for the pattern somewhere in the buffer (ie., a
; copy of the pattern should be placed as a sentinel at the end of
; the buffer if the pattern isn't already known to be in the buffer).
; Tested with TASM.
; C near-callable as:
;       unsigned char * FindString(unsigned char * BufferPtr,
;          unsigned int BufferLength, unsigned char * PatternPtr,
;          unsigned int PatternLength);

parms   struc
        dw      2 dup(?) ;pushed BP & return address
BufferPtr dw    ?       ;pointer to buffer to be searched
BufferLength dw ?       ;# of bytes in buffer to be searched
                        ; (not used, actually)
PatternPtr dw   ?       ;pointer to pattern for which to search
                        ; (pattern *MUST* exist in the buffer)
PatternLength dw ?      ;length of pattern for which to search (must
                        ; be <= 255)
parms   ends

        .model small
        .code
        public _FindString
_FindString     proc    near
        cld
        push    bp      ;preserve caller's stack frame
        mov     bp,sp   ;point to our stack frame
        push    si      ;preserve caller's register variables
        push    di
        sub     sp,256  ;allocate space for SkipTable
; Create the table of distances by which to skip ahead on mismatches
; for every possible byte value. First, initialize all skips to the
; pattern length; this is the skip distance for bytes that don't
; appear in the pattern.
        mov     di,ds
        mov     es,di   ;ES=DS=SS
        mov     di,sp   ;point to SkipBuffer
        mov     al,byte ptr [bp+PatternLength]
        and     al,al   ;return an instant match if the pattern is
        jz      InstantMatch ; 0-length
        mov     ah,al
        mov     cx,256/2
        rep     stosw
        mov     ax,[bp+PatternLength]
        dec     ax                       ;from now on, we only need
        mov     [bp+PatternLength],ax    ; PatternLength - 1
; Point to rightmost byte of first potential pattern match location
; in buffer.
        add     [bp+BufferPtr],ax
; Set the skip values for the bytes that do appear in the pattern to
; the distance from the byte location to the end of the pattern.
        mov     si,[bp+PatternPtr] ;point to start of pattern
        and     ax,ax   ;are there any skips to set?
        jz      SetSkipDone ;no
        mov     di,sp   ;point to SkipBuffer
        sub     bx,bx   ;prepare for word addressing off byte value
SetSkipLoop:
        mov     bl,[si] ;get the next pattern byte
        inc     si      ;advance the pattern pointer
        mov     [di+bx],al ;set the skip value when this byte value is
                        ; the mismatch value in the buffer
        dec     ax
        jnz     SetSkipLoop
SetSkipDone:
        mov     dl,[si] ;DL=rightmost pattern byte from now on
        dec     si      ;point to next-to-rightmost byte of pattern
        mov     [bp+PatternPtr],si ; from now on
; Search the buffer.
        std                     ;for backward REPZ CMPSB
        mov     di,[bp+BufferPtr] ;point to the first search location
        mov     bx,sp           ;point to SkipTable for XLAT
SearchLoop:
        sub     ah,ah   ;used to convert AL to a word
; Skip through until there's a match for the first pattern byte.
QuickSearchLoop:
; See if we have a match at the first buffer location.
        REPT    8       ;unroll loop 8 times to reduce branching
        mov     al,[di] ;next buffer byte
        cmp     dl,al   ;does it match the pattern?
        jz      FullCompare ;yes, so keep going
        xlat            ;no, look up the skip value for this mismatch
        add     di,ax   ;BufferPtr += Skip;
        ENDM
        jmp     QuickSearchLoop
; Return a pointer to the start of the buffer (for 0-length pattern).
        align   2
InstantMatch:
        mov     ax,[bp+BufferPtr]
        jmp     short Done
; Compare the pattern and the buffer location, searching from high
; memory toward low (right to left).
        align   2
FullCompare:
        mov     [bp+BufferPtr],di ;save the current buffer location
        mov     cx,[bp+PatternLength] ;# of bytes yet to compare
        jcxz    Match   ;done if there was only one character
        dec     di      ;point to next dest byte to compare (SI
                        ; points to next-to-rightmost src byte)
        repz    cmpsb   ;compare the rest of the pattern
        jz      Match   ;that's it; we've found a match
; It's a mismatch; let's see what we can learn from it.
        inc     di      ;compensate for 1-byte overrun of REPZ CMPSB;
                        ; point to mismatch location in buffer
; # of bytes that did match.
        mov     si,[bp+BufferPtr]
        sub     si,di
; If, based on the mismatch character, we can't even skip ahead as far
; as where we started this particular comparison, then just advance by
; 1 to the next potential match; otherwise, skip ahead from this
; comparison location by the skip distance for the mismatch character,
; less the distance covered by the partial match.
        mov     al,[di] ;get the value of the mismatch byte in buffer
        xlat            ;get the skip value for this mismatch
        mov     cx,1    ;assume we'll just advance to the next
                        ; potential match location
        sub     ax,si   ;is the skip far enough to be worth taking?
        jna     MoveAhead ;no, go with the default advance of 1
        mov     cx,ax   ;yes, this is the distance to skip ahead from
                        ; the last potential match location checked
MoveAhead:
; Skip ahead and perform the next comparison.
        mov     di,[bp+BufferPtr]
        add     di,cx              ;BufferPtr += Skip;
        mov     si,[bp+PatternPtr] ;point to the next-to-rightmost
                                   ; pattern byte
        jmp     SearchLoop
; Return start of match in buffer (BufferPtr - (PatternLength - 1)).
        align   2
Match:
        mov     ax,[bp+BufferPtr]
        sub     ax,[bp+PatternLength]
Done:
        cld             ;restore default direction flag
        add     sp,256  ;deallocate space for SkipTable
        pop     di      ;restore caller's register variables
        pop     si
        pop     bp      ;restore caller's stack frame
        ret
_FindString     endp
        end

