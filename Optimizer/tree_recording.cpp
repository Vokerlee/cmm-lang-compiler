#include "optimizer.h"

#define PRE_ORDER_REC                                        \
    write_in_tree_file(element->left, lang_tree, var);       \
    fprintf(lang_tree, "}{");                                \
    write_in_tree_file(element->right, lang_tree, var);      \
    fprintf(lang_tree, "}");

void record_tree (bin_tree *tree, variables *var, FILE *lang_tree)
{
    assert(lang_tree);

    fprintf(lang_tree, "{");
    write_in_tree_file(tree->root, lang_tree, var);
    fprintf(lang_tree, "}");

    fclose(lang_tree);
}

void write_in_tree_file (bin_tree_elem *element, FILE *lang_tree, variables *var)
{
    if (element == nullptr)
        fprintf(lang_tree, " nil ");
    else if (element->type == BUNCH && element->left != nullptr && (element->left->type == MAIN || (element->left->type == USER_FUNC && element->left->right != nullptr)))
    {
        fprintf(lang_tree, " function-declaration {");
        PRE_ORDER_REC
    }
    else if (element->type == BUNCH)
    {
        fprintf(lang_tree, " concatenation {");
        PRE_ORDER_REC
    }
    else if (element->type == MAIN)
    {
        fprintf(lang_tree, " $main {");
        PRE_ORDER_REC
    }
    else if (element->type == NUM)
    {
        fprintf(lang_tree, " %lg {", element->value);
        PRE_ORDER_REC
    }
    else if (element->type == OPER)
    {
        fprintf(lang_tree, " %c {", (int) element->value);
        PRE_ORDER_REC
    }
    else if (element->type == COMMAND && (int) element->value == IF)
    {
        fprintf(lang_tree, " if {");
        PRE_ORDER_REC
    }
    else if (element->type == COMMAND && (int) element->value == WHILE)
    {
        fprintf(lang_tree, " while {");
        PRE_ORDER_REC
    }
    else if (element->type == COMMAND && (int) element->value == ASSIGN)
    {
        fprintf(lang_tree, " = {");
        PRE_ORDER_REC
    }
    else if (element->type == RETURN)
    {
        fprintf(lang_tree, " return {");
        PRE_ORDER_REC
    }
    else if (element->type == FUNC && (int) element->value == SCAN)
    {
        fprintf(lang_tree, " $scan {");
        PRE_ORDER_REC
    }
    else if (element->type == FUNC && (int) element->value == PRINT)
    {
        fprintf(lang_tree, " $print {");
        PRE_ORDER_REC
    }
    else if (element->type == USER_FUNC || element->type == FUNC)
    {
        fprintf(lang_tree, " $%s {", var->var[(int) element->value]);
        PRE_ORDER_REC
    }
    else if (element->type == VAR || element->type == GLOB_VAR)
    {
        fprintf(lang_tree, " %s {", var->var[(int) element->value]);
        PRE_ORDER_REC
    }
    else if (element->type == CONDITION)
    {
        fprintf(lang_tree, " %s {", condition_name(element->value));
        PRE_ORDER_REC
    }
}

const char *func_name (int value)
{
    switch (value)
    {
        case SIN:
            return "$sin";
        case COS:
            return "$cos";
        case DERIV:
            return "$deriv";
        case POWER:
            return "$power";
        case TG:
            return "$tg";
        case ARCSIN:
            return "$arcsin";
        case ARCCOS:
            return "$arccos";
        case ARCTG:
            return "$arctg";
        case ARCCTG:
            return "$sin";
        case LN:
            return "$ln";
    }
}

const char *condition_name (int value)
{
    switch (value)
    {
        case JE:
            return "==";
        case JNE:
            return "!=";
        case JA:
            return ">";
        case JAE:
            return ">=";
        case JB:
            return "<";
        case JBE:
            return "<=";
    }
}