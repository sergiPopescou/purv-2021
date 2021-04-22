/*
 * This program simulates the behavior of readelf -S
 * 
 * Author: Milan Vujmilovic
 * 
 *
 */

#include <stdio.h>
#include <elf.h>
#include <stdint.h>
#include <stdlib.h>

#define ERR_CODE -1
#define SUCCESS 0
#define TRUE 1
#define FALSE (!TRUE)
#define AR32 32
#define AR64 64

typedef struct sh_type {
    uint32_t value;
    char name[20];
}sh_type;

sh_type sh_info_hashtable[21] = {
    {0, "NULL"},
    {1, "PROGBITS"},
    {2, "SYMTAB"},
    {3, "STRTAB"},
    {4, "RELA"},
    {5, "HASH"},
    {6, "DYNAMIC"},
    {7, "NOTE"},
    {8, "NOBITS"},
    {9, "REL"},
    {10, "SHLIB"},
    {11, "DYNSIM"},
    {0x70000000, "LOPROC"},
    {0x7fffffff, "HIPROC"},
    {0x80000000, "LOUSER"},
    {0xffffffff, "HIUSER"},
    {0x6ffffff6, "GNU_HASH"},
    {0x6fffffff, "VERSYM"},
    {0x6ffffffe, "VERNEED"},
    {0xe, "INIT_ARRAY"},
    {0xf, "FINI_ARRAY"}
};

typedef struct flags {
    uint32_t value;
    char flag[2];
} flags;

flags flgs[5] = {
    {0x2, "A"},
    {0x42, "AI"},
    {0x6, "AX"},
    {0x3, "WA"},
    {0x30, "MS"},
    
};

// Function declarations:
char* find_type(uint32_t value);
uint8_t is_elf(Elf64_Ehdr*);
uint8_t check_architecture(const char*);
char* read_string_table(FILE* fd, Elf32_Shdr sh);
char* read_string_table64(FILE* fd, Elf64_Shdr sh);
uint8_t read_elf_sh32(const char* file_name);
uint8_t read_elf_sh64(const char* file_name);
char* find_flags(uint32_t value);
/*
 * File path has to be specified as a command line argument.
 */

uint8_t main(uint8_t argc, char* argv[])
{
    if(1 == argc)
    {
	printf("Specify the ELF file as the argument.\n");
	return ERR_CODE;
    }

    uint8_t architecture = check_architecture(argv[1]);
    if(architecture == AR32)
      read_elf_sh32(argv[1]);

    else if(architecture == AR64)
      read_elf_sh64(argv[1]);
    else
	return ERR_CODE;
    return SUCCESS;
}


uint8_t read_elf_sh32(const char* file_name) {
	Elf32_Ehdr header;
    FILE* file = fopen(file_name, "rb");
    
    if(file)
    {
        fread(&header, sizeof(header), 1, file);

        uint32_t offset = header.e_shoff; // start of section headers -> offset from beginning of the file; we start readeing S header here
        uint32_t num_of_headers = header.e_shnum;
        uint32_t string_table_index = header.e_shstrndx;

        printf("There are %d section headers, starting at offset 0x%x:\n\n", num_of_headers, offset);
        
	    if(is_elf(&header))
        {
            fseek(file, offset, SEEK_SET);
            // read all section headers
            Elf32_Shdr s_headers[num_of_headers];
            for(int i = 0; i < num_of_headers; i++) {
                fread(&(s_headers[i]), header.e_shentsize, 1, file);
            }
            // read string table
            char* string_table = read_string_table(file, s_headers[string_table_index]);

            printf("Section Headers:\n[Nr] Name               Type\t\t  Addr\t   Off\t    Size   ES  Flg   Lk Inf   Al\n");

            for(uint8_t i = 1; i < num_of_headers; i++) {
                if(i < 10)
                    printf("[ %d] ", i);
                else
                    printf("[%d]", i);
                
                printf("%-18s\t", s_headers[i].sh_name + string_table);
                printf("%-15s\t", find_type(s_headers[i].sh_type));
                //printf("  %x  ", s_header.sh_type);
                printf("  %08x", s_headers[i].sh_addr);
                printf(" %08x", s_headers[i].sh_offset);
                printf(" %06x", s_headers[i].sh_size);
                printf(" %02x", s_headers[i].sh_entsize);
                printf("   %-3s ", find_flags(s_headers[i].sh_flags));
                printf("   %-3d", s_headers[i].sh_link);
                printf("%-3d", s_headers[i].sh_info);
                printf("   %-2x", s_headers[i].sh_addralign);
                printf("\n");
               
            }
             printf("Key to Flags:\n");
                printf("W (write), A (alloc), X (execute), M (merge), S (strings), I (info),");
                printf("L (link order), O (extra OS processing required), G (group), T (TLS),");
                printf("C (compressed), x (unknown), o (OS specific), E (exclude),");
                printf("p (processor specific)");
        }
    }
}


uint8_t read_elf_sh64(const char* file_name) {
	Elf64_Ehdr header;
    FILE* file = fopen(file_name, "rb");
    
    if(file)
    {
        fread(&header, sizeof(header), 1, file);

        uint32_t offset = header.e_shoff; // start of section headers -> offset from beginning of the file; we start readeing S header here
        uint32_t num_of_headers = header.e_shnum;
        uint32_t string_table_index = header.e_shstrndx;

        printf("There are %d section headers, starting at offset 0x%x:\n\n", num_of_headers, offset);
        
	    if(is_elf(&header))
        {
            fseek(file, offset, SEEK_SET);
            // read all section headers
            Elf64_Shdr s_headers[num_of_headers];
            for(int i = 0; i < num_of_headers; i++) {
                fread(&(s_headers[i]), header.e_shentsize, 1, file);
            }
            // read string table
            char* string_table = read_string_table64(file, s_headers[string_table_index]);

            printf("Section Headers:\n[Nr] Name               Type\t\t  Addr\t   Off\t    Size   ES  Flg   Lk Inf   Al\n");

            for(uint8_t i = 1; i < num_of_headers; i++) {
                if(i < 10)
                    printf("[ %d] ", i);
                else
                    printf("[%d]", i);
                
                printf("%-18s\t", s_headers[i].sh_name + string_table);
                printf("%-15s\t", find_type(s_headers[i].sh_type));
                //printf("  %x  ", s_header.sh_type);
                printf("  %08x", s_headers[i].sh_addr);
                printf(" %08x", s_headers[i].sh_offset);
                printf(" %06x", s_headers[i].sh_size);
                printf(" %02x", s_headers[i].sh_entsize);
                printf("   %-3s ", find_flags(s_headers[i].sh_flags));
                printf("   %-3d", s_headers[i].sh_link);
                printf("%-3d", s_headers[i].sh_info);
                printf("   %-2x", s_headers[i].sh_addralign);
                printf("\n");
               
            }
            printf("Key to Flags:\n");
            printf("W (write), A (alloc), X (execute), M (merge), S (strings), I (info),");
            printf("L (link order), O (extra OS processing required), G (group), T (TLS),");
            printf("C (compressed), x (unknown), o (OS specific), E (exclude),");
            printf("p (processor specific)");
        }
    }
}

/*
 * This function checks if the given file is in ELF format.
 * Parameters: 
 *      header:  Elf64_Ehdr type file header (type defined in elf.h)
 * Returns:   TRUE   (1)   if the file is in the ELF format, or
 *            FALSE  (0)   if the file is not in the ELF format
 */
uint8_t is_elf(Elf64_Ehdr* header)
{
    if(header->e_ident[1] == 0x45 &&
       header->e_ident[2] == 0x4c &&
       header->e_ident[3] == 0x46)
	return TRUE;
    else
	return FALSE;
}


/*
 * This function checks if the file is 64 or 32 bit.
 * Parameters:
 *      file_name:  string that represents a path to file.
 * Returns:  32, 64 or ERR_CODE
 *      
 */
uint8_t check_architecture(const char* file_name)
{
    FILE* file = fopen(file_name, "rb");
    Elf64_Ehdr header;
    if(file)
    {
	fread(&header, sizeof(header), 1, file);
	if(header.e_ident[EI_CLASS] == ELFCLASS32) return AR32;
	else if(header.e_ident[EI_CLASS] == ELFCLASS64) return AR64;
	else
	    return ERR_CODE;
    }
}

char* find_type(uint32_t value) {
    for(int i = 0; i < 21; i++) {
        if(sh_info_hashtable[i].value == value)
            return sh_info_hashtable[i].name;
    }
    return "UNKNOWN";
}

char* find_flags(uint32_t value) {
    for(int i = 0; i < 5; i++) {
        if(flgs[i].value == value)
            return flgs[i].flag;
    }
    return "";
}

char* read_string_table(FILE* fd, Elf32_Shdr sh) {
    char* buff = malloc(sh.sh_size);
    fseek(fd, (off_t)sh.sh_offset, SEEK_SET);
    fread((void *)buff, sh.sh_size, 1, fd);
    return buff;
}


char* read_string_table64(FILE* fd, Elf64_Shdr sh) {
    char* buff = malloc(sh.sh_size);
    fseek(fd, (off_t)sh.sh_offset, SEEK_SET);
    fread((void *)buff, sh.sh_size, 1, fd);
    return buff;
}
