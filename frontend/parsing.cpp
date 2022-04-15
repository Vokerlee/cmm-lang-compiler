#include "frontend.h"

int ERROR_STATE = 0;

#define SKIP_SPACES                                                                             \
    while (isspace(*(text.counter)) && *(text.counter) != '\0')                                 \
    {                                                                                           \
        if (*(text.counter) == '\n' && text.line_counter < text.n_real_lines - 1)               \
            text.counter = text.lines[++(text.line_counter)].line;                              \
        else                                                                                    \
            text.counter++;                                                                     \
    }

#define MULTIPLY(left, right)                                                                   \
    create_tree_element(OPER, MUL, left, right)

#define DIVIDE(left, right)                                                                     \
    create_tree_element(OPER, DIV, left, right)

#define ADDITION(left, right)                                                                   \
    create_tree_element(OPER, ADD, left, right)

#define SUBTRACT(left, right)                                                                   \
    create_tree_element(OPER, SUB, left, right)

#define CR_NUM(value)                                                                           \
    create_tree_element(NUM, value, nullptr, nullptr)

#define CR_VAR(value)                                                                           \
    create_tree_element(VAR, value, nullptr, nullptr)


int frontend (FILE *program, FILE *lang_tree)
{
    if (program == NULL || lang_tree == NULL)
        return -1;

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    variables var = {};
    elements elem = {};

    elem.elements_ = (bin_tree_elem **) calloc(MAX_PROJECT_TREE_ELEM, sizeof(bin_tree_elem *));

    fill_tree(&tree, program, &var, &elem);

    free(elem.elements_);

    if (tree.root == nullptr)
    {
        destruct_tree(&tree);
        return -1;
    }

    record_tree(&tree, &var, lang_tree);

    destruct_tree(&tree);

    return 0;
}

void fill_tree (bin_tree *tree, FILE *program, variables *var, elements *elem)
{
    ASSERT_TREE_OK_VOID

    text_t text = {};
    construct_text(&text);

    fill_text(program, &text, NO_COMMENTS);

    text.counter = text.lines[0].line;

    double value = 0;

    while (*(text.counter) != '\0')
    {
        if (isdigit(*(text.counter)))
        {
            value = atof(text.counter);

            while (isdigit(*(text.counter)) || *(text.counter) == '.' || *(text.counter) == '-')
                text.counter++;

            elem->elements_[elem->curr_size_++] = create_tree_element(NUM, value, nullptr, nullptr);
        }
        else if (isalpha(*(text.counter)))
        {
            char temp_var_name[MAX_VAR_NAME_LENGTH + 1] = {0};
            char *start = text.counter;
            int num_var = 0;

            if (isalpha(*(text.counter)))
            {
                temp_var_name[text.counter - start] = *(text.counter);
                text.counter++;

                while (isalnum(*(text.counter)) || *(text.counter) == '_')
                {
                    temp_var_name[text.counter - start] = *(text.counter);
                    text.counter++;
                }
            }

            int type = 0;
            int op_value = is_keyword(temp_var_name, &type);

            if (op_value == -1)
            {
                num_var = var_search(var, temp_var_name);
                elem->elements_[elem->curr_size_++] = create_tree_element(VAR, num_var, nullptr, nullptr);
            }
            else
            {
                switch (type)
                {
                    case FUNC:
                    {
                        elem->elements_[elem->curr_size_++] = create_tree_element(FUNC, op_value, nullptr, nullptr);
                        break;
                    }
                    case MAIN:
                    {
                        elem->elements_[elem->curr_size_++] = create_tree_element(MAIN, 0, nullptr, nullptr);
                        break;
                    }
                    case COMMAND:
                    {
                        elem->elements_[elem->curr_size_++] = create_tree_element(COMMAND, op_value, nullptr, nullptr);
                        break;
                    }
                    case RETURN:
                    {
                        elem->elements_[elem->curr_size_++] = create_tree_element(RETURN, 0, nullptr, nullptr);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        else if (*(text.counter) == '+')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(OPER, ADD, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '-')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(OPER, SUB, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '*')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(OPER, MUL, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '/')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(OPER, DIV, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '^')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(OPER, POW, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '(')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(BRACKET, OPEN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == ')')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(BRACKET, CLOSE, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '{')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(FIG_BRACKET, OPEN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '}')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(FIG_BRACKET, CLOSE, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == ';')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(BUNCH, 0, nullptr, nullptr);
            text.counter++;
        }
        else if (strncmp(text.counter, "==", 2) == 0)
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, "!=", 2) == 0)
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JNE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, "<=", 2) == 0)
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JBE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, ">=", 2) == 0)
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JAE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (*(text.counter) == '<')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JB, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '>')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(CONDITION, JA, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '=')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(COMMAND, ASSIGN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == ',')
        {
            elem->elements_[elem->curr_size_++] = create_tree_element(ENUM, 0, nullptr, nullptr);
            text.counter++;
        }

        SKIP_SPACES
    }

    int counter = 0;

    free(tree->root);
    tree->root = create_prog_tree(elem, &counter);

    destruct_text(&text);

    int size_tree = 0;
    recalc_size_tree(tree->root, &size_tree);
    tree->tree_size = size_tree;

    for (size_t i = 0; i < elem->curr_size_; i++)
    {
        if (elem->elements_[i]->type == BRACKET || elem->elements_[i]->type == FIG_BRACKET)
            free(elem->elements_[i]);
    }

    ASSERT_TREE_OK_VOID
}

int is_keyword (char *temp_var_name, int *type)
{
    *type = FUNC;

    if (strncmp(temp_var_name, "sin", 3) == 0)
        return SIN;
    if (strncmp(temp_var_name, "cos", 3) == 0)
        return COS;
    else if (strncmp(temp_var_name, "tg", 2) == 0)
        return TG;
    else if (strncmp(temp_var_name, "ctg", 3) == 0)
        return CTG;
    else if (strncmp(temp_var_name, "arcsin", 6) == 0)
        return ARCSIN;
    else if (strncmp(temp_var_name, "arccos", 6) == 0)
        return ARCCOS;
    else if (strncmp(temp_var_name, "arctg", 5) == 0)
        return ARCTG;
    else if (strncmp(temp_var_name, "arcctg", 6) == 0)
        return ARCCTG;
    else if (strncmp(temp_var_name, "exp", 3) == 0)
        return EXP;
    else if (strncmp(temp_var_name, "sh", 2) == 0)
        return SH;
    else if (strncmp(temp_var_name, "ch", 2) == 0)
        return CH;
    else if (strncmp(temp_var_name, "th", 2) == 0)
        return TH;
    else if (strncmp(temp_var_name, "cth", 3) == 0)
        return CTH;
    else if (strncmp(temp_var_name, "ln", 2) == 0)
        return LN;
    else if (strncmp(temp_var_name, "deriv", 5) == 0)
        return DERIV;
    else if (strncmp(temp_var_name, "scan", 4) == 0)
        return SCAN;
    else if (strncmp(temp_var_name, "print", 5) == 0)
        return PRINT;
    else if (strncmp(temp_var_name, "power", 5) == 0)
        return POWER;

    *type = MAIN;

    if (strncmp(temp_var_name, "main", 4) == 0)
        return MAIN;

    *type = COMMAND;

    if (strncmp(temp_var_name, "if", 2) == 0)
        return IF;

    if (strncmp(temp_var_name, "while", 5) == 0)
        return WHILE;

    *type = RETURN;

    if (strncmp(temp_var_name, "return", 6) == 0)
        return RETURN;

    return -1;

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

bin_tree_elem *create_prog_tree (elements *elem, int *counter)
{
    bin_tree_elem *bunch  = nullptr;
    bin_tree_elem *vertex = elem->elements_[*counter];
    (*counter)++;

    if (vertex->type == MAIN)
        vertex = create_main_tree(elem, counter);
    else if (vertex->type == VAR && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
        vertex = create_user_func_tree(elem, counter);
    else if (vertex->type == VAR && elem->elements_[*counter]->type == COMMAND && (int) elem->elements_[*counter]->value == ASSIGN)
    {
        vertex        = elem->elements_[*counter];
        vertex->left  = elem->elements_[*counter - 1];
        (*counter)++;
        vertex->right = create_e_tree(elem, counter);
        (*counter)++;
        vertex->left->type = GLOB_VAR;
    }

    bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->left = vertex;

    if (elem->curr_size_ > *counter)
        bunch->right = create_prog_tree(elem, counter);

    return bunch;
}

bin_tree_elem *create_user_func_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements_[*counter - 1];

    if (vertex->type == VAR && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
    {
        (*counter)++;
        vertex->left = create_var_func_tree(elem, counter);

        bin_tree_elem *bracket = elem->elements_[*counter];
        (*counter)++;

        bracket = elem->elements_[*counter];
        (*counter)++;

        if (bracket->type == FIG_BRACKET && (int) bracket->value == OPEN)
        {
            vertex->right = create_body_tree(elem, counter);

            bracket = elem->elements_[*counter];
            (*counter)++;
        }
    }

    vertex->type = USER_FUNC;

    return vertex;
}

bin_tree_elem *create_var_func_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements_[*counter];

    if (vertex->type == BRACKET)
        return nullptr;

    (*counter)++;

    if (vertex->type == VAR && elem->elements_[*counter]->type == ENUM)
    {
        (*counter)++;
        vertex->left = create_var_func_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *create_param_func_tree (elements *elem, int *counter)
{
    if (elem->elements_[*counter]->type == BRACKET)
        return nullptr;

    bin_tree_elem *vertex = create_e_tree(elem, counter);

    bin_tree_elem *bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->right = vertex;

    if (elem->elements_[*counter]->type == ENUM)
    {
        (*counter)++;
        bunch->left = create_param_func_tree(elem, counter);
    }

    return bunch;
}

bin_tree_elem *create_main_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements_[*counter - 1];

    if (vertex->type == MAIN)
    {
        bin_tree_elem *bracket = elem->elements_[*counter];
        (*counter)++;

        if (bracket->type == FIG_BRACKET && (int) bracket->value == OPEN)
        {
            vertex->right = create_body_tree(elem, counter);

            bracket = elem->elements_[*counter];
            (*counter)++;
        }
    }

    return vertex;
}

bin_tree_elem *create_body_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = create_cmd_tree(elem, counter);
    bin_tree_elem *bunch  = nullptr;

    if ((vertex->type == FUNC && ((int) vertex->value == SCAN || (int) vertex->value == PRINT)) || vertex->type == RETURN || vertex->type == USER_FUNC || (vertex->type == COMMAND && ((int) vertex->value != IF && (int) vertex->value != WHILE)))
    {
        bunch = elem->elements_[*counter];
        (*counter)++;
    }
    else
        bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->left = vertex;

    if (elem->elements_[*counter]->type != FIG_BRACKET)
        bunch->right = create_body_tree(elem, counter);

    return bunch;
}

bin_tree_elem *create_cmd_tree (elements *elem, int *counter)
{
    bin_tree_elem *command = nullptr;

    if (elem->elements_[*counter]->type == COMMAND && ((int) elem->elements_[*counter]->value == IF || (int) elem->elements_[*counter]->value == WHILE))
    {
        command = elem->elements_[*counter];
        (*counter)++;

        (*counter)++;

        bin_tree_elem *condition_left = create_e_tree(elem, counter);

        bin_tree_elem *condition = elem->elements_[*counter];
        (*counter)++;

        bin_tree_elem *condition_right = create_e_tree(elem, counter);
        (*counter)++;

        condition->left  = condition_left;
        condition->right = condition_right;
        command->left = condition;

        (*counter)++;
        command->right = create_body_tree(elem, counter);
        (*counter)++;

    }
    else if (elem->elements_[*counter]->type == VAR && elem->elements_[*counter + 1]->type == BRACKET && (int) elem->elements_[*counter + 1]->value == OPEN)
    {
        command = elem->elements_[*counter];
        command->type = USER_FUNC;
        *counter += 2;

        command->left = create_param_func_tree(elem, counter);

        (*counter)++;
    }
    else if (elem->elements_[*counter]->type == RETURN)
    {
        command = elem->elements_[*counter];
        (*counter)++;

        if (elem->elements_[*counter]->type != BUNCH)
            command->left = create_e_tree(elem, counter);
    }
    else if (elem->elements_[*counter]->type == FUNC && (int) elem->elements_[*counter]->value == SCAN)
    {
        command = elem->elements_[*counter];
        *counter += 2;

        command->left = elem->elements_[*counter];
        *counter += 2;
    }
    else if (elem->elements_[*counter]->type == FUNC && (int) elem->elements_[*counter]->value == PRINT)
    {
        command = elem->elements_[*counter];
        *counter += 2;
        
        command->left = create_e_tree(elem, counter);
        (*counter)++;
    }
    else
    {
        bin_tree_elem *expression1 = create_e_tree(elem, counter);

        command = elem->elements_[*counter];
        (*counter)++;

        bin_tree_elem *expression2 = create_e_tree(elem, counter);

        command->left  = expression1;
        command->right = expression2;
    }

    return command;
}

bin_tree_elem *create_e_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex      = nullptr;
    bin_tree_elem *prev_vertex = nullptr;

    int sign = 0;

    if (elem->elements_[*counter]->type == OPER && (int) elem->elements_[*counter]->value == SUB)
    {
        sign = -1;
        (*counter)++;
    }

    vertex = create_t_tree(elem, counter);

    if (sign == -1)
        vertex = MULTIPLY(CR_NUM(-1), vertex);

    if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && ((int) elem->elements_[*counter]->value == SUB || (int) elem->elements_[*counter]->value == ADD))
    {
        while (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && ((int) elem->elements_[*counter]->value == SUB || (int) elem->elements_[*counter]->value == ADD))
        {
            prev_vertex = vertex;

            vertex = elem->elements_[*counter];

            vertex->left = prev_vertex;

            (*counter)++;

            vertex->right = create_t_tree(elem, counter);
        }
    }

    return vertex;
}

bin_tree_elem *create_t_tree (elements *elem, int *counter)
{
    bin_tree_elem *prev_vertex = nullptr;
    bin_tree_elem *vertex      = create_w_tree(elem, counter);

    if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && ((int) elem->elements_[*counter]->value == MUL || (int) elem->elements_[*counter]->value == DIV))
    {
        while (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && ((int) elem->elements_[*counter]->value == MUL || (int) elem->elements_[*counter]->value == DIV))
        {
            prev_vertex = vertex;

            vertex = elem->elements_[*counter];

            vertex->left = prev_vertex;

            (*counter)++;

            vertex->right = create_w_tree(elem, counter);
        }
    }

    return vertex;
}

bin_tree_elem *create_w_tree (elements *elem, int *counter)
{
    bin_tree_elem *prev_vertex = nullptr;
    bin_tree_elem *vertex      = create_p_tree(elem, counter);

    if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && (int) elem->elements_[*counter]->value == POW)
    {
        prev_vertex = vertex;

        vertex = elem->elements_[*counter];

        vertex->left = prev_vertex;

        (*counter)++;

        vertex->right = read_w_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *read_w_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = create_p_tree(elem, counter);

    if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == OPER && (int) elem->elements_[*counter]->value == POW)
    {
        bin_tree_elem *new_vertex = vertex;

        vertex = elem->elements_[*counter];

        (*counter)++;

        vertex->left  = new_vertex;
        vertex->right = read_w_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *create_p_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = nullptr;

    if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
    {
        (*counter)++;

        vertex = create_e_tree(elem, counter);

        if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == CLOSE)
            (*counter)++;
    }
    else
        vertex = create_n_tree(elem, counter);

    return vertex;
}

bin_tree_elem *create_n_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = nullptr;

    if (elem->elements_[*counter]->type == NUM)
    {
        vertex = elem->elements_[*counter];
        (*counter)++;
    }
    else if (elem->elements_[*counter]->type == VAR && elem->elements_[*counter + 1]->type == BRACKET && (int) elem->elements_[*counter + 1]->value == OPEN)
    {
        vertex = elem->elements_[*counter];
        vertex->type = USER_FUNC;

        *counter += 2;

        vertex->left = create_param_func_tree(elem, counter);
        
        (*counter)++;

    }
    else if (elem->elements_[*counter]->type == VAR)
    {
        vertex = elem->elements_[*counter];
        (*counter)++;
    }
    else if (elem->elements_[*counter]->type == FUNC && (int) elem->elements_[*counter]->value != DERIV && (int) elem->elements_[*counter]->value != POWER)
    {
        bin_tree_elem *func_elem = elem->elements_[*counter];
        (*counter)++;

        if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = create_e_tree(elem, counter);

            if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left = vertex;
        vertex = func_elem;
    }
    else if (elem->elements_[*counter]->type == FUNC && (int) elem->elements_[*counter]->value == DERIV)
    {
        bin_tree_elem *func_elem = elem->elements_[*counter];
        bin_tree_elem *formula = nullptr;
        (*counter)++;

        if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = elem->elements_[*counter];

            *counter += 2;

            formula = create_e_tree(elem, counter);

            if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left  = vertex;
        func_elem->right = formula;
        vertex = func_elem;
    }
    else if (elem->elements_[*counter]->type == FUNC && (int) elem->elements_[*counter]->value == POWER)
    {
        bin_tree_elem *func_elem = elem->elements_[*counter];
        bin_tree_elem *power = nullptr;
        (*counter)++;

        if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = create_e_tree(elem, counter);

            (*counter)++;

            power = create_e_tree(elem, counter);

            if (*counter < elem->curr_size_ && elem->elements_[*counter]->type == BRACKET && (int) elem->elements_[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left  = vertex;
        func_elem->right = power;
        vertex = func_elem;
    }

    return vertex;
}

void syntax_error(text_t *text, elements *elem, int line, const char *file)
{
    assert(elem);
    fprintf(stderr, "Syntax error [%d line in cpp] (%s).\n"
           "Process of file-reading terminated. Please, use the right syntax.\n", line, file);
    fprintf(stderr, "You have an error in line %ld in file: ", (long int) text->lines[text->line_counter].real_num_line + 1);

    switch(ERROR_STATE)
    {
        case BRAKETS_ERR:
        {
            printf("the bracket was missed here: ");
            break;
        }
        case UNKNOWN_DESIGN:
        {
            printf("you have entered unknown command: ");
            break;
        }
        case END_OF_LINE_ERR:
        {
            printf("there is no end of line here: ");
            break;
        }
        default:
        {
            printf("the error is not recognized: ");
            break;
        }
    }

    printf("...");

    for (int i = 0; i < MAX_ERROR_TEXT_LENGTH && *(text->counter) != '\0'; text->counter++, i++)
        printf("%c", *(text->counter));

    printf("...\n\n");

    for (int i = 0; i < elem->curr_size_; i++)
        free(elem->elements_[i]);
}
