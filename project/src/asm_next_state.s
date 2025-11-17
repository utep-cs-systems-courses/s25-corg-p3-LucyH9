    .text
    .global asm_next_state
asm_next_state:
    mov.b   &state, r14      ; load current state
    inc.b   r14              ; increment
    cmp     #NUM_STATES, r14 ; compare with number of states
    jl      done             ; if less, jump
    clr.b   r14              ; else wrap to 0
done:
    mov.b   r14, &state      ; store back
    ret
