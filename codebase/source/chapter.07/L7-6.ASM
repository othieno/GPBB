 

     CLC            ;clear the carry for the initial addition
LOOP_TOP:
     MOV  AX,[SI]   ;get next source operand word
     ADC  [DI],AX   ;add with carry to dest operand word
     LAHF           ;set aside the Carry flag
     ADD  SI,2      ;point to next source operand word
     ADD  DI,2      ;point to next dest operand word
     SAHF           ;restore the Carry flag
     LOOP LOOP_TOP


