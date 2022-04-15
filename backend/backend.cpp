#include "backend.h"

DWORD CODE_CURRENT_ADDR = 0;

extern DWORD EXIT_ADDR;
extern DWORD GET_NUMBER_ADDR;
extern DWORD PRINT_NUMBER_ADDR;

DWORD  FUNC_ADDRS[MAX_N_FUNCS]       = { 0 };
WORD   FUNC_NUMBER[MAX_N_FUNCS]      = { 0 };
BYTE   FUNC_COUNTER                  = 0;

DWORD* CALL_ADDRS[MAX_N_FUNC_CALLS]       = { 0 };
WORD   FUNC_CALL_NUMBER[MAX_N_FUNC_CALLS] = { 0 };
WORD   FUNC_PTR_CALLS[MAX_N_FUNC_CALLS]   = { 0 };
BYTE   CALL_COUNTER                       = 0;

void backend (FILE *tree_lang, FILE *exe_file)
{
    assert(tree_lang);
    assert(exe_file);

    text_t text = {};
    construct_text(&text);

    fill_text(tree_lang, &text);

    text.counter = text.lines[0].line;

    while ((isspace(*(text.counter)) || *(text.counter) == '{') && *(text.counter) != '\0')
    {
        if (*(text.counter) == '\n' && text.line_counter < text.n_real_lines - 1)
            text.counter = text.lines[++(text.line_counter)].line;
        else
            text.counter++;
    }

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    variables var = {};

    tree.root = fill_tree(&text, &var);

    create_exe(&tree, exe_file, &var);

    destruct_text(&text);
    destruct_tree(&tree);
}

void fill_text_sec (bin_tree *tree, BYTE *text_section, variables *var)
{
    PUT_BYTE(MOV_EDI)
    PUT_DWORD(IMAGE_BASE + 2 * VRT_SIZE + ENTRY_POINT_ADDR)
    
    PUT_BYTE(MOV_ESI)
    PUT_DWORD(IMAGE_BASE + 2 * VRT_SIZE + ENTRY_POINT_ADDR + 2 * VRT_SIZE / 3)

    bin_tree_elem* vertex = tree->root;

    int n_glob_vars = 0;

    while (vertex->left->type == COMMAND && (int) vertex->left->value == ASSIGN)
    {
        encode_expr(vertex->left->right, text_section, var);

        PUT_WORD(MOV_ESI_MEM_EBX)
        PUT_BYTE((int) vertex->left->value * sizeof(int))

        vertex = vertex->right;

        n_glob_vars++;
    }

    find_glob_vars(vertex, n_glob_vars);

    main_var_optimize(vertex, n_glob_vars);

    encode_body(vertex->left->right, text_section, var);

    vertex = vertex->right;

    // EXIT FROM THE PROGRAM

    call_exit_program(text_section);

    // WRITE ALL FUNCTIONS

    while (vertex != nullptr)
    {
        while (CODE_CURRENT_ADDR % 4 != 0)
            PUT_BYTE(NOP)

        FUNC_ADDRS[FUNC_COUNTER]  = CODE_CURRENT_ADDR;
        FUNC_NUMBER[FUNC_COUNTER] = vertex->left->value;

        encode_func(vertex->left, text_section, var);
        vertex = vertex->right;

        FUNC_COUNTER++;
    }

    for (int i = 0; i < CALL_COUNTER; ++i)
    {
        for (int j = 0; j < FUNC_COUNTER; j++)
        {
            if (FUNC_CALL_NUMBER[i] == FUNC_NUMBER[j])
                *(CALL_ADDRS[i]) = FUNC_ADDRS[j] - FUNC_PTR_CALLS[i];
        }   
    }
}

void find_glob_vars (bin_tree_elem *element, int n_glob_vars)
{
    if (element->left != nullptr)
        find_glob_vars(element->left, n_glob_vars);

    if (element->type == VAR && element->value < n_glob_vars)
        element->type = GLOB_VAR;

    if (element->right != nullptr)
        find_glob_vars(element->right, n_glob_vars);
}

void main_var_optimize (bin_tree_elem *element, int n_glob_vars)
{
    if (element->left != nullptr)
        main_var_optimize(element->left, n_glob_vars);

    if (element->type == VAR)
        element->value -= n_glob_vars;

    if (element->right != nullptr)
        main_var_optimize(element->right, n_glob_vars);
}

void encode_body (bin_tree_elem *vertex, BYTE *text_section, variables *var)
{
    while (vertex != nullptr)
    {
        encode_command(vertex->left, text_section, var);
        vertex = vertex->right;
    }
}

void encode_command (bin_tree_elem *element, BYTE *text_section, variables *var)
{
    if (element->type == COMMAND && (int) element->value == ASSIGN)
    {
        encode_expr(element->right, text_section, var);

        if (element->left->type == VAR)
        {
            PUT_WORD(MOV_EDI_MEM_EBX)
            PUT_BYTE((int) element->left->value * sizeof(int))
        }
        else if (element->left->type == GLOB_VAR)
        {
            PUT_WORD(MOV_ESI_MEM_EBX)
            PUT_BYTE((int)element->left->value * sizeof(int))
        }
    }
    else if (element->type == COMMAND && (int) element->value == IF)
    {
        encode_expr(element->left->left,  text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->left->right, text_section, var);
        PUT_BYTE(POP_ECX)

        PUT_WORD(CMP_ECX_EBX)

        switch ((int) element->left->value)
        {
            case JE:
                PUT_BYTE(JNE) break;
            case JA:
                PUT_BYTE(JBE) break;
            case JAE:
                PUT_BYTE(JB) break;
            case JB:
                PUT_BYTE(JAE) break;
            case JBE:
                PUT_BYTE(JA) break;
            case JNE:
                PUT_BYTE(JE) break;
            default:
                break;
        }

        size_t commands_shift = CODE_CURRENT_ADDR + 1;
        BYTE *jmp_addr = text_section + CODE_CURRENT_ADDR; // is to been filled in the end of the procedure
        CODE_CURRENT_ADDR++;

        encode_body(element->right, text_section, var);
        
        *jmp_addr = CODE_CURRENT_ADDR - commands_shift;
    }
    else if (element->type == COMMAND && (int) element->value == WHILE)
    {
        DWORD check_cond = IMAGE_BASE + ENTRY_POINT_ADDR + CODE_CURRENT_ADDR;

        encode_expr(element->left->left,  text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->left->right, text_section, var);
        PUT_BYTE(POP_ECX)

        PUT_WORD(CMP_ECX_EBX)

        switch ((int) element->left->value)
        {
            case JE:
                PUT_BYTE(JNE) break;
            case JA:
                PUT_BYTE(JBE) break;
            case JAE:
                PUT_BYTE(JB) break;
            case JB:
                PUT_BYTE(JAE) break;
            case JBE:
                PUT_BYTE(JA) break;
            case JNE:
                PUT_BYTE(JE) break;
            default:
                break;
        }

        size_t commands_shift = CODE_CURRENT_ADDR + 1;
        BYTE *jmp_invalid = text_section + CODE_CURRENT_ADDR; // is to been filled in the end of the procedure
        CODE_CURRENT_ADDR++;

        encode_body(element->right, text_section, var);

        PUT_BYTE(MOV_ECX)
        PUT_DWORD(check_cond)

        PUT_WORD(JMP_ECX)
        
        *jmp_invalid = CODE_CURRENT_ADDR - commands_shift;
    }
    else if (element->type == FUNC && ((int) element->value == SCAN || (int) element->value == PRINT))
    {
        if ((int) element->value == SCAN)
        {
            call_get_number(text_section);

            PUT_WORD(MOV_EDI_MEM_EAX)
            PUT_BYTE((int) element->left->value * sizeof(int))
        }
        else if ((int) element->value == PRINT)
        {
            encode_expr(element->left, text_section, var);
            PUT_BYTE(PUSH_EBX)

            call_print_number(text_section);
        }
    }
    else if (element->type == USER_FUNC)
    {
        int n_params = 0;

        for (bin_tree_elem* vertex = element->left; vertex != nullptr; vertex = vertex->left)
            n_params++;

        PUT_BYTE(PUSH_EDI)

        for (bin_tree_elem* vertex = element->left; vertex != nullptr; vertex = vertex->left)
        {
            encode_expr(vertex->right, text_section, var);
            PUT_BYTE(PUSH_EBX)
        }

        PUT_WORD(ADD_EDI)
        PUT_DWORD(n_params * sizeof(DWORD))

        PUT_BYTE(CALL)

        CALL_ADDRS[CALL_COUNTER] = (DWORD*) (text_section + CODE_CURRENT_ADDR);
        CODE_CURRENT_ADDR += sizeof(DWORD);
        FUNC_CALL_NUMBER[CALL_COUNTER] = (WORD) element->value;
        FUNC_PTR_CALLS[CALL_COUNTER++] = (WORD) CODE_CURRENT_ADDR;

        PUT_WORD(ADD_ESP)
        PUT_DWORD(n_params * sizeof(DWORD))

        PUT_BYTE(POP_EDI)
    }
    else if (element->type == RETURN)
    {
        if (element->left != nullptr)
            encode_expr(element->left, text_section, var);

        PUT_WORD(MOV_ESP_EBP)
        PUT_BYTE(POP_EBP)

        PUT_BYTE(RET)
    }
}

void encode_expr (bin_tree_elem *element, BYTE *text_section, variables *var)
{
    if (element->type == NUM)
    { 
        PUT_BYTE(MOV_EBX)
        PUT_DWORD((int) element->value)
    }
    else if (element->type == GLOB_VAR)
    {
        PUT_WORD(MOV_EBX_ESI_MEM)
        PUT_BYTE((int)element->value * sizeof(int))
    }
    else if (element->type == VAR)
    {
        // mov ebx, [edi + element->value]
        PUT_WORD(MOV_EBX_EDI_MEM)
        PUT_BYTE((int) element->value * sizeof(int))
    }
    else if (element->type == USER_FUNC)
    {
        int n_params = 0;

        for (bin_tree_elem* vertex = element->left; vertex != nullptr; vertex = vertex->left)
            n_params++;

        PUT_BYTE(PUSH_EDI)

        for (bin_tree_elem* vertex = element->left; vertex != nullptr; vertex = vertex->left)
        {
            encode_expr(vertex->right, text_section, var);
            PUT_BYTE(PUSH_EBX)
        }
        
        PUT_WORD(ADD_EDI)
        PUT_DWORD(n_params * sizeof(DWORD))

        PUT_BYTE(CALL)

        CALL_ADDRS[CALL_COUNTER] = (DWORD*)(text_section + CODE_CURRENT_ADDR);
        CODE_CURRENT_ADDR += sizeof(DWORD);
        FUNC_CALL_NUMBER[CALL_COUNTER] = (WORD) element->value;
        FUNC_PTR_CALLS[CALL_COUNTER++] = (WORD) CODE_CURRENT_ADDR;

        PUT_WORD(ADD_ESP)
        PUT_DWORD(n_params * sizeof(DWORD))

        PUT_BYTE(POP_EDI)  
    }
    else if (element->type == OPER && (int) element->value == ADD)
    {
        encode_expr(element->left, text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->right, text_section, var);
        PUT_BYTE(POP_ECX)

        PUT_WORD(ADD_EBX_ECX)
    }
    else if (element->type == OPER && (int) element->value == SUB)
    {
        encode_expr(element->left, text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->right, text_section, var);
        PUT_BYTE(POP_ECX)

        PUT_WORD(SUB_ECX_EBX)
        PUT_WORD(MOV_EBX_ECX)
    }
    else if (element->type == OPER && (int) element->value == MUL)
    {
        encode_expr(element->left, text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->right, text_section, var);
        PUT_BYTE(POP_EAX)

        PUT_WORD(MUL_EBX)
        PUT_WORD(MOV_EBX_EAX)
    }
    else if (element->type == OPER && (int) element->value == DIV)
    {
        encode_expr(element->left, text_section, var);
        PUT_BYTE(PUSH_EBX)

        encode_expr(element->right, text_section, var);
        PUT_BYTE(POP_EAX)

        PUT_WORD(XOR_EDX_EDX)
        PUT_WORD(DIV_EBX)
        PUT_WORD(MOV_EBX_EAX)
    }
}

void find_func_var (bin_tree_elem *element, int *param, int *n_param, int first_loc_var)
{
    if (element->left != nullptr)
        find_func_var(element->left, param, n_param, first_loc_var);

    if (element->type == VAR)
    {
        int find_state = 0;
        int n_params = *n_param;

        for (int i = first_loc_var; i < n_params; i++)
        {
            if (param[i] == (int) element->value)
            {
                find_state = 1;
                element->value = i;
                break;
            }
        }

        if (find_state == 0 && element->value > first_loc_var)
        {
            param[*n_param] = (int) element->value;
            element->value = *n_param;
            (*n_param)++;
        }
    }

    if (element->right != nullptr)
        find_func_var(element->right, param, n_param, first_loc_var);
}

void encode_func (bin_tree_elem *vertex, BYTE *text_section, variables *var)
{
    int *param = (int *) calloc(MAX_FUNC_PARAM, sizeof(int));
    int n_param      = 0;
    int first_loc_var = 0;

    for (bin_tree_elem *elem = vertex->left; elem != nullptr; elem = elem->left)
        param[n_param++] = elem->value;

    first_loc_var = n_param;

    user_func_optimize(vertex, param, n_param);
    find_func_var(vertex->right, param, &n_param, first_loc_var);

    param_pass_func(text_section, first_loc_var);

    PUT_BYTE(PUSH_EBP)
    PUT_WORD(MOV_EBP_ESP)

    encode_body(vertex->right, text_section, var);

    free(param);
}

void param_pass_func (BYTE *text_section, char n_param)
{
    for (int i = 0; i < n_param; ++i)
    {
        // mov ecx, dword ptr [esp + n_param - i]
        PUT_WORD(MOV_ECX_ESP_MEM1)
        PUT_BYTE(MOV_ECX_ESP_MEM2)
        PUT_BYTE(sizeof(DWORD) * (n_param - i))

        // mov [edi + i], ecx
        PUT_WORD(MOV_EDI_MEM_ECX)
        PUT_BYTE(sizeof(DWORD) * i)
    }
}

void user_func_optimize (bin_tree_elem *element, int *param, int n_param)
{
    if (element->left != nullptr)
        user_func_optimize(element->left, param, n_param);

    if (element->type == VAR)
    {
        int find_state = 0;

        for (int i = 0; i < n_param; i++)
        {
            if (param[i] == (int) element->value)
            {
                find_state     = 1;
                element->value = i;
            }
        }

        if (find_state == 0 && element->value < n_param)
            element->value += 128;   // the trick to mark the element of tree (for the future)
    }
    
    if (element->right != nullptr)
        user_func_optimize(element->right, param, n_param);
}

void call_exit_program (BYTE *text_section)
{
    PUT_WORD(CALL_DWORD_PTR_DS)
    PUT_DWORD(EXIT_ADDR)
}

void call_get_number (BYTE *text_section)
{
    PUT_WORD(CALL_DWORD_PTR_DS)
    PUT_DWORD(GET_NUMBER_ADDR)
}

void call_print_number (BYTE *text_section)
{
    PUT_WORD(CALL_DWORD_PTR_DS)
    PUT_DWORD(PRINT_NUMBER_ADDR)
}
