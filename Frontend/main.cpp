#include "frontend.h"

int main(int argc, char *argv[])
{
    FILE *program   = fopen(argv[1], "rb");
    FILE *lang_tree = fopen(argv[2], "wb");
    
    if (!program && !lang_tree)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!program)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(lang_tree);
    }
    else if (!lang_tree)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(program);
    }
    else
    {
        frontend(program, lang_tree);

        fclose(program);
        fclose(lang_tree);

        printf("OK. The program has been converted into tree-file.\n");
    }

    return 0;
}
