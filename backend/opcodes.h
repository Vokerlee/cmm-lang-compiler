#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

// SOME DEFINES

#define PUT_DWORD(value)                                   \
    *((DWORD*)(text_section + CODE_CURRENT_ADDR)) = value; \
    CODE_CURRENT_ADDR += sizeof(DWORD);

#define PUT_WORD(value)                                    \
    *((WORD*)(text_section + CODE_CURRENT_ADDR)) = value;  \
    CODE_CURRENT_ADDR += sizeof(WORD);

#define PUT_BYTE(value)                                    \
     text_section[CODE_CURRENT_ADDR++] = value;

// ALL OPCODES

#define NOP 0x90

#define CALL_DWORD_PTR_DS 0x15ff
#define CALL_DWORD_PTR_CS1 0xfff2e
#define CALL_DWORD_PTR_CS2 0x15

#define MOV_ECX_ESP_MEM1 0x4c8b
#define MOV_ECX_ESP_MEM2 0x24
 
#define MOV_EBP_ESP 0xe589
#define MOV_ESP_EBP 0xec89

// MOV <REG>, [EDI + NUMBER]

#define MOV_EAX_EDI_MEM 0x478b
#define MOV_ECX_EDI_MEM 0x4f8b
#define MOV_EDX_EDI_MEM 0x578b
#define MOV_EBX_EDI_MEM 0x5f8b

// MOV [EDI + NUMBER], <REG>

#define MOV_EDI_MEM_EAX 0x4789
#define MOV_EDI_MEM_ECX 0x4f89
#define MOV_EDI_MEM_EDX 0x5789
#define MOV_EDI_MEM_EBX 0x5f89

// MOV <REG>, [ESI + NUMBER]

#define MOV_EAX_ESI_MEM 0x468b
#define MOV_ECX_ESI_MEM 0x4e8b
#define MOV_EDX_ESI_MEM 0x568b
#define MOV_EBX_ESI_MEM 0x5e8b

// MOV [ESI + NUMBER], <REG>

#define MOV_ESI_MEM_EAX 0x4689
#define MOV_ESI_MEM_ECX 0x4e89
#define MOV_ESI_MEM_EDX 0x5689
#define MOV_ESI_MEM_EBX 0x5e89

// PUSH <REG>

#define PUSH_EAX 0x50
#define PUSH_ECX 0x51
#define PUSH_EDX 0x52
#define PUSH_EBX 0x53
#define PUSH_EBP 0x55
#define PUSH_ESI 0x56
#define PUSH_EDI 0x57

#define PUSH 0x68 

// POP <REG>

#define POP_EAX 0x58
#define POP_ECX 0x59
#define POP_EDX 0x5a
#define POP_EBX 0x5b
#define POP_EBP 0x5d
#define POP_ESI 0x5e
#define POP_EDI 0x5f

// MOV EAX, <REG>

#define MOV_EAX_EAX 0xc08b
#define MOV_EAX_ECX 0xc18b
#define MOV_EAX_EDX 0xc28b
#define MOV_EAX_EBX 0xc38b
#define MOV_EAX_ESP 0xc48b
#define MOV_EAX_EBP 0xc58b
#define MOV_EAX_ESI 0xc68b
#define MOV_EAX_EDI 0xc78b

// MOV ECX, <REG>

#define MOV_ECX_EAX 0xc88b
#define MOV_ECX_ECX 0xc98b
#define MOV_ECX_EDX 0xca8b
#define MOV_ECX_EBX 0xcb8b
#define MOV_ECX_ESP 0xcc8b
#define MOV_ECX_EBP 0xcd8b
#define MOV_ECX_ESI 0xce8b
#define MOV_ECX_EDI 0xcf8b

// MOV EDX, <REG>

#define MOV_EDX_EAX 0xd08b
#define MOV_EDX_ECX 0xd18b
#define MOV_EDX_EDX 0xd28b
#define MOV_EDX_EBX 0xd38b
#define MOV_EDX_ESP 0xd48b
#define MOV_EDX_EBP 0xd58b
#define MOV_EDX_ESI 0xd68b
#define MOV_EDX_EDI 0xd78b

// MOV EBX, <REG>

#define MOV_EBX_EAX 0xd88b
#define MOV_EBX_ECX 0xd98b
#define MOV_EBX_EDX 0xda8b
#define MOV_EBX_EBX 0xdb8b
#define MOV_EBX_ESP 0xdc8b
#define MOV_EBX_EBP 0xdd8b
#define MOV_EBX_ESI 0xde8b
#define MOV_EBX_EDI 0xdf8b

// MOV NUM TO REG

#define MOV_EAX 0xb8
#define MOV_ECX 0xb9
#define MOV_EDX 0xba
#define MOV_EBX 0xbb
#define MOV_ESP 0xbc
#define MOV_EBP 0xbd
#define MOV_ESI 0xbe
#define MOV_EDI 0xbf

// ADD <REG>, EAX +

#define ADD_EAX_EAX 0xc001
#define ADD_ECX_EAX 0xc101
#define ADD_EDX_EAX 0xc201
#define ADD_EBX_EAX 0xc301
#define ADD_ESP_EAX 0xc401
#define ADD_EBP_EAX 0xc501
#define ADD_ESI_EAX 0xc601
#define ADD_EDI_EAX 0xc701

// ADD <REG>, ECX +

#define ADD_EAX_ECX 0xc801
#define ADD_ECX_ECX 0xc901
#define ADD_EDX_ECX 0xca01
#define ADD_EBX_ECX 0xcb01
#define ADD_ESP_ECX 0xcc01
#define ADD_EBP_ECX 0xcd01
#define ADD_ESI_ECX 0xce01
#define ADD_EDI_ECX 0xcf01

// ADD <REG>, EDX +

#define ADD_EAX_EDX 0xd001
#define ADD_ECX_EDX 0xd101
#define ADD_EDX_EDX 0xd201
#define ADD_EBX_EDX 0xd301
#define ADD_ESP_EDX 0xd401
#define ADD_EBP_EDX 0xd501
#define ADD_ESI_EDX 0xd601
#define ADD_EDI_EDX 0xd701

// ADD <REG>, EBX +

#define ADD_EAX_EBX 0xd801
#define ADD_ECX_EBX 0xd901
#define ADD_EDX_EBX 0xda01
#define ADD_EBX_EBX 0xdb01
#define ADD_ESP_EBX 0xdc01
#define ADD_EBP_EBX 0xdd01
#define ADD_ESI_EBX 0xde01
#define ADD_EDI_EBX 0xdf01

// ADD NUM TO REG +

#define ADD_EAX 0xc081
#define ADD_ECX 0xc181
#define ADD_EDX 0xc281
#define ADD_EBX 0xc381
#define ADD_ESP 0xc481
#define ADD_EBP 0xc581
#define ADD_ESI 0xc681
#define ADD_EDI 0xc781

// SUB EAX, <REG> +

#define SUB_EAX_EAX 0xc02b
#define SUB_EAX_ECX 0xc12b
#define SUB_EAX_EDX 0xc22b
#define SUB_EAX_EBX 0xc32b
#define SUB_EAX_ESP 0xc42b
#define SUB_EAX_EBP 0xc52b
#define SUB_EAX_ESI 0xc62b
#define SUB_EAX_EDI 0xc72b

// SUB ECX, <REG> +

#define SUB_ECX_EAX 0xc82b
#define SUB_ECX_ECX 0xc92b
#define SUB_ECX_EDX 0xca2b
#define SUB_ECX_EBX 0xcb2b
#define SUB_ECX_ESP 0xcc2b
#define SUB_ECX_EBP 0xcd2b
#define SUB_ECX_ESI 0xce2b
#define SUB_ECX_EDI 0xcf2b

// SUB EDX, <REG> +

#define SUB_EDX_EAX 0xd02b
#define SUB_EDX_ECX 0xd12b
#define SUB_EDX_EDX 0xd22b
#define SUB_EDX_EBX 0xd32b
#define SUB_EDX_ESP 0xd42b
#define SUB_EDX_EBP 0xd52b
#define SUB_EDX_ESI 0xd62b
#define SUB_EDX_EDI 0xd72b

// SUB EBX, <REG> +

#define SUB_EBX_EAX 0xd82b
#define SUB_EBX_ECX 0xd92b
#define SUB_EBX_EDX 0xda2b
#define SUB_EBX_EBX 0xdb2b
#define SUB_EBX_ESP 0xdc2b
#define SUB_EBX_EBP 0xdd2b
#define SUB_EBX_ESI 0xde2b
#define SUB_EBX_EDI 0xdf2b

// SUB NUM FROM REG +

#define SUB_EAX 0xe881
#define SUB_ECX 0xe981
#define SUB_EDX 0xea81
#define SUB_EBX 0xeb81
#define SUB_ESP 0xec81
#define SUB_EBP 0xed81
#define SUB_ESI 0xee81
#define SUB_EDI 0xef81

// CMP <REG>, EAX +

#define CMP_EAX_EAX 0xc039
#define CMP_ECX_EAX 0xc139
#define CMP_EDX_EAX 0xc239
#define CMP_EBX_EAX 0xc339
#define CMP_ESP_EAX 0xc439
#define CMP_EBP_EAX 0xc539
#define CMP_ESI_EAX 0xc639
#define CMP_EDI_EAX 0xc739

// CMP <REG>, ECX +

#define CMP_EAX_ECX 0xc839
#define CMP_ECX_ECX 0xc939
#define CMP_EDX_ECX 0xca39
#define CMP_EBX_ECX 0xcb39
#define CMP_ESP_ECX 0xcc39
#define CMP_EBP_ECX 0xcd39
#define CMP_ESI_ECX 0xce39
#define CMP_EDI_ECX 0xcf39

// CMP <REG>, EDX +

#define CMP_EAX_EDX 0xd039
#define CMP_ECX_EDX 0xd139
#define CMP_EDX_EDX 0xd239
#define CMP_EBX_EDX 0xd339
#define CMP_ESP_EDX 0xd439
#define CMP_EBP_EDX 0xd539
#define CMP_ESI_EDX 0xd639
#define CMP_EDI_EDX 0xd739

// CMP <REG>, EBX +

#define CMP_EAX_EBX 0xd839
#define CMP_ECX_EBX 0xd939
#define CMP_EDX_EBX 0xda39
#define CMP_EBX_EBX 0xdb39
#define CMP_ESP_EBX 0xdc39
#define CMP_EBP_EBX 0xdd39
#define CMP_ESI_EBX 0xde39
#define CMP_EDI_EBX 0xdf39

// CMP NUM WITH REG +

#define CMP_EAX 0xf881
#define CMP_ECX 0xf981
#define CMP_EDX 0xfa81
#define CMP_EBX 0xfb81
#define CMP_ESP 0xfc81
#define CMP_EBP 0xfd81
#define CMP_ESI 0xfe81
#define CMP_EDI 0xff81

// MUL <REG> +

#define MUL_EAX 0xe0f7
#define MUL_ECX 0xe1f7
#define MUL_EDX 0xe2f7
#define MUL_EBX 0xe3f7
#define MUL_ESP 0xe4f7
#define MUL_EBP 0xe5f7
#define MUL_ESI 0xe6f7
#define MUL_EDI 0xe7f7

// DIV <REG> +

#define DIV_EAX 0xf0f7
#define DIV_ECX 0xf1f7
#define DIV_EDX 0xf2f7
#define DIV_EBX 0xf3f7
#define DIV_ESP 0xf4f7
#define DIV_EBP 0xf5f7
#define DIV_ESI 0xf6f7
#define DIV_EDI 0xf7f7

// XOR E*X, E*X

#define XOR_EAX_EAX 0x33c0
#define XOR_ECX_ECX 0x33c9
#define XOR_EDX_EDX 0x33d2
#define XOR_EBX_EBX 0x33db


// JMPS <REG>

#define JMP_EAX 0xe0ff
#define JMP_ECX 0xe1ff
#define JMP_EDX 0xe2ff
#define JMP_EBX 0xe3ff
#define JMP_ESP 0xe4ff
#define JMP_EBP 0xe5ff
#define JMP_ESI 0xe6ff
#define JMP_EDI 0xe7ff

// JMPS RELATIVE

#define JMP_REL 0xeb
#define JA_REL  0x870f
#define JAE_REL 0x830f
#define JB_REL  0x820f
#define JBE_REL 0x860f
#define JE_REL  0x840f
#define JNE_REL 0x850f

// JMPS

#define JMP 0xe9
#define JA  0x77
#define JAE 0x73
#define JB  0x72
#define JBE 0x76
#define JE  0x74
#define JNE 0x75

// CALL & RET

#define CALL 0xe8
#define RET  0xc3


#endif // OPCODES_H_INCLUDED