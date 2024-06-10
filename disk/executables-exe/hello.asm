[BITS 32]

SECTION .text

global main

main:
	push ebp
	mov ebp , esp
	
	pop ebp
	ret

SECTION .data

string_hello: db "Hello world!" , 0x0D , 0x0A , 0x00
