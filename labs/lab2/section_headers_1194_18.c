#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define size
#define EI_NIDENT 16

typedef long Elf64Addr;
typedef short Elf64Half;
typedef long Elf64Off;
typedef int Elf64Word;
typedef long Elf64DoubleWord;

// offset in bytes in file header where section header table offset info is stored
#define SECTION_HEADER_TABLE_OFFSET (EI_NIDENT + 2 * sizeof(Elf64Half) + sizeof(Elf64Word) + sizeof(Elf64Addr) + sizeof(Elf64Off))

// offset in bytes in file header whene section header size (one item size) is stored
#define SECTION_HEADER_SIZE (SECTION_HEADER_TABLE_OFFSET + 3 * sizeof(Elf64Half) + sizeof(Elf64Word) + sizeof(Elf64Off))

typedef struct {
    Elf64Word name; // 4 bytes (32-bit and 64-bit)
    Elf64Word type; // 4 bytes (32-bit and 64-bit)
    Elf64DoubleWord flags; // 4 bytes in 32-bit and 8 bytes in 64-bit
    Elf64Addr addr; // 4 bytes in 32-bit and 8 bytes in 64-bit
    Elf64Off offset; // 4 bytes in 32-bit and 8 bytes in 64-bit
    Elf64DoubleWord size; // 4 bytes in 32-bit and 8 bytes in 64-bit
    Elf64Word link; // 4 bytes (32-bit and 64-bit)
    Elf64Word info; // 4 bytes (32-bit and 64-bit)
    Elf64DoubleWord addrAlign; // 4 bytes in 32-bit and 8 bytes in 64-bit
    Elf64DoubleWord entSize; // 4 bytes in 32-bit and 8 bytes in 64-bit
} SectionHeader;

/*
    *
    * Get Flags String From Number
    * 
*/
char* getFlagsStringFromNumber(Elf64DoubleWord flags) {

    /*
        * 0x1 	        SHF_WRITE 	            Writable
        * 0x2 	        SHF_ALLOC 	            Occupies memory during execution
        * 0x4 	        SHF_EXECINSTR 	        Executable
        * 0x10 	        SHF_MERGE 	            Might be merged
        * 0x20 	        SHF_STRINGS 	        Contains null-terminated strings
        * 0x40 	        SHF_INFO_LINK 	        'sh_info' contains SHT index
        * 0x80 	        SHF_LINK_ORDER 	        Preserve order after combining
        * 0x100 	    SHF_OS_NONCONFORMING 	Non-standard OS specific handling required
        * 0x200 	    SHF_GROUP 	            Section is member of a group
        * 0x400 	    SHF_TLS 	            Section hold thread-local data
        * 0x0ff00000 	SHF_MASKOS 	            OS-specific
        * 0xf0000000 	SHF_MASKPROC 	        Processor-specific
        * 0x8000000 	SHF_EXCLUDE 	        Section is excluded unless referenced or allocated (Solaris) 
    */

   char* flagsStr = (char*) calloc(16, sizeof(char));

   if (flags & 0x1) strcat(flagsStr, "W");
   if (flags & 0x2) strcat(flagsStr, "A");
   if (flags & 0x4) strcat(flagsStr, "X");
   if (flags & 0x10) strcat(flagsStr, "M");
   if (flags & 0x20) strcat(flagsStr, "S");
   if (flags & 0x40) strcat(flagsStr, "I");
   if (flags & 0x80) strcat(flagsStr, "L");
   if (flags & 0x100) strcat(flagsStr, "O");
   if (flags & 0x200) strcat(flagsStr, "G");
   if (flags & 0x400) strcat(flagsStr, "T");
   if (flags & 0x8000000) strcat(flagsStr, "E");
   if (flags & 0x0ff00000) strcat(flagsStr, "o");
   if (flags & 0xf0000000) strcat(flagsStr, "p");

   return flagsStr;
}

/* 
    *
    * Get Type String From Number
    * 
*/
char* getTypeStringFromNumber(Elf64Word type) {

    /*
        * 0x0 	        SHT_NULL 	        Section header table entry unused
        * 0x1 	        SHT_PROGBITS 	    Program data
        * 0x2 	        SHT_SYMTAB 	        Symbol table
        * 0x3 	        SHT_STRTAB 	        String table
        * 0x4 	        SHT_RELA 	        Relocation entries with addends
        * 0x5 	        SHT_HASH 	        Symbol hash table
        * 0x6 	        SHT_DYNAMIC 	    Dynamic linking information
        * 0x7 	        SHT_NOTE 	        Notes
        * 0x8 	        SHT_NOBITS 	        Program space with no data (bss)
        * 0x9 	        SHT_REL 	        Relocation entries, no addends
        * 0x0A 	        SHT_SHLIB 	        Reserved
        * 0x0B 	        SHT_DYNSYM 	        Dynamic linker symbol table
        * 0x0E 	        SHT_INIT_ARRAY 	    Array of constructors
        * 0x0F 	        SHT_FINI_ARRAY 	    Array of destructors
        * 0x10 	        SHT_PREINIT_ARRAY 	Array of pre-constructors
        * 0x11 	        SHT_GROUP 	        Section group
        * 0x12 	        SHT_SYMTAB_SHNDX 	Extended section indices
        * 0x13 	        SHT_NUM 	        Number of defined types.
        * 0x60000000 	SHT_LOOS 	        Start OS-specific.
    */

    switch (type) {
        case 0x0: return "NULL";
        case 0x1: return "PROGBITS";
        case 0x2: return "SYMTAB";
        case 0x3: return "STRTAB";
        case 0x4: return "REAL";
        case 0x5: return "HASH";
        case 0x6: return "DYNAMIC";
        case 0x7: return "NOTE";
        case 0x8: return "NOBITS";
        case 0x9: return "REL";
        case 0x0A: return "SHLIB";
        case 0x0B: return "DYNSYM";
        case 0x0E: return "INIT_ARRAY";
        case 0x0F: return "FINI_ARRAY";
        case 0x10: return "PREINIT_ARRAY";
        case 0x11: return "GROUP";
        case 0x12: return "SYMTAB_SHNDX";
        case 0x13: return "NUM";
        default: return "LOOS";
    }
}

/*
    *
    * Print Header   
    * 
*/
void printHeaderFormat() {
    printf("%6s", "[Nr]");
    printf("%17s", "Name");
    printf("%18s", "Type");
    printf("%23s", "Address");
    printf("%9s\n", "Offset");
    printf("%23s", "Size");
    printf("%18s", "EntSize");
    printf("%11s%6s%6s", "Flags", "Link", "Info");
    printf("%9s\n", "Align");
}

/*
    *
    * Print One Row
    * 
*/
void printSectionData(int number, SectionHeader* data) {
    printf("  [%2d]", number);
    printf("%17d", data->name);
    printf("%18s", getTypeStringFromNumber(data->type));
    printf("       %016lx", data->addr);
    printf(" %08lx\n", data->offset);
    printf("%7s%016lx", "", data->size);
    printf("  %016lx", data->entSize);
    printf("%11s%6d%6d", getFlagsStringFromNumber(data->flags), data->link, data->info);
    printf("%9ld\n", data->addrAlign);
}

/*
    * 
    * Main Program Starts Here
    * 

*/
int main(int argc, char** argv) {

    if (argc == 2) {

        char* filename = argv[1];

        FILE* fp = fopen(filename, "rb");

        if (fp) {

            Elf64Off sectionOffset;
            Elf64Half sectionHeaderSize, sectionHeaderCount, stringTableOffset;
            
            fseek(fp, SECTION_HEADER_TABLE_OFFSET, SEEK_SET);
            fread(&sectionOffset, sizeof(Elf64Off), 1, fp);

            fseek(fp, SECTION_HEADER_SIZE, SEEK_SET);
            fread(&sectionHeaderSize, sizeof(Elf64Half), 1, fp);
            fread(&sectionHeaderCount, sizeof(Elf64Half), 1, fp);
            fread(&stringTableOffset, sizeof(Elf64Half), 1, fp);

            SectionHeader* sections = (SectionHeader *) calloc(sectionHeaderCount, sectionHeaderSize);
            if (sections) {

                fseek(fp, sectionOffset, SEEK_SET);
                fread(sections, sectionHeaderSize, sectionHeaderCount, fp);

                printHeaderFormat();

                for (int i = 0; i < sectionHeaderCount; ++i)
                    printSectionData(i, &sections[i]);

            } else
                printf("Can't allocate enough memory\n");

        } else
            printf("Can't open given file\n");

    } else
        printf("Wrong arguments\n");

    return 0;
}