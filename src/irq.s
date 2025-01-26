.global set_irq

; set_irq(void *irq_handler_fn);

IRQ_VEC=$0314

default_irq: .word 0

c_vector:
    jmp $00FF ; Will be patched by the C handler's address

set_irq:
    ; Save the vector of the c handler function
    lda __rc2
    sta c_vector + 1 ; Overwrite the lo byte of the jmp
    lda __rc3
    sta c_vector + 2 ; Overwrite the hi byte of the jmp

    ; Save the system's IRQ vector
    lda IRQ_VEC
    sta default_irq
    lda IRQ_VEC+1
    sta default_irq+1
    
    ; Set the IRQ vector to point to our handler routine
    sei
    lda #<handle_irq
    sta IRQ_VEC
    lda #>handle_irq
    sta IRQ_VEC+1
    cli
    rts

handle_irq:
    lda #<c_vector
    lda #>c_vector
    jsr c_vector
    lsr ; Mark handled
    jmp (default_irq)


