ScanLoop:
        lodsw           ;get the next 2 bytes (AL = first, AH = 2nd)
        xlat            ;look up first's char/not status
        xor     dl,al   ;see if there's a new char/not status
        add     di,dx   ;we add 1 for each char/not transition
        mov     dl,al
        mov     al,ah   ;look at the second byte
        xlat            ;look up its char/not status
        xor     dl,al   ;see if there's a new char/not status
        add     di,dx   ;we add 1 for each char/not transition
        mov     dl,al
        dec     dx
        jnz     ScanLoop

