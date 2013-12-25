     mov bl,[di]         ;get the state value for the pair
     mov di,[bp+OFFS]    ;get the next pair of characters
     add dx,[bx+8000h]   ;increment word and line count
                         ; appropriately for the pair

