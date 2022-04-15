#include "backend.h"

DWORD EXIT_ADDR         = 0;
DWORD GET_NUMBER_ADDR   = 0;
DWORD PRINT_NUMBER_ADDR = 0;

bool create_exe (bin_tree *tree, FILE *exe_file, variables *var)
{
    assert(tree);
    assert(exe_file);
    assert(var);

    const size_t stub_size = SIZE_RAW;
    BYTE zeros[stub_size] = { 0 };
    size_t ptr_raw = 0x400;

    // --- ALL HEADERS:

    IMAGE_DOS_HEADER dos_header = { 0 };
    fill_dos_header(&dos_header);

    IMAGE_NT_HEADERS NT_header  = { 0 };
    fill_NT_header(&NT_header);

    IMAGE_SECTION_HEADER text_section = { 0 };
    fill_section_header(&text_section, ".text", VRT_SIZE, ENTRY_POINT_ADDR, SIZE_RAW, ptr_raw, CODE_CHARACTER);

    IMAGE_SECTION_HEADER import_data_section = { 0 };
    fill_section_header(&import_data_section, ".idata", VRT_SIZE, IMPORT_START, SIZE_RAW, ptr_raw + SIZE_RAW, IDATA_CHARACTER);

    IMAGE_SECTION_HEADER data_section = { 0 };
    fill_section_header(&data_section, ".data", VRT_SIZE, DATA_START, SIZE_RAW, ptr_raw + 2 * SIZE_RAW, DATA_CHARACTER);
    
    fwrite(&dos_header, sizeof(IMAGE_DOS_HEADER), 1, exe_file);
    fwrite(&DOS_STUB, sizeof(DOS_STUB[0]), sizeof(DOS_STUB) / sizeof(DOS_STUB[0]), exe_file);
    fwrite(&NT_header, sizeof(IMAGE_NT_HEADERS), 1, exe_file);

    fwrite(&text_section,        sizeof(IMAGE_SECTION_HEADER), 1, exe_file);
    fwrite(&import_data_section, sizeof(IMAGE_SECTION_HEADER), 1, exe_file);
    fwrite(&data_section,        sizeof(IMAGE_SECTION_HEADER), 1, exe_file);

    size_t sizeof_headers = sizeof(IMAGE_DOS_HEADER) + sizeof(DOS_STUB) + sizeof(IMAGE_NT_HEADERS) + 3 * sizeof(IMAGE_SECTION_HEADER);
    fwrite(zeros, sizeof(zeros[0]), ptr_raw - sizeof_headers, exe_file); // for alignment (1024 bytes of all headers)

    // --- END OF HEADERS

    import_tabel imp_tabel;
    imp_tabel.fill_default();

    EXIT_ADDR         = IMAGE_BASE + IMPORT_START + imp_tabel.get_proc_addr(0);
    GET_NUMBER_ADDR   = IMAGE_BASE + IMPORT_START + imp_tabel.get_proc_addr(1);
    PRINT_NUMBER_ADDR = IMAGE_BASE + IMPORT_START + imp_tabel.get_proc_addr(2);

    // --- PROGRAM SECTIONS:

    // 1. TEXT SECTION

    BYTE *opcodes = (BYTE*) calloc(SIZE_RAW, sizeof(BYTE));
    fill_text_sec(tree, opcodes, var);

    fwrite((unsigned char*) opcodes, sizeof(opcodes[0]), stub_size, exe_file);

    // 2. IMPORT DATA SECTION
    imp_tabel.put_in_file(exe_file);

    // 3. DATA SECTION
    fwrite(zeros, sizeof(zeros[0]), stub_size, exe_file);

    // --- EVERYTHING IS WRITTEN

    free(opcodes);

    return true;
}

void fill_dos_header (IMAGE_DOS_HEADER *dos_header)
{
    dos_header->e_magic    = 'ZM';      // Mark Zbikowski

    dos_header->e_cblp     = 0x0090;    
    dos_header->e_cp       = 0x0003;
    dos_header->e_cparhdr  = 0x0004;    // 64 bytes (size of DOS Header)

    dos_header->e_minalloc = 0x0010;
    dos_header->e_maxalloc = 0xFFFF;

    dos_header->e_sp       = 0x00B8;
    dos_header->e_lfarlc   = 0x0040;

    dos_header->e_lfanew   = 0x00B0;    // The beginning of NT Header
}

void fill_NT_header (IMAGE_NT_HEADERS *NT_header, int NUM_OF_SEC)
{
    NT_header->Signature = 'EP';

    fill_NT_file_header(&(NT_header->FileHeader), NUM_OF_SEC);
    fill_NT_optional_header(&(NT_header->OptionalHeader));
}

void fill_NT_file_header (IMAGE_FILE_HEADER *NT_file_header, int NUM_OF_SEC)
{
    NT_file_header->Machine              = IMAGE_FILE_MACHINE_I386;
    NT_file_header->NumberOfSections     = static_cast<WORD>(NUM_OF_SEC),
    NT_file_header->TimeDateStamp        = static_cast<DWORD>(time(nullptr));
    NT_file_header->SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
    NT_file_header->Characteristics      = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_32BIT_MACHINE;
}

void fill_NT_optional_header (IMAGE_OPTIONAL_HEADER *NT_optional_header)
{
    NT_optional_header->Magic                 = IMAGE_NT_OPTIONAL_HDR32_MAGIC;

    NT_optional_header->AddressOfEntryPoint   = ENTRY_POINT_ADDR;
    NT_optional_header->ImageBase             = IMAGE_BASE;
    NT_optional_header->BaseOfCode            = ENTRY_POINT_ADDR;
    NT_optional_header->BaseOfData            = DATA_START;
    NT_optional_header->SectionAlignment      = ENTRY_POINT_ADDR;
    NT_optional_header->SizeOfImage           = SIZE_RAW + 3 * VRT_SIZE;

    NT_optional_header->FileAlignment         = 0x200;
    NT_optional_header->SizeOfHeaders         = 0x400;

    NT_optional_header->MajorSubsystemVersion = 4;
    NT_optional_header->Subsystem             = IMAGE_SUBSYSTEM_WINDOWS_CUI;
    NT_optional_header->NumberOfRvaAndSizes   = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
    NT_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = IMPORT_START;
}

void fill_section_header (IMAGE_SECTION_HEADER *section, const char sec_name[], size_t vrt_size, size_t vrt_addr,
                         size_t size_raw_data, size_t ptr_raw_data, size_t character)
{
    sprintf((char *const) section->Name, "%s", sec_name);
    section->Misc.VirtualSize = vrt_size;

    section->VirtualAddress   = vrt_addr;
    section->SizeOfRawData    = size_raw_data;
    section->PointerToRawData = ptr_raw_data;
    section->Characteristics  = character;
}
