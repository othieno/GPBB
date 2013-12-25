     mov bl,[di]         ;get the state value for the pair
     mov di,[bp+OFFS]    ;get the next pair of characters
     mov ax,[bx+8000h]   ;increment word and line count 
     add dx,ax 	         ; appropriately for the pair

