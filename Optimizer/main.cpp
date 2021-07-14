#include "optimizer.h"

int main(int argc, char *argv[])
{
    FILE *tree           = fopen(argv[1], "rb");
    FILE *optimized_tree = fopen(argv[2], "wb");

    if (!tree && !optimized_tree)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!tree)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(optimized_tree);
    }
    else if (!optimized_tree)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(tree);
    }
    else
    {
        optimizer(tree, optimized_tree);

        fclose(tree);
        fclose(optimized_tree);

        printf("OK. The tree has been optimized.\n");
    }

    return 0;
}
