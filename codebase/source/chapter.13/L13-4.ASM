     mov bl,[di]         ;get the state value for the pair
     mov di,[bp+OFFS]    ;get the next pair of characters
     add dx,ax           ;increment word and line count
                         ; appropriately for the pair
     mov ax,[bx+8000h]   ;get increments for next time

