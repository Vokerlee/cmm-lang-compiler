#include "tree.h"

void construct_tree (bin_tree *tree, const char *name)
{
    assert(tree);
    assert(name);

    tree->root = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));
    tree->root->type  = 0;
    tree->root->value = 0;

    tree->root->left  = nullptr;
    tree->root->right = nullptr;

    tree->tree_size = 0;

    tree->error_state = 0;

    tree->tree_name = (char *) calloc(strlen(name) + 1, sizeof(char));
    strcpy(tree->tree_name, name);
}

void destruct_tree (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    delete_tree_elem(tree->root);

    free(tree->tree_name);

    tree->tree_name  = nullptr;
    tree->root       = nullptr;

    tree->error_state = 0;

    tree->tree_size = -1;
}

void delete_tree_elem (bin_tree_elem *element)
{
    if (element->left != nullptr)
        delete_tree_elem(element->left);

    if (element->right != nullptr)
        delete_tree_elem(element->right);

    free(element);
}

bin_tree_elem *insert_left_tree (bin_tree *tree, bin_tree_elem *element, int type, double value)
{
    ASSERT_TREE_OK

    if (tree->tree_size == 0)
    {
        tree->root->type      = type;
        tree->root->value     = value;
        tree->root->hash_tree = 0;

        tree->tree_size++;

        return tree->root;
    }

    if (element == nullptr)
    {
        tree->error_state = TREE_INVALID_INSERT_ELEMENT;
        dump_tree(tree);

        return nullptr;
    }

    if (element->left)
    {
        tree->error_state = TREE_REPEAT_INSERT_LEFT;
        dump_tree(tree);

        return nullptr;
    }

    bin_tree_elem *new_element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));
    assert(new_element);

    new_element->left  = nullptr;
    new_element->right = nullptr;

    new_element->type      = type;
    new_element->value     = value;
    new_element->hash_tree = 0;

    element->left = new_element;

    tree->tree_size++;

    ASSERT_TREE_OK

    return new_element;
}

bin_tree_elem *insert_right_tree (bin_tree *tree, bin_tree_elem *element, int type, double value)
{
    ASSERT_TREE_OK

    if (tree->tree_size == 0)
    {
        tree->root->type  = type;
        tree->root->value = value;
        tree->root->hash_tree = 0;

        tree->tree_size++;

        return tree->root;
    }

    if (element == nullptr)
    {
        tree->error_state = TREE_INVALID_INSERT_ELEMENT;
        dump_tree(tree);

        return nullptr;
    }

    if (element->left)
    {
        tree->error_state = TREE_REPEAT_INSERT_LEFT;
        dump_tree(tree);

        return nullptr;
    }

    bin_tree_elem *new_element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    new_element->left  = nullptr;
    new_element->right = nullptr;

    new_element->type      = type;
    new_element->value     = value;
    new_element->hash_tree = 0;

    element->right = new_element;

    tree->tree_size++;

    ASSERT_TREE_OK

    return new_element;
}

bin_tree_elem *create_tree_element (int type, double value, bin_tree_elem *left, bin_tree_elem *right)
{
    bin_tree_elem *element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    element->type  = type;
    element->value = value;
    L = left;
    R = right;

    return element;
}

bin_tree_elem *copy_tree_elem (bin_tree_elem *element)
{
    bin_tree_elem *copy_elem = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    copy_elem->type  = element->type;
    copy_elem->value = element->value;

    return copy_elem;
}

bin_tree_elem *copy_tree (bin_tree_elem *element)
{
    bin_tree_elem *new_elem = copy_tree_elem(element);

    if (L != nullptr)
        new_elem->left = copy_tree(L);

    if (R != nullptr)
        new_elem->right = copy_tree(R);

    return new_elem;
}

void recalc_size_tree (bin_tree_elem *element, int *size_tree)
{
    (*size_tree)++;

    if (element->left != nullptr)
        recalc_size_tree(element->left, size_tree);

    if (element->right != nullptr)
        recalc_size_tree(element->right, size_tree);
}

void dump_tree (bin_tree *tree)
{
    assert(tree);

    FILE *output   = fopen("tree_log.txt", "wb");
    FILE *graphviz = fopen("graph.dot"   , "wb");
    assert(output);
    assert(graphviz);

    int error = tree->error_state;

    if (error == 0)
        fprintf(output, "Binary tree (OK) [%p] \"%s\"\n", tree, tree->tree_name);
    else if (error == TREE_NULL_PTR || error == TREE_ROOT_NULL_PTR)
    {
        fprintf(output, "Binary tree (ERROR #%d: %s) [%p] \"%s\"\n", error, print_tree_error(error), tree, tree->tree_name);

        fclose(output);
        fclose(graphviz);

        return;
    }
    else
        fprintf(output, "Binary tree (ERROR #%d: %s) [%p] \"%s\"\n", error, print_tree_error(error), tree, tree->tree_name);

    fprintf(output, "tree-size = %d\n", tree->tree_size);

    print_tree(tree->root, output);

    fprintf(graphviz, "digraph binary_tree {\n");
    fprintf(graphviz, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graphviz, "  rankdir = \"TB\";\n\n");
    fprintf(graphviz, "  label = \"Tree Dump\";\n");

    declare_vertex_tree(tree->root, graphviz);

    link_vertex_tree(tree->root, graphviz);

    fprintf(output, "\n");

    fprintf(graphviz, "}");

    fclose(graphviz);
    fclose(output);

    system(GRAPH_BASE);
    system(START_GRAPH);
    system(START_TEXT);
}

int error_tree (bin_tree *tree)
{
    if (tree == nullptr)
    {
        tree->error_state = TREE_NULL_PTR;
        return TREE_NULL_PTR;
    }

    if (tree->root == nullptr)
    {
        tree->error_state = TREE_ROOT_NULL_PTR;
        return TREE_ROOT_NULL_PTR;
    }

    int counter = 0;

    check_size_tree(tree, tree->root, &counter);

    if (tree->tree_size != 0 && counter != tree->tree_size)
    {
        tree->error_state = TREE_INVALID_SIZE;
        return TREE_INVALID_SIZE;
    }

    return 0;
}

void check_size_tree (bin_tree *tree, bin_tree_elem *element, int *counter)
{
    if (element == nullptr)
        return;

    if (*counter > tree->tree_size)
        return;

    (*counter)++;

    if (element->left != nullptr)
        check_size_tree(tree, element->left, counter);

    if (element->right != nullptr)
        check_size_tree(tree, element->right, counter);
}

void print_tree (bin_tree_elem *element, FILE *output)
{
    if (element == nullptr)
        return;

    fprintf(output, "[%p] type = %d, value = %lg, left = [%p], right = [%p]\n", element, element->type, element->value, element->left, element->right);

    if (element->left != nullptr)
        print_tree(element->left, output);

    if (element->right != nullptr)
        print_tree(element->right, output);
}

void declare_vertex_tree (bin_tree_elem *element, FILE *output)
{
    if (element->left == nullptr && element->right == nullptr)
        fprintf(output, "  \"%p\" [shape = \"record\", fillcolor = \"red\", label = \"{value: %lg | <f0> pos\\n%p| left\\n%p | right\\n%p | type: %d\\n | size: %d | hash: %d}\"];\n", element, element->value, element, element->left, element->right, element->type, element->elem_size, element->hash_tree);
    else
        fprintf(output, "  \"%p\" [shape = \"record\", label = \"{value: %lg | <f0> pos\\n%p| left\\n%p | right\\n%p | type: %d\\n | size: %d | hash: %d}\"];\n", element, element->value, element, element->left, element->right, element->type, element->elem_size, element->hash_tree);

    if (element->left != nullptr)
        declare_vertex_tree(element->left, output);

    if (element->right != nullptr)
        declare_vertex_tree(element->right, output);
}

void link_vertex_tree (bin_tree_elem *element, FILE *output)
{
    if (element->left != nullptr)
    {
        fprintf(output, "  \"%p\"->\"%p\";\n", element, element->left);

        link_vertex_tree(element->left, output);
    }

    if (element->right != nullptr)
    {
        fprintf(output, "  \"%p\"->\"%p\";\n", element, element->right);

        link_vertex_tree(element->right, output);
    }
}

const char *print_tree_error (int error)
{
    switch (error)
    {
        case TREE_REPEAT_INSERT_LEFT:
            return "REPEAT INSERT LEFT";
        case TREE_REPEAT_INSERT_RIGHT:
            return "REPEAT INSERT RIGHT";
        case TREE_INVALID_INSERT_ELEMENT:
            return "INVALID INSERT ELEMENT";
        case TREE_NULL_PTR:
            return "TREE ROOT NULL PTR";
        case TREE_ROOT_NULL_PTR:
            return "REPEAT INSERT RIGHT";
        case TREE_INVALID_SIZE:
            return "INVALID TREE SIZE";
        default:
            return nullptr;
    }
}
