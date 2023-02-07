%define BUFFER_SIZE 255
%define OFFSET 8
%include "lib.inc"
%include "words.inc"

section .bss
    buffer: resb BUFFER_SIZE                                        ; Создадим буффер
section .rodata
    buffer_size_msg: db "Error! Buffer max size exceeded",0         ; Зададим сообщения об ошибке
    no_key_msg: db "Error! Key wasn't found",0
    %define bsm_size 32
    %define nkm_size 24


global _start
extern find_word
section .text                                                       
    _start:
        mov rdi, buffer                                             ; Передадим в качестве аргументов функции read_word начало буффера и его размер
        mov rsi, BUFFER_SIZE
        call read_word                                              ; Прочитаем слово в буффер
        test rax,rax                                                ; Если возникла ошибка по переполнению буффера - перейдем к ее обработке
        jz .buffer_size_err                                          
        mov rdi, buffer                                             ; Передадим в качестве аргументов функции find_word начало буффера и указатель на последний элемент
        mov rsi, pointer
        call find_word                                              ; Найдем слово
        test rax,rax                                                ; Если мы не нашли такого ключа, перейдем к обработке этой ошибки
        jz .no_key_err  
        mov rdi,rax                                                 ; Иначе у нас есть адрес элемента словаря с подходящим ключем
        add rdi, OFFSET                                             ; Пропустим указатель
        push rdi
        call string_length                                          ; Считаем длину ключа
        pop rdi
        add rdi, rax                                                ; Пропустим ключ
        inc rdi                                                     ; И нуль-терминатор ключа
        call print_string                                           ; Выведем значение
        call exit                       
        

        .buffer_size_err:
            mov rsi, buffer_size_msg                                ; Передадим сообщения об ошибке и их длины
            mov rdx, bsm_size
            jmp .print_err
        .no_key_err:
            mov rsi, no_key_msg
            mov rdx, nkm_size
        .print_err:
            mov rax, 1                                              ; syscall sys_write
            mov rdi, 2                                              ; дескриптор stderr
            syscall 
            call exit
