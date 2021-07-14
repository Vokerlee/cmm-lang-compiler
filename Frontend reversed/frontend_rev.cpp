#include "frontend_rev.h"

#define SKIP_SPACES                                                                             \
    while (isspace(*(text->counter)) && *(text->counter) != '\0')                               \
    {                                                                                           \
        if (*(text->counter) == '\n' && text->line_counter < text->n_real_lines - 1)            \
            text->counter = text->lines[++(text->line_counter)].line;                           \
        else                                                                                    \
            text->counter++;                                                                    \
    }

#define CREATE_TREE_ELEM(type, value, shift)                                                     \
    {                                                                                            \
        element = create_tree_element(type, value, nullptr, nullptr);                            \
        text->counter += shift;                                                                  \
    }


void frontend_rev (FILE *tree_lang, FILE *lang_code)
{
    assert(tree_lang);
    assert(lang_code);

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

    lang_transfer(tree.root, lang_code, &var);

    destruct_text(&text);
    destruct_tree(&tree);
}

void lang_transfer (bin_tree_elem *element, FILE *lang, variables *var)
{
    assert(element);
    assert(lang);
    assert(var);

    int n_tab = 0;

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == COMMAND && bunch->left->value == ASSIGN)
        {
            fprintf(lang, "%s = ", var->var[(int) bunch->left->left->value]);
            print_expression(bunch->left->right, lang, var);
            fprintf(lang, ";\n");
        }
    }

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == MAIN)
        {
            fprintf(lang, "\nmain\n{\n");
            n_tab++;
            print_body(bunch->left->right, lang, var, &n_tab);
            n_tab--;
            fprintf(lang, "}\n");
        }
    }

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == USER_FUNC)
        {
            fprintf(lang, "\n%s(", var->var[(int) bunch->left->value]);

            for (bin_tree_elem *temp_bunch = bunch->left->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
            {
                fprintf(lang, "%s", var->var[(int) temp_bunch->value]);

                if (temp_bunch->left != nullptr)
                    fprintf(lang, ", ");
            }

            fprintf(lang, ")\n{\n");
            n_tab++;
            print_body(bunch->left->right, lang, var, &n_tab);
            n_tab--;
            fprintf(lang, "}\n\n");
        }
    }
}

void print_body (bin_tree_elem *element, FILE *lang, variables *var, int *n_tab)
{
    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == COMMAND && (int) bunch->left->value == ASSIGN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "%s = ", var->var[(int) bunch->left->left->value]);
            print_expression(bunch->left->right, lang, var);
            fprintf(lang, ";\n");
        }

        if (bunch->left->type == COMMAND && ((int) bunch->left->value == IF || (int) bunch->left->value == WHILE))
        {
            print_tab(n_tab, lang);

            if (bunch->left->value == IF)
                fprintf(lang, "if (");
            else
                fprintf(lang, "while (");

            print_expression(bunch->left->left->left, lang, var);

            switch ((int) bunch->left->left->value)
            {
                case JE:
                {
                    fprintf(lang, " == ");
                    break;
                }
                case JNE:
                {
                    fprintf(lang, " != ");
                    break;
                }
                case JA:
                {
                    fprintf(lang, " > ");
                    break;
                }
                case JAE:
                {
                    fprintf(lang, " >= ");
                    break;
                }
                case JB:
                {
                    fprintf(lang, " < ");
                    break;
                }
                case JBE:
                {
                    fprintf(lang, " <= ");
                    break;
                }
            }

            print_expression(bunch->left->left->right, lang, var);

            fprintf(lang, ")\n");
            print_tab(n_tab, lang);
            fprintf(lang, "{\n");

            (*n_tab)++;
            print_body(bunch->left->right, lang, var, n_tab);
            (*n_tab)--;

            print_tab(n_tab, lang);
            fprintf(lang, "}\n\n");
        }

        if (bunch->left->type == FUNC && (int) bunch->left->value == PRINT)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "print(");
            print_expression(bunch->left->left, lang, var);
            fprintf(lang, ");\n");
        }

        if (bunch->left->type == FUNC && (int) bunch->left->value == SCAN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "scan(%s);\n", var->var[(int) bunch->left->left->value]);
        }

        if (bunch->left->type == RETURN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "return");

            if (bunch->left->left != nullptr)
            {
                fprintf(lang, " ");
                print_expression(bunch->left->left, lang, var);
            }

            fprintf(lang, ";\n");
        }

        if (bunch->left->type == USER_FUNC)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "%s(", var->var[(int) bunch->left->value]);

            for (bin_tree_elem *temp_bunch = bunch->left->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
            {
                print_expression(temp_bunch->right, lang, var);

                if (temp_bunch->left != nullptr)
                {
                    fprintf(lang, ", ");
                }
            }

            fprintf(lang, ");\n");
        }

    }
}

void print_expression (bin_tree_elem *element, FILE *lang, variables *var)
{
    if (element->type == NUM)
        fprintf(lang, "%lg", element->value);
    else if (element->type == VAR)
        fprintf(lang, "%s", var->var[(int) element->value]);
    else if (element->type == USER_FUNC)
    {
        fprintf(lang, "%s(", var->var[(int) element->value]);

        for (bin_tree_elem *temp_bunch = element->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
        {
            print_expression(temp_bunch->right, lang, var);

            if (temp_bunch->left != nullptr)
            {
                fprintf(lang, ", ");
            }
        }

        fprintf(lang, ")");
    }
    else if (element->type == FUNC)
    {
        switch((int) element->value)
        {
            case SIN:
            {
                fprintf(lang, "sin(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case COS:
            {
                fprintf(lang, "cos(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCSIN:
            {
                fprintf(lang, "arcsin(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCCOS:
            {
                fprintf(lang, "arccos(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case TG:
            {
                fprintf(lang, "tg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCTG:
            {
                fprintf(lang, "arctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCCTG:
            {
                fprintf(lang, "arcctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case CTG:
            {
                fprintf(lang, "ctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case LN:
            {
                fprintf(lang, "ln(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case DERIV:
            {
                fprintf(lang, "deriv(%s, ", var->var[(int) element->left->value]);
                print_expression(element->right, lang, var);
                fprintf(lang, ")");
                break;
            }
            case POWER:
            {
                fprintf(lang, "power(");
                print_expression(element->left, lang, var);
                fprintf(lang, ", ");
                print_expression(element->right, lang, var);
                fprintf(lang, ")");
                break;
            }
        }
    }
    else if (element->type == OPER)
    {
        switch ((int) element->value)
        {
            case ADD:
            {
                print_expression(element->left, lang, var);
                fprintf(lang, " + ");
                print_expression(element->right, lang, var);
                break;
            }
            case SUB:
            {
                print_expression(element->left, lang, var);
                fprintf(lang, " - ");
                print_expression(element->right, lang, var);
                break;
            }
            case MUL:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, " * ");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
            }
            case DIV:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, " / ");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
                break;
            }
            case POW:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, "^");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
            }
        }
    }
}

void print_tab (int *n_tab, FILE *lang)
{
    for (int i = 0; i < *n_tab; i++)
        fprintf(lang, "    ");
}