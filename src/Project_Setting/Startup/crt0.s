/*
 * File:	crt0.s
 * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:	
 *
 */

;         AREA   Crt0, CODE, READONLY      ; name this block of code
  SECTION .noinit : CODE

        EXPORT  __startup
__startup
    MOV     r0,#0                   ; Initialize the GPRs
    MOV     r1,#0
    MOV     r2,#0
    MOV     r3,#0
    MOV     r4,#0
    MOV     r5,#0
    MOV     r6,#0
    MOV     r7,#0
    MOV     r8,#0
    MOV     r9,#0
    MOV     r10,#0
    MOV     r11,#0
    MOV     r12,#0
    CPSIE   i                       ; Unmask interrupts
    import start
    BL      start                  ; call the C code
__done
    B       __done

// hard fault handler wrapper in assembly
// it extract the location of stack frame and pass it
// to handler in C as pointer.
    EXPORT  __hard_fault_handler_asm
    IMPORT  hard_fault_handler_c
__hard_fault_handler_asm
    TST     LR, #4
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    B       hard_fault_handler_c    ; call the C code

    EXPORT  set_pit
set_pit
        push {r0}
        push {r2}
        push {r3}
        push {r4}
        ldr r3, =0x40037000    ; PIT MCR
        ldr r4, =0x4004803c    ; SIM_SCGC6
        ldr r2, =0x0
        ldr r0, [r4]           ; load SIM_SCGC6
        orr r0, r0, #0x800000  ; |= xxx, open clock gate
        str r0, [r4]           ; set SIM_SCGC6, PIT clock enable
        ldr r0, [r3]           ; Dummy read PIT MCR
        str r2, [r3]           ; ENABLE PIT
        pop {r4}
        pop {r3}
        pop {r2}
        pop {r0}
        bx  lr
        
        END
