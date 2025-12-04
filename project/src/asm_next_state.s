    .text
    .global asm_next_state
    .global state

NUM_STATES  .equ 5

asm_next_state:
    mov.b state, r14       ; load state into r14
    inc.b r14              ; increment
    cmp r14, #NUM_STATES   ; compare with NUM_STATES
    jb  done               ; jump if below NUM_STATES
    clr.b r14              ; else reset to 0
done:
    mov.b r14, state       ; store back to state
    ret

