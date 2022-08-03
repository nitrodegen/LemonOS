
global cleanafter

cleanafter:
    pusha
    mov eax,0
    mov ebx,0
    mov ecx,0
    mov esp,0
    mov ebp,0
    mov edi,0
    mov bp,0x9000
    mov sp,bp
    
    hlt
    jmp $
