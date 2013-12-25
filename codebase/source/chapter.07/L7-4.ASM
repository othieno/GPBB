 

     OR   EAX,ShiftTable[EBX*4]    ;look up the bit and OR it in
          :
ShiftTable     LABEL     DWORD
BIT_PATTERN=0001H
     REPT 32
     DD   BIT_PATTERN
BIT_PATTERN=BIT_PATTERN SHL 1
     ENDM

