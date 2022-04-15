#ifndef EXE_CREATOR_H_INCLUDED
#define EXE_CREATOR_H_INCLUDED

// To understand everything here you can checkk my .exe ccreation guide:
// ...

#include "exe_config.h"
#include "import_table.h"

struct variables;

bool create_exe (bin_tree *tree, FILE *exe_file, variables *var);

void fill_dos_header (IMAGE_DOS_HEADER *dos_header);

void fill_NT_header (IMAGE_NT_HEADERS *NT_header, int NUM_OF_SEC = 3);
void fill_NT_file_header (IMAGE_FILE_HEADER *NT_file_header, int NUM_OF_SEC);
void fill_NT_optional_header (IMAGE_OPTIONAL_HEADER *NT_optional_header);

void fill_section_header (IMAGE_SECTION_HEADER *section, const char sec_name[], size_t vrt_size, size_t vrt_addr,
    size_t size_raw_data, size_t ptr_raw_data, size_t charac);


#endif // EXE_CREATOR_H_INCLUDED