#!/bin/bash
nasm hello.asm -f elf64 -o hello.obj
ld hello.obj -o hello

cp new_signature.bin hello-modified
chmod +x hello-modified

file_size=$(stat -c %s hello)
file_size=$((file_size-4))

dd if=hello of=hello-modified seek=4 skip=4 count=${file_size} bs=1
