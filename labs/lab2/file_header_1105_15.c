#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>

#define FILENAME_SIZE 256

typedef struct elf_file {
    FILE *file;         //sluzi za smijestanje pokazivaca na file ciji header ispisujemo
    Elf64_Ehdr *ehdr64; // elf header tabela
} elf_file_t;

elf_file_t *re_create_handle(const char *pathname)
{
    elf_file_t *ret = NULL;
    //pokazivac na strukturu elf_file_t
    elf_file_t *hdl = calloc(1, sizeof(elf_file_t));
    if (hdl == NULL)
        return NULL;
    //pokazivac na nas fajl
    hdl->file = fopen(pathname, "rb");
    if (hdl->file == NULL)
        free(hdl);

    // dohvatanje file header-a
    hdl->ehdr64 = calloc(1, sizeof(Elf64_Ehdr)); //alociranje prostora za header tabelu
    if (hdl->ehdr64 == NULL)
     {   
     	printf("Greska pri alokaciji prostora za elf header tabelu.\n");
        fclose(hdl->file);
     }
    //citanje binarnog file-a
    fread(hdl->ehdr64, 1, sizeof(Elf64_Ehdr), hdl->file);

    if (hdl->ehdr64->e_ident[EI_MAG0] != ELFMAG0 &&
            hdl->ehdr64->e_ident[EI_MAG1] != ELFMAG1 &&
            hdl->ehdr64->e_ident[EI_MAG2] != ELFMAG2 &&
            hdl->ehdr64->e_ident[EI_MAG3] != ELFMAG3)
         free(hdl->ehdr64);

    ret = hdl;
    return ret;
}
//f-ja koja vraca elf header tabelu
Elf64_Ehdr *re_get_elf_header(elf_file_t *hdl)
{
    return hdl->ehdr64;
}
//f-ja za oslobadjanje alociranog prostora za header i zatvaranje file-a
void re_free_handle(elf_file_t *hdl)
{
    fclose(hdl->file);
    free(hdl->ehdr64);
    free(hdl);
}

const char *ehdr_get_class(unsigned char e_class)
{
    switch (e_class) {
        case ELFCLASSNONE: return "This class is invalid"; break;
        case ELFCLASS32: return "ELF32"; break;
        case ELFCLASS64: return "ELF64"; break;
        default: return ""; break;
    }
}


const char *ehdr_get_data(unsigned char e_data)
{
    switch (e_data) {
        case ELFDATANONE: return "Unknown data format"; break;
        case ELFDATA2LSB: return "2's complement, little-endian"; break;
        case ELFDATA2MSB: return "2's complement, big-endian"; break;
        default: return ""; break;
    }
}


const char *ehdr_get_version(unsigned char e_version)
{
    switch (e_version) {
        case EV_NONE: return "(Invalid version)"; break;
        case EV_CURRENT: return "(Current version)"; break;
        default: return ""; break;
    }
}


const char *ehdr_get_osabi(unsigned char e_osabi)
{
    switch (e_osabi) {
        case ELFOSABI_SYSV: return "UNIX System V ABI"; break;
        case ELFOSABI_HPUX: return "HP-UX ABI"; break;
        case ELFOSABI_NETBSD: return "NetBSD ABI"; break;
        case ELFOSABI_LINUX: return "Linux ABI"; break;
        case ELFOSABI_SOLARIS: return "Solaris ABI"; break;
        case ELFOSABI_IRIX: return "IRIX ABI"; break;
        case ELFOSABI_FREEBSD: return "FreeBSD ABI"; break;
        case ELFOSABI_TRU64: return "TRU64 UNIX ABI"; break;
        case ELFOSABI_ARM: return "ARM architecture ABI"; break;
        case ELFOSABI_STANDALONE: return "Stand-alone (embedded) ABI"; break;
        default: return ""; break;
    }
}


const char *ehdr_get_type(unsigned char e_type)
{
    switch (e_type) {
        case ET_NONE: return "NONE (Unknown type)"; break;
        case ET_REL: return "REL (Relocatable file)"; break;
        case ET_EXEC: return "EXEC (Executable file)"; break;
        case ET_DYN: return "DYN (Shared file)"; break;
        case ET_CORE: return "CORE (Core file)"; break;
        default: return ""; break;
    }
}


const char *ehdr_get_machine(unsigned char e_machine)
{
    switch (e_machine) {
        case EM_NONE: return "An unknown machine"; break;
        case EM_M32: return "AT&T WE 32100"; break;
        case EM_SPARC: return "Sun Microsystems SPARC"; break;
        case EM_386: return "Intel 80386"; break;
        case EM_68K: return "Motorola 68000"; break;
        case EM_88K: return "Motorola 88000"; break;
        case EM_860: return "Intel 80860"; break;
        case EM_MIPS: return "MIPS RS3000 (big-endian only)"; break;
        case EM_PARISC: return "HP/PA"; break;
        case EM_SPARC32PLUS: return "SPARC with enhanced instruction set"; break;
        case EM_PPC: return "PowerPC"; break;
        case EM_PPC64: return "PowerPC 64-bit"; break;
        case EM_S390: return "IBM S/390"; break;
        case EM_ARM: return "Advanced RISC Machines"; break;
        case EM_SH: return "Renesas SuperH"; break;
        case EM_SPARCV9: return "SPARC v9 64-bit"; break;
        case EM_IA_64: return "Intel Itanium"; break;
        case EM_X86_64: return "AMD x86-64"; break;
        case EM_VAX: return "DEC Vax"; break;
        default: return ""; break;
    }
}
//f-ja za ispis ELF Header-a na ekran
static void dump_elf_file_header(Elf64_Ehdr *ehdr)
{
    printf("ELF HEADER:\n");
    printf("  Magic:   ");
    unsigned int i;
    for (i = 0; i < EI_NIDENT; i++)
        printf("%02x ", ehdr->e_ident[i]);
    printf("\n");
    printf("  %-35s%s\n", "Class:", ehdr_get_class(ehdr->e_ident[EI_CLASS]));
    printf("  %-35s%s\n", "Data:", ehdr_get_data(ehdr->e_ident[EI_DATA]));
    printf("  %-35s%d %s\n", "Version:", ehdr->e_ident[EI_VERSION], ehdr_get_version(ehdr->e_ident[EI_VERSION]));
    printf("  %-35s%s\n", "OS/ABI:", ehdr_get_osabi(ehdr->e_ident[EI_OSABI]));
    printf("  %-35s%d\n", "ABI Version:", ehdr->e_ident[EI_ABIVERSION]);
    printf("  %-35s%s\n", "Type:", ehdr_get_type(ehdr->e_type));
    printf("  %-35s%s\n", "Machine:", ehdr_get_machine(ehdr->e_machine));
    printf("  %-35s%s%d\n", "Version:", "0x", ehdr->e_version);
    printf("  %-35s%s%lx\n", "Entry point address:", "0x", ehdr->e_entry);
    printf("  %-35s%ld %s\n", "Start of program headers:", ehdr->e_phoff, "(bytes into file)");
    printf("  %-35s%ld %s\n", "Start of section headers:", ehdr->e_shoff, "(bytes into file)");
    printf("  %-35s%s%x\n", "Flags:", "0x", ehdr->e_flags);
    printf("  %-35s%d %s\n", "Size of this header:", ehdr->e_ehsize, "(bytes)");
    printf("  %-35s%d %s\n", "Size of program headers:", ehdr->e_phentsize, "(bytes)");
    printf("  %-35s%d\n", "Number of program headers:", ehdr->e_phnum);
    printf("  %-35s%d %s\n","Size of section headers:", ehdr->e_shentsize, "(bytes)");
    printf("  %-35s%d\n", "Number of section headers:", ehdr->e_shnum);
    printf("  %-35s%d\n", "Section header string table index:", ehdr->e_shstrndx);
}

static int read_elf(const char *pathname)
{
    elf_file_t *hdl = re_create_handle(pathname);
    if (hdl == NULL)
        return -1;

        // dohvatanje pokazivaca za file header
        Elf64_Ehdr *ehdr = re_get_elf_header(hdl);

        dump_elf_file_header(ehdr);

        re_free_handle(hdl);

    return 0;
}

int main(int argc, char* argv[])
{
    char *pathname=malloc(FILENAME_SIZE*sizeof(char));
    if(argc!=2)
    {
        printf("Neispravan unos argumenata!\n");
        printf("Upotreba: naziv_programa ime_datoteke\n");
    }
     strcpy(pathname,argv[1]);
     read_elf(pathname);
    return 0;
}

