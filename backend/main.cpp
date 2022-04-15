#include "backend.h"

int main(int argc, char *argv[])
{
    FILE *tree     = fopen(argv[1], "rb");
    FILE *exe_file = fopen(argv[2], "wb");

    if (!tree && !exe_file)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!tree)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(exe_file);
    }
    else if (!exe_file)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(tree);
    }
    else
    {
        backend(tree, exe_file);

        fclose(tree);
        fclose(exe_file);

        printf("OK. The program has been compiled into executable file \"%s\".\n", argv[2]);
    }

    return 0;
}