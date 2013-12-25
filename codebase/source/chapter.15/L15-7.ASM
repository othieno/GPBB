; C near-callable assembly function for inserting a new node in a
; linked list sorted by ascending order of the Value field. The list
; is circular; that is, it has a dummy node as both the head and the
; tail of the list. The dummy node is a sentinel, containing the
; largest possible Value field setting. Tested with TASM.

MAX_TEXT_LENGTH equ 100         ;longest allowed Text field
SENTINEL equ  32767             ;largest possible Value field

LinkNode struc
NextNode dw     ?
Value   dw      ?
Text    db      MAX_TEXT_LENGTH+1 dup(?)
;*** Any number of additional data fields may by present ***
LinkNode ends

        .model  small
        .code

; Inserts the specified node into a ascending-value-sorted linked
; list, such that value-sorting is maintained. Returns a pointer to
; the node after which the new node is inserted.
; C near-callable as:
; struct LinkNode *InsertNodeSorted(struct LinkNode *HeadOfListNode,
;      struct LinkNode *NodeToInsert)

parms   struc
        dw      2 dup (?)       ;pushed return address & BP
HeadOfListNode dw       ?       ;pointer to head node of list
NodeToInsert dw         ?       ;pointer to node to insert
parms   ends

        public  _InsertNodeSorted
_InsertNodeSorted proc  near
        push    bp
        mov     bp,sp                   ;point to stack frame
        push    si                      ;preserve register vars
        push    di
        mov     si,[bp].NodeToInsert    ;point to node to insert
        mov     ax,[si].Value           ;search value
        mov     di,[bp].HeadOfListNode  ;point to linked list in
                                        ; which to insert
SearchLoop:
        mov     bx,di                   ;advance to the next node
        mov     di,[bx].NextNode        ;point to following node
        cmp     [di].Value,ax           ;is the following node's
                                        ; value less than the value
                                        ; from the node to insert?
        jl      SearchLoop              ;yes, so continue searching
                                        ;no, so we have found our
                                        ; insert point
        mov     ax,[bx].NextNode        ;link the new node between
        mov     [si].NextNode,ax        ; the current node and the
        mov     [bx].NextNode,si        ; following node
        mov     ax,bx                   ;return pointer to node
                                        ; after which we inserted
        pop     di                      ;restore register vars
        pop     si
        pop     bp
        ret
_InsertNodeSorted endp
        end

