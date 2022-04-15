#ifndef IMPORT_TABLE_H_INCLUDED
#define IMPORT_TABLE_H_INCLUDED

#include "exe_config.h"

struct import_name
{
    WORD Hint;
    const CHAR* Name;
    size_t size;

    import_name(const char name[], size_t size, WORD hnt = 0) :
        Hint(hnt),
        Name(name),
        size(size)
    {}

    void fill(const char name[], size_t size, WORD hnt = 0)
    {
        Name = name;
        this->size = size;
        Hint = hnt;
    }

    size_t put_in_file(FILE* file)
    {
        if (file == nullptr)
            return 0;

        fwrite(&Hint, sizeof(WORD), 1, file);
        fwrite(Name, sizeof(char), size, file);

        char zero = 0;
        fwrite(&zero, sizeof(zero), 1, file); // feature of the end

        return size;
    }
};

class import_tabel
{
private:
    size_t size;
    IMAGE_IMPORT_DESCRIPTOR* table;
    import_name* name_imp;
    size_t names_size;
    IMAGE_THUNK_DATA64* thunk;
    size_t align_size;

public:
    import_tabel(size_t sz = 3) :
        size(sz + 1),
        name_imp(nullptr),
        names_size(0),
        align_size(0)
    {
        table    = (IMAGE_IMPORT_DESCRIPTOR*) calloc(sz + 1, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        name_imp = (import_name*) calloc(sz, sizeof(import_name));
        thunk    = (IMAGE_THUNK_DATA64*) calloc(sz, sizeof(thunk[0]));
    }

    void fill_def_names(void)
    {
        name_imp[0].fill("ExitProgram", 11);
        name_imp[1].fill("GetNumber", 9);
        name_imp[2].fill("PrintNumber", 11);

        for (size_t i = 0; i < size - 1; ++i)
            names_size += name_imp[i].size + 3;
    }

    bool fill_default(void)
    {
        fill_def_names();

        size_t tbl_size        = sizeof(table[1]) * size;
        size_t thunks_size     = sizeof(thunk[1]) * (size - 1);
        size_t names_start     = IMPORT_START + tbl_size + thunks_size;
        size_t fst_thunk_start = names_start + names_size;
        size_t name_rva        = fst_thunk_start + thunks_size;
        size_t cur_name_size   = 0;

        for (size_t i = 0; i < size - 1; ++i)
        {
            table[i].OriginalFirstThunk = IMPORT_START + tbl_size + i * sizeof(thunk[1]);
            table[i].FirstThunk = fst_thunk_start + i * sizeof(thunk[1]);
            table[i].Name = name_rva;
            thunk[i].u1.AddressOfData = names_start + cur_name_size;
            cur_name_size += name_imp[i].size + 3;
        }

        return true;
    }

    bool put_in_file(FILE* file) // default
    {
        if (file == nullptr)
            return false;

        const size_t dll_name_size = 13;
        const size_t stub_size = 1 + SIZE_RAW - (sizeof(table[0]) * size + 2 * sizeof(thunk[1]) * (size - 1) + names_size + dll_name_size);
        char* stub = new char[stub_size] {0};

        fwrite(table, sizeof(table[0]), size, file);

        fwrite(thunk, sizeof(thunk[0]), size - 1, file);

        for (size_t i = 0; i < size - 1; ++i)
            name_imp[i].put_in_file(file);

        fwrite(thunk, sizeof(thunk[0]), size - 1, file);

        fprintf(file, "sfasmlib.dll\0");

        fwrite(stub, sizeof(stub[0]), stub_size, file);

        delete[] stub;

        return true;
    }

    unsigned get_proc_addr(size_t num)
    {
        return sizeof(table[0]) * size + sizeof(thunk[0]) * (size - 1) + names_size + num * sizeof(thunk[0]);
    }

    ~import_tabel(void)
    {
        if (table != nullptr)
        {
            free(table);
            table = nullptr;
        }
        if (name_imp != nullptr)
        {
            free(name_imp);
            name_imp = nullptr;
        }
        if (thunk != nullptr)
        {
            free(thunk);
            thunk = nullptr;
        }
        size = 0;
    }
};


#endif // IMPORT_TABLE_H_INCLUDED
