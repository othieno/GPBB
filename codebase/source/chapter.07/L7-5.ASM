 

     CLC            ;clear the carry for the initial addition
LOOP_TOP:
     MOV  AX,[SI]   ;get next source operand word
     ADC  [DI],AX   ;add with carry to dest operand word
     INC  SI        ;point to next source operand word
     INC  SI
     INC  DI        ;point to next dest operand word
     INC  DI
     LOOP LOOP_TOP

