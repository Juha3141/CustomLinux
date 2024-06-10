[BITS 64]

SECTION .text

global _start

_start:
        push rbp
        mov rbp , rsp
        
        mov rax , 548
        syscall
        
        mov rax , 69 ; call number
        mov rdi , 1  ; stdout
        mov rsi , string_hello
        mov rdx , 15
        syscall
        
        mov rax , 60
        syscall

SECTION .data

string_hello: db "Hello, world!" , 0x0D , 0x0A , 0x00
