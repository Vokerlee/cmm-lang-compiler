#include "frontend_rev.h"

#define CREATE_TREE_ELEM(type, value, shift)                                                     \
    {                                                                                            \
        element = create_tree_element(type, value, nullptr, nullptr);                            \
        text->counter += shift;                                                                  \
    }


bin_tree_elem *fill_tree (text_t *text, variables *var)
{
    bin_tree_elem *element = nullptr;

    while (isspace(*(text->counter)) && *(text->counter) != '\0')
    {
        if (*(text->counter) == '\n' && text->line_counter < text->n_real_lines - 1)
            text->counter = text->lines[++(text->line_counter)].line;
        else
            text->counter++;
    }

    if (strncmp(text->counter, "nil", 3) == 0 && !isalpha(*(text->counter + 3)))
    {
        element = nullptr;
        text->counter += 4;
        return element;
    }
    else if (strncmp(text->counter, "concatenation", 13) == 0 && !isalpha(*(text->counter + 13)))
        CREATE_TREE_ELEM(BUNCH, 0, 13)
    else if (strncmp(text->counter, "+", 1) == 0)
        CREATE_TREE_ELEM(OPER, ADD, 1)
    else if (strncmp(text->counter, "-", 1) == 0 && !isdigit(*(text->counter +1)))
        CREATE_TREE_ELEM(OPER, SUB, 1)
    else if (strncmp(text->counter, "*", 1) == 0)
        CREATE_TREE_ELEM(OPER, MUL, 1)
    else if (strncmp(text->counter, "/", 1) == 0)
        CREATE_TREE_ELEM(OPER, DIV, 1)
    else if (strncmp(text->counter, "^", 1) == 0)
        CREATE_TREE_ELEM(OPER, POW, 1)
    else if (strncmp(text->counter, "$sin", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, SIN, 4)
    else if (strncmp(text->counter, "$cos", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, COS, 4)
    else if (strncmp(text->counter, "$tg", 3) == 0 && !isalpha(*(text->counter + 3)))
        CREATE_TREE_ELEM(FUNC, TG, 3)
    else if (strncmp(text->counter, "$arcsin", 7) == 0 && !isalpha(*(text->counter + 7)))
        CREATE_TREE_ELEM(FUNC, ARCSIN, 7)
    else if (strncmp(text->counter, "$arccos", 7) == 0 && !isalpha(*(text->counter + 7)))
        CREATE_TREE_ELEM(FUNC, ARCCOS, 7)
    else if (strncmp(text->counter, "$arctg", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, ARCTG, 6)
    else if (strncmp(text->counter, "$ctg", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, CTG, 4)
    else if (strncmp(text->counter, "$scan", 5) == 0 && !isalpha(*(text->counter + 5)))
        CREATE_TREE_ELEM(FUNC, SCAN, 5)
    else if (strncmp(text->counter, "$print", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, PRINT, 6)
    else if (strncmp(text->counter, "$deriv", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, DERIV, 6)
    else if (strncmp(text->counter, "$ln", 3) == 0 && !isalpha(*(text->counter + 3)))
        CREATE_TREE_ELEM(FUNC, LN, 3)
    else if (strncmp(text->counter, "$power", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, POWER, 6)
    else if (strncmp(text->counter, "if", 2) == 0 && !isalpha(*(text->counter + 2)))
        CREATE_TREE_ELEM(COMMAND, IF, 2)
    else if (strncmp(text->counter, "while", 5) == 0 && !isalpha(*(text->counter + 5)))
        CREATE_TREE_ELEM(COMMAND, WHILE, 5)
    else if (strncmp(text->counter, "!=", 2) == 0)
       CREATE_TREE_ELEM(CONDITION, JNE, 2)
    else if (strncmp(text->counter, ">=", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JAE, 2)
    else if (strncmp(text->counter, "<=", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JBE, 2)
    else if (strncmp(text->counter, ">", 1) == 0)
        CREATE_TREE_ELEM(CONDITION, JA, 1)
    else if (strncmp(text->counter, "<", 1) == 0)
        CREATE_TREE_ELEM(CONDITION, JB, 1)
    else if (strncmp(text->counter, "==", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JE, 2)
    else if (strncmp(text->counter, "=", 1) == 0)
        CREATE_TREE_ELEM(COMMAND, ASSIGN, 1)
    else if (strncmp(text->counter, "function-declaration", strlen("function-declaration")) == 0)
        CREATE_TREE_ELEM(BUNCH, 0, strlen("function-declaration"))
    else if (strncmp(text->counter, "$main", 5) == 0 && !isalpha(*(text->counter + 5)))
         CREATE_TREE_ELEM(MAIN, 0, 5)
    else if (strncmp(text->counter, "return", 6) == 0 && !isalpha(*(text->counter + 6)))
         CREATE_TREE_ELEM(RETURN, 0, 6)
    else if ((*(text->counter) == '$' && isalpha(*(text->counter + 1))) || isalpha(*(text->counter)))
    {
        int type = VAR;

        if (*(text->counter) == '$')
        {
            type = USER_FUNC;
            text->counter++;
        }

        char *temp_var_name = (char *) calloc(MAX_VAR_NAME_LENGTH + 1, sizeof(char));
        char *start = text->counter;
        int num_var = 0;

        if (isalpha(*(text->counter)))
        {
            temp_var_name[text->counter - start] = *(text->counter);
            text->counter++;

            while (isalnum(*(text->counter)) || *(text->counter) == '_')
            {
                temp_var_name[text->counter - start] = *(text->counter);
                text->counter++;
            }
        }

        int var_value = var_search(var, temp_var_name);

        element = create_tree_element(type, var_value, nullptr, nullptr);
    }
    else if (isdigit(*(text->counter)) || (*(text->counter) == '-' && isdigit(*(text->counter + 1))))
    {
        double value = atof(text->counter);

        while (isdigit(*(text->counter)) || *(text->counter) == '.' || *(text->counter) == '-')
            text->counter++;

        element = create_tree_element(NUM, value, nullptr, nullptr);
    }

    while (isspace(*(text->counter)) && *(text->counter) != '\0')                                                                                  
        text->counter++;

    if (*(text->counter) == '{')
    {
        text->counter++;
        element->left = fill_tree(text, var);
    }

    while (isspace(*(text->counter)) && *(text->counter) != '\0')                                                                                  
        text->counter++;

    text->counter++;

    while (isspace(*(text->counter)) && *(text->counter) != '\0')                                                                                  
        text->counter++;

    if (*(text->counter) == '{')
    {
        text->counter++;
        element->right = fill_tree(text, var);
    }

    while (isspace(*(text->counter)) && *(text->counter) != '\0')                                                                                  
        text->counter++;

    text->counter++;

    while (isspace(*(text->counter)) && *(text->counter) != '\0')                                                                                  
        text->counter++;

    return element;
}

int var_search (variables *var, char *temp_var_name)
{
    char *start = temp_var_name;

    for (int i = 0; i < var->curr_size; i++)
    {
        if (strcmp(temp_var_name, var->var[i]) == 0)
        {
            temp_var_name++;
            return i;
        }
    }

    temp_var_name = start;

    var->curr_size++;

    strcpy(var->var[var->curr_size - 1], temp_var_name);

    return var->curr_size - 1;
}