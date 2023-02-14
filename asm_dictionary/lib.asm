 
%define space 0x20
%define tab   0x9
%define new_line 0xA

global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy

section .text
; Принимает код возврата и завершает текущий процесс
exit: 	
    mov rax, 60                 ; Функция exit
    syscall 

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax,rax
    .count:
        cmp byte[rdi+rax], 0    ; Ищем нуль-символ
        je .end                 ; Если нуль - выход
        inc rax                 ; Иначе увеличим аккумулятор и повторим цикл
        jmp .count
    .end:
        ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    push rdi                    ; Сохраним строку
    call string_length          ; Считаем длину строки
    pop rsi                     ; Передаем строку
    mov rdx, rax                ; Передаем длину строки
    mov rax, 1                  ; Функция write
    mov rdi, 1                  ; Дескриптор stdout
    syscall
    ret

; Принимает код символа и выводит его в stdout
print_char:
    mov rax, 1                  ; Функция write
    push rdi                    ; Сохраним char на стэке
    mov rsi, rsp                ; Передаем char
    pop rdi                     ; Возвращаем стэк в исходное состояние
    mov rdx, 1                  ; Длина 1 символ
    mov rdi, 1                  ; Дескриптор stdout
    syscall    
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, new_line
    jmp print_char

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    mov r11, rsp                ; Сохраним вершину стека
    mov rax, rdi                ; Запишем число в rax для деления
    xor rdx, rdx                ; Обнулим rdx
    dec rsp                     ; Запишем в стэк байт нуль-терминатор
    mov byte[rsp],dl
    .loop: 
        mov rcx, 10            ; Поделим число в rdx:rax на 10   
        div rcx
        add rdx,'0'             ; Переведем цифру в ASCII символ
        dec rsp                 ; Запишем цифру на стэк
        mov byte[rsp],dl
        xor rdx,rdx             ; Обнулим rdx
        cmp rax,0               ; Если частное больше нуля продолжим цикл
        ja .loop
    mov rdi,rsp                 ; Передадим вершину стэка в качестве указателя на строку
    push r11                    ; Сохраним старую вершину стэка
    call print_string           ; Напечатаем число, как строку
    pop rsp                     ; Вернем стэк в исходную позицию
    ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
   test rdi,rdi                 ; Установим флаги по числу
   jns .pos                     ; Если положительное, то перейдем к выполнению программы для беззнакового числа
   neg rdi                      ; Иначе инвертируем 
   push rdi                     ; Сохраним на стэке
   mov rdi, '-'                 ; Напечатаем '-'
   call print_char
   pop rdi
   .pos: jmp print_uint

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rcx,rcx                 ; Обнулим сетчик
    .loop:
        mov r11b, byte[rsi+rcx] ; Прочитаем символ из первого слова
        cmp byte[rdi+rcx], r11b ; Сравним его с символом из второго слова
        jne .false              ; Если они не равны, то выдадим 0
        test r11b,r11b          ; Иначе если они равны и мы нашли нуль-терминатор, то выдадим 1
        jz .true 
        inc rcx
        jmp .loop
    .false:
        mov rax, 0
        ret
    .true:
        mov rax,1
        ret
        
; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    mov rax, 0                  ; Функция read
    mov rdi, 0                  ; Дескриптор stdin
    dec rsp                     ; Переведем указатель на вершину стека на один байт вверх
    mov byte[rsp], 0            ; Почистим буффер
    mov rsi,rsp                 ; Положим его в качестве буффера функции read
    mov rdx, 1                  ; Читаем 1 символ
    syscall
    mov al, byte[rsp]           ; Положим в аккумулятор значение из буффера
    inc rsp                     ; Вернем стэк в исходное состояние
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
read_word:
     xor rcx,rcx                ; Очистим счетчик
    .loop:
        cmp rcx, rsi            ; Сравним текущую длину строки и размер буффера
        jge .fail               ; Если больше или равно, то выдадим ошибку
        push rdi                ; Сохраним необходимые нам регистры в стэке
        push rsi
        push rcx
        call read_char          ; Прочитаем символ из потока
        pop rcx                 ; Достанем регистры обратно
        pop rsi
        pop rdi
        cmp al, space           ; Сравним прочитанный символ с " " пробелом
        je .skip                ; Если равны перейдем в пропуск
        cmp al, tab             ; Сравним прочитанный символ с "    " табуляцией
        je .skip
        cmp al, new_line        ; Сравним прочитанный символ с "\n" переносом строки
        je .skip
        mov byte[rdi+rcx],al    ; Запишем прочитанный символ в буффер
        cmp al,0                ; Если он 0, то есть поток кончился, перейдем в успешное выполнение программы
        je .success
        inc rcx                 ; Иначе увеличим длину на 1
        jmp .loop               ; Повторим цикл

    .skip:
        test rcx,rcx            ; Проверим длину на 0     
        jz .loop                ; Если равно нулю, то это те пробельные символы перед словом, которые надо пропустить, то есть мы продолжим цикл,
        mov byte[rdi+rcx],0     ; Запишем нуль-терминатор
    .success:                   ; Иначе мы прочитали слово полностью, то есть попали в условие успешного исполнения программы
        mov rax, rdi            ; Поместим в rax адрес буффера
        mov rdx, rcx            ; А в rdx длину строки
        ret
    .fail:
        xor rax, rax            ; При ошибке выведем 0 в rax
        ret
 
; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax,rax                 ; Очистим rax
    xor rdx,rdx                 ; Очистим счетчик
    .read_digit_state:
        cmp byte[rdi+rdx], '0'  ; Проверим прочитанный символ на цифру: он должен лежать между 0x30 и 0x39
        jl .end_state           ; Иначе мы нашли сивол не цифру - выходим из цикла
        cmp byte[rdi+rdx], '9'  
        jg .end_state
        mov r8, rdx             ; Сохраним rdx т.к. он изменяется при mul
        mov rdx,10              ; Положим в rdx 10
        mul rdx                 ; Умножим rax на 10
        mov rdx, r8             ; Вернем на место rdx
        xor r11,r11             ; Очистим r11
        mov r11b,byte[rdi+rdx]  ; Положим туда код прочитанного символа
        add rax,r11             ; Добавим к итоговому числу код символа
        sub rax, '0'            ; Вычтем код '0' чтобы получить цифру
        inc rdx                 ; Увеличим длину числа
        jmp .read_digit_state   ; Повторим цикл
    .end_state:
        ret                                      
    
; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    cmp byte[rdi], '-'          ; Сравним с '-' полученный символ
    je  .sign                   ; Если равно переместимся к обработке отрицательного числа
    cmp byte[rdi], '+'          ; Сравним с '+'
    jne parse_uint              ; Если не равен, то попробуем просто обработать, как беззнаковое число
    .sign:                      ; Иначе обработаем, как число, начинающееся с '+'
        push rdi                ; Сохраним адрес строки
        inc rdi                 ; Начнем обработку со следующего символа
        call parse_uint         ; Обрабатываем, как беззнаковое число
        pop rdi                 ; Достанем со стека исходный адрес
        cmp rdx, 0              ; Сравним длину численной части с нулем
        je .err                 ; Если нулевая, то мы прочитали не число, а знак
        inc rdx                 ; Добавим символ в длину
        cmp byte[rdi], '-'      ; Сравним знак с минусом
        jne .err                ; Если знак не минус, то выйдем
        neg rax                 ; Иначе инвертируем число
    .err:
        ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    xor rax, rax                ; Очистим счетчик
    .loop:
        cmp rax, rdx            ; Если длина слова больше или равна размеру буфера - выкинем ошибку
        jge .err
        mov r11b, byte[rdi+rax] ; Переместим символ в r11 из строки
        mov byte[rsi+rax], r11b ; Переместим символ в буффер из r11
        cmp byte[rsi+rax], 0x0  ; Если прочитанный символ - нуль-терминатор, то мы успешно скопировали строку
        jz .success
        inc rax                 ; Иначе продолжим цикл, увеличив длину на 1
        jmp .loop
    .err: 
        xor rax,rax             ; При оштбке выдадим 0
    .success:
        ret