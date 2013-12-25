 

     SHL  BX,1                ;prepare for word sized look up
     OR   AX,ShiftTable[BX]   ;look up the bit and OR it in
          :
ShiftTable     LABEL     WORD
BIT_PATTERN=0001H
     REPT 16
     DW   BIT_PATTERN
BIT_PATTERN=BIT_PATTERN SHL 1
     ENDM
