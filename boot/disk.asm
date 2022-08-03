disk_setup:
    pusha 
    push dx
    mov ah,0x2 ; read 
    mov al,dh ; load num of sectors
    mov cl,2; sector num
    mov ch,0;always this when loading sectors (cylinder)
    mov dh,0;always this when loading sectors (head)
    int 0x13;hard disk interrupt

    jc disk_error
    pop dx
    cmp al,dh
    jne sect_err
    popa
    ret

disk_error:
    mov si,derr_msg
    mov cx, derr_l
    call print16
    jmp $;while(true){}
sect_err:
    mov si,sect_msg
    mov cx,sectl
    call print16
    jmp $ ;while(true){}


derr_msg:
    db "DISK ERROR!",0
derr_l equ $- derr_msg

sect_msg:
    db "SECTOR ERROR!",0
sectl equ $- sect_msg

