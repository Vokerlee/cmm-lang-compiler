#include "frontend_rev.h"

int main(int argc, char *argv[])
{
    FILE *tree      = fopen(argv[1], "rb");
    FILE *lang_code = fopen(argv[2], "wb");

    if (!tree && !lang_code)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!tree)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(lang_code);
    }
    else if (!lang_code)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(tree);
    }
    else
    {
        frontend_rev(tree, lang_code);

        fclose(tree);
        fclose(lang_code);

        printf("OK. The tree has been converted into the program.\n");
    }

    return 0;
}