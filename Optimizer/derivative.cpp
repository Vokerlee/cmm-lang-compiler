#include "optimizer.h"

#define dR derivate_tree(element->right, var_value)
#define dL derivate_tree(element->left, var_value)

bin_tree_elem *derivate_tree (bin_tree_elem *element, int var_value)
{
    switch (element->type)
    {
        case NUM:
            return CR_NUM(0);
        case VAR:
        {
            if ((int) element->value == var_value)
                return CR_NUM(1);
            else
                return CR_NUM(0);
        }
        case OPER:
        {
            switch ((int) element->value)
            {
                case ADD:
                    return ADDITION(dL, dR);
                case SUB:
                    return SUBTRACT(dL, dR);
                case MUL:
                    return ADDITION(MULTIPLY(dL, cR), MULTIPLY(cL, dR));
                case DIV:
                    return DIVIDE(SUBTRACT(MULTIPLY(dL, cR), MULTIPLY(cL, dR)), POWER_NUM(cR, 2));
                case POW:
                {
                    if ((L->type == NUM || (L->type == VAR && L->value != var_value)) && (R->type == NUM || (R->type == VAR && R->value != var_value)))
                        return CR_NUM(0);
                    else if ((L->type == NUM || (L->type == VAR && L->value != var_value)) && R->type == VAR && R->value == var_value)
                        return MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(L->value), nullptr));
                    else if ((L->type == NUM || (L->type == VAR && L->value != var_value)) && (R->type == FUNC || R->type == OPER))
                        return MULTIPLY(MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(L->value), nullptr)), dR);
                    else if (L->type == VAR && L->value == var_value && R->type == NUM)
                        return MULTIPLY(CR_NUM(R->value), POWER_NUM(CR_VAR(L->value), R->value - 1));
                    else if (L->type == VAR && L->value == 0 && R->type == VAR && R->value != 0)
                        return MULTIPLY(cR, POWER(CR_VAR(L->value), SUBTRACT(cR, CR_NUM(1))));
                    else if (L->type == FUNC && R->type == NUM)
                        return MULTIPLY(MULTIPLY(CR_NUM(R->value), POWER_NUM(cL, R->value - 1)), dL);
                    else if (is_elem_func(L) != 0 && is_elem_func(R) != 0)
                        return MULTIPLY(copy_tree(element), derivate_tree(MULTIPLY(cR, create_tree_element(FUNC, LN, cL, nullptr)), var_value));
                }
                default:
                    break;
            }
        }
        case FUNC:
        {
            switch ((int) element->value)
            {
                case SIN:
                    return MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), dL);
                case COS:
                    return MULTIPLY(MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(-1)), dL);
                case TG:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(2))), dL);
                case CTG:
                    return MULTIPLY(CR_NUM(-1), MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SIN, cL, nullptr), CR_NUM(2))), dL));
                case ARCSIN:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL);
                case ARCCOS:
                    return MULTIPLY(CR_NUM(-1), MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL));
                case ARCTG:
                    return DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2))));
                case ARCCTG:
                    return MULTIPLY(CR_NUM(-1), DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2)))));
                case SH:
                    return MULTIPLY(create_tree_element(FUNC, CH, cL, nullptr), dL);
                case CH:
                    return MULTIPLY(create_tree_element(FUNC, SH, cL, nullptr), dL);
                case TH:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, CH, cL, nullptr), CR_NUM(2))), dL);
                case CTH:
                    return MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SH, cL, nullptr), CR_NUM(2))), dL);
                case LN:
                    return MULTIPLY(DIVIDE(CR_NUM(1), cL), dL);
                default:
                    break;
            }
        }
        case USER_FUNC:
            return CR_NUM(1);
        default:
            break;
    }
}

void print_func_text (int value, FILE *tex)
{
    assert(tex);

    switch(value)
    {
        case SIN:
        {
            fprintf(tex, "sin");
            break;
        }
        case COS:
        {
            fprintf(tex, "cos");
            break;
        }
        case TG:
        {
            fprintf(tex, "tg");
            break;
        }
        case CTG:
        {
            fprintf(tex, "ctg");
            break;
        }
        case ARCSIN:
        {
            fprintf(tex, "arcsin");
            break;
        }
        case ARCCOS:
        {
            fprintf(tex, "arccos");
            break;
        }
        case ARCTG:
        {
            fprintf(tex, "arctg");
            break;
        }
        case ARCCTG:
        {
            fprintf(tex, "arcctg");
            break;
        }
        case SH:
        {
            fprintf(tex, "sh");
            break;
        }
        case CH:
        {
            fprintf(tex, "ch");
            break;
        }
        case TH:
        {
            fprintf(tex, "th");
            break;
        }
        case CTH:
        {
            fprintf(tex, "cth");
            break;
        }
        case LN :
        {
            fprintf(tex, "ln");
            break;
        }
        default:
            return;
    }
}

int is_elem_func (bin_tree_elem *element)
{
    int state1 = 0;
    int state2 = 0;

    if (element->type == VAR)
        return 1;

    if (L != nullptr)
        state1 = is_elem_func(L);

    if (R != nullptr)
        state2 = is_elem_func(R);

    return state1 + state2;
}
