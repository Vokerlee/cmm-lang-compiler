.686
.model flat, stdcall
option casemap: none

include                 D:\masm32\include\msvcrt.inc
include                 D:\masm32\include\kernel32.inc
include                 D:\masm32\include\windows.inc

includelib              D:\masm32\lib\kernel32.lib
includelib              D:\masm32\lib\user32.lib
includelib              D:\masm32\lib\Exit_label

STD_OUTPUT_HANDLE equ   -11d
STD_INPUT_HANDLE  equ   -10d


GetStdHandle            proto :dword
WriteConsoleA           proto :dword, :dword, :dword, :dword, :dword
ReadConsole             proto :dword, :dword, :dword, :dword, :dword
ExitProcess             proto :dword

WriteSym                proto :byte    
Atoi                    proto  

GetNumber               proto
ToDec                   proto :dword
ExitProgram             proto


.data
smbol                   db 0
hex                     db "0123456789abcdef"
temp_string             db 64 dup (0)
                			
bytesRead               dw 0
hInput                  dd 0


.code

;==========================================================================
COMMENT @
    Write symbol to console macro
    Arguments:
        - sym - symbol to write         
@
;==========================================================================
WriteSym        proc, sym: byte
                nop
                
                push eax
                push ebx
                push ecx
                push edx

                invoke GetStdHandle, STD_OUTPUT_HANDLE

                xor edx, edx
                mov dl, sym
                
                mov byte ptr smbol, dl

                invoke WriteConsoleA, eax, offset smbol, 1, 0, 0

                pop edx
                pop ecx
                pop ebx
                pop eax

                nop
                ret 
                
WriteSym        endp               
;==========================================================================



;==========================================================================
COMMENT @
        Useful define for PrintNumber function
@
;==========================================================================
DivTen          macro ExitLabel, NextLabel
                nop

                push edx
                xor edx, edx                
                mov eax, ebx
                mov ebx, 10         
                div ebx             

                cmp eax, 0
                je ExitLabel               
  
                xchg eax, ebx
                pop eax

                jmp NextLabel

                nop
                endm
;==========================================================================



;==========================================================================
COMMENT @
    Convert number from decimal to string
    Arguments:
        - num - number to convert 
@
;==========================================================================
PrintNumber     proc num: dword

                push eax
                push edx
                push ebx

                mov ebx, 1000000000d    ; ebx = 10^9
                push num
                
Next_prep:
                xor edx, edx
                mov eax, num
                div ebx

                cmp eax, 0
                jne Convert

                DivTen Zero, Next_prep

Convert:        
                pop eax
                xor edx, edx
                
Next:
                div ebx                 ; eax = eax / ebx
                                        ; edx = eax % ebx
                add al, '0'             ; al = '0'
                invoke WriteSym, al     ; putchar(al)

                DivTen EndPrint, Next

Zero:
                invoke WriteSym, '0'
				
EndPrint:
                invoke WriteSym, 10
			 	
                pop ebx
                pop edx
                pop eax
                
                ret
                
PrintNumber     endp
;==========================================================================



;==========================================================================
COMMENT @
    Convert string to decimal number
    Arguments:
        - edi - the address of string to convert
@
;==========================================================================
Atoi            proc

                push esi
                push ebx

                xor eax, eax
                mov ebx, 10
     
AtoiConvert:
                movzx esi, byte ptr [edi]   ; Get the current character

                test esi, esi               ; Check for \0
                je EndAtoi
                cmp esi, 0Ah                ; Check for \0
                je EndAtoi
                cmp esi, 0Dh                ; Check for \0
                je EndAtoi

                cmp esi, '0'                ; Anything less than 0 is invalid
                jb ErrorAtoi
    
                cmp esi, '9'                ; Anything greater than 9 is invalid
                ja ErrorAtoi

                sub esi, '0'                ; Convert from ASCII to decimal 
                mul ebx                     ; Multiply total by 10   
                add eax, esi                ; Add current digit to total
    
                inc edi                     ; Get the address of the next character
                jmp AtoiConvert

ErrorAtoi:
                mov eax, -1                 ; Return -1 on error

EndAtoi:
                pop ebx
                pop esi

                ret
                
Atoi            endp
;==========================================================================



;==========================================================================
COMMENT @
    Read the number from console and put it in eax
    Register eax is destroyed!!
@
;==========================================================================
GetNumber       proc

                push edi

                invoke GetStdHandle, STD_INPUT_HANDLE
                mov hInput, eax
                invoke ReadConsole, hInput, addr temp_string, sizeof temp_string - 1, addr bytesRead, 0

                mov edi, offset temp_string
                call Atoi

                pop edi

                ret
                
GetNumber       endp
;==========================================================================



;==========================================================================
COMMENT @
    Exit from the program        
@
;==========================================================================
ExitProgram     proc

                invoke ExitProcess, 0
                
ExitProgram     endp               
;==========================================================================



;==========================================================================
; Main function, this is the DLL entry point that Windows will call
;==========================================================================
LibMain         proc hInstDLL:dword, reason:dword, unused:dword 

                .if reason == DLL_PROCESS_ATTACH
                mov eax, TRUE                  ; return TRUE so DLL will start

                .elseif reason == DLL_PROCESS_DETACH
                mov eax, FALSE 
			
                .endif
			
                ret

LibMain         endp

end LibMain
;==========================================================================
