[org 0x7c00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
mov bp, 0x9000
mov sp, bp

call load_kern
mov ax,4F02h
mov bx,10Ch
int 0x10

;switch to non-cursor VGA mode
call protected_mode 

jmp $

%include "./boot/disk.asm"
%include "./boot/gdt.asm"
%include "./boot/switch32.asm"
;16bit functions
[bits 16]
print16:
    push si
    push cx
    pusha
    mov bx,0
    sub cx,1
loop:
    mov ah,0x0e
    mov al,[si+bx]
    int 0x10
    inc bx
    cmp bx,cx
    jne loop
    popa
    ret

[bits 16]
load_kern:

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 53; THIS FUCKING NUMBER CRASHED MY WHOLE CODE FOR WHOLE FUCKING DAY!
    ; IT SHOULD FOR SOME REASON BE 31 NOT FUCKING 2!
    ;just found out, we need larger kernel (MORE SECTORS!)
    
    mov dl, [BOOT_DRIVE]
    call disk_setup
    ret

[bits 32]
useful_code:
    ;testing 32bit
    call KERNEL_OFFSET
    
    jmp $

;DATA section of code
BOOT_DRIVE db 0

times 510-($-$$) db 0 
dw 0xaa55