;Disable interrupts
;Load our GDT
;Set a bit on the CPU control register cr0
;Flush the CPU pipeline by issuing a carefully crafted far jump
;Update all the segment registers
;Update the stack
;Call to a well-known label which contains the first useful code in 32 bits
[bits 16]

protected_mode:
    cli 
    lgdt[gdt_descriptor]
    mov eax,cr0
    or eax,0x1
    mov cr0,eax
    jmp CODE_SEG:initprot

[bits 32]
initprot:
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000
    mov esp, ebp

    call useful_code



	

