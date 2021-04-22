//------- PURV - VJEZBA BR. 2 -------
//STUDENT      : Aleksandar Simonovic
//BROJ INDEKSA : 1154/16
//-----------------------------------

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define EI_NIDENT 16

//DEFINISANJE TIPOVA
//ELF HEADER
typedef struct
{
	unsigned char e_ident[EI_NIDENT]; // Bajt 1-4 - Magic number, Bajt 5 - 32/64bit ELF, Bajt 6 - Little/Big Endian, Bajt 7 - Verzija ELF headera (uvijek 1), Bajt 8 - OS ABI, Bajt 9 - Ne koristi se, Bajt 10-16 - poravnanje
	uint16_t      e_type;             // Tip fajla (REL, EXEC, DYN, CORE)
	uint16_t      e_machine;		  // Arhitektura (x86, MIPS, ARM, AMD64, ARVv8, ...)
	uint32_t      e_version;          // Object file verzija (Uvijek 1)
	uint64_t      e_entry;            // Virtuelna adresa ulazne tacke u program
	uint64_t      e_phoff;            // Ofset u fajlu na kom pocinje Program Header tabela izrazen u Bajtovima
	uint64_t      e_shoff;            // Ofset u fajlu na kom pocinje Section Header tabela izrazen u Bajtovima
	uint32_t      e_flags;            // Flag-ovi specificni za procesor
	uint16_t      e_ehsize;           // Velicina ELF zaglavlja izrazena u Bajtovima
	uint16_t      e_phentsize;        // Velicina svakog elementa u Program header tabeli
	uint16_t      e_phnum;            // Broj elemenata u Program Header tabeli
	uint16_t      e_shentsize;        // Velicina svakog elementa u Section header tabeli
	uint16_t      e_shnum;            // Broj elemenata u Section Header tabeli
	uint16_t      e_shstrndx;         // Na ovom indeksu u Section Header tabeli nalazi se Section Header koji se odnosi na sekciju koja sadrzi string tabelu u kojoj se nalaze imena svih sekcija. Ova string tabela je niz null-terminiranih stringova.
} Elf64_Ehdr;

//PROGRAM HEADER se odnosi na segmente
typedef struct 
{
	uint32_t   p_type;				  // Tip segmenta
	uint32_t   p_flags;			      // Prava citanja/pisanja/izvrsavanja
	uint64_t   p_offset;			  // Ofset segmenta od pocetka fajla
	uint64_t   p_vaddr;				  // Virtuelna adresa prvog bajta segmenta
	uint64_t   p_paddr;               // Fizicka adresa na koju ce segment biti ucitan
	uint64_t   p_filesz;              // Velicina segmenta u fajlu
	uint64_t   p_memsz;               // Velicina segmenta u memoriji
	uint64_t   p_align;               // Informacija o poravnanju
} Elf64_Phdr; 

//SECTION HEADER se odnosi na sekcije
typedef struct {
    uint32_t   sh_name;				  // Ime sekcije - predstavlja ofset u Section Header String tabeli
    uint32_t   sh_type;				  // Tip sekcije
    uint64_t   sh_flags;			  // Flegovi vezani za sekcije
    uint64_t   sh_addr;				  // Adresa na koju ce upisati prvi bajt sekcije ako se ona potrebna u memoriji
    uint64_t   sh_offset;             // Ofset sekcije od pocetka fajla
    uint64_t   sh_size;               // Velicina sekcije u fajlu
    uint32_t   sh_link;               // Informacije vezane za linkovanje sekcija
    uint32_t   sh_info;               // Dodadne informacije
    uint64_t   sh_addralign;          // Poravnanje
    uint64_t   sh_entsize;            // Velicina svakog elementa tabele
} Elf64_Shdr;

//DEKLARACIJA FUNKCIJA
void readType(char*, uint32_t);
void readFlag(char*, uint32_t);
void readHeaders(FILE*);

int main(int argc, char** argv)
{
	if (argc != 2) 
	{
        printf("KORISTI: %s filename\n", argv[0]);
        return(0);
    }
    
    FILE* filePointer = fopen(argv[1], "rb");
    if(filePointer == NULL)
    {
    	printf("Fajl %s se ne moze otvoriti!\n", argv[1]);
    	return 0;
    }
    
	readHeaders(filePointer); 
	
	fclose(filePointer);
	return 0;
}

void readType(char* type, uint32_t value)
{
	switch (value) 
	{
		case 0:
			strcpy(type, "NULL");
			break;
		case 1:
			strcpy(type, "LOAD");
			break;
		case 2:
			strcpy(type, "DYNAMIC");
			break;
		case 3:
			strcpy(type, "INTERP");
			break;
		case 4:
			strcpy(type, "NOTE");
			break;
		case 5:
			strcpy(type, "SHLIB");
			break;
		case 6:
			strcpy(type, "PHDR");
			break;
		case 7:
			strcpy(type, "TLS");
			break;
		case 8:
			strcpy(type, "NUM");
			break;
		case 0x60000000:
			strcpy(type, "LOOS");
			break;
		case 0x6fffffff:
			strcpy(type, "HIOS");
			break;
		case 0x70000000:
			strcpy(type, "LOPROC");
			break;
		case 0x7fffffff:
			strcpy(type, "HIPROC");
			break;
		case 0x6474e550:
			strcpy(type, "GNU_EH_FRAME");
			break;
		case 0x6474e551:
			strcpy(type, "GNU_STACK");
			break;
		case 0x6474e552: 
			strcpy(type, "GNU_RELRO");
            break;
		case 0x6474e553:
			strcpy(type, "GNU_PROPERTY");
			break;
        case 0x6ffffffa:
        	strcpy(type, "LOSUNW");
       		break;
       	case 0x6fffffff:
       		strcpy(type, "HISUNW");
       		break;
       	case 0x6ffffffb:
       		strcpy(type, "SUNWSTACK");
       		break;
		default:
			strcpy(type, "Unknown");
			break;
		}
}

void readFlag(char* flag, uint32_t value)
{
	switch (value) 
	{
		case 0:
			strcpy(flag, "   ");
			break;
		case 1:
			strcpy(flag, "  E");
			break;
		case 2:
			strcpy(flag, " W ");
			break;
		case 3:
			strcpy(flag, " WE");
			break;
		case 4:
			strcpy(flag, "R  ");
			break;
		case 5:
			strcpy(flag, "R E");
			break;
		case 6:
			strcpy(flag, "RW ");
			break;
		case 7:
			strcpy(flag, "RWE");
			break;
		default:
			strcpy(flag, "Unknown");
	}
}

void readHeaders(FILE* filePointer)
{
	//Citanje ELF headera
	Elf64_Ehdr elfHeader;
	fread(&elfHeader, sizeof(Elf64_Ehdr), 1, filePointer);
	
	if(elfHeader.e_phoff == 0)
	{
		printf("No program header table!\n");
		return;
	}
	
	fseek(filePointer, elfHeader.e_phoff, SEEK_SET);
	Elf64_Phdr programHeaderArray[elfHeader.e_phnum];
	
	for(int i = 0; i < elfHeader.e_phnum; i++)
	{
		fread(&programHeaderArray[i], elfHeader.e_phentsize, 1, filePointer);
	}
	
	//ISPIS:
	printf("Elf file type is ");
	switch (elfHeader.e_type) {
		case 0:
			printf("NONE (An unknown type))\n");
			break;
		case 1:
			printf("REL (A relocatable file)\n");
			break;
		case 2:
			printf("EXEC (An executable file)\n");
			break;
		case 3:
			printf("DYN (A shared object)\n");
			break;
		case 4:
			printf("CORE (A core file)\n");
			break;
	}
	printf("Entry point %#lx\n", elfHeader.e_entry);
	printf("There are %d program headers, starting at offset %ld\n\n", elfHeader.e_phnum,  elfHeader.e_phoff);
	printf("Program Headers:\n");
	printf("  Type           Offset             VirtAddr           PhysAddr\n");
	printf("                 FileSiz            MemSiz              Flags  Align\n");
	
	for(int i = 0; i < elfHeader.e_phnum; i++)
	{
		char type[15];
		char flag[15];
		readType(type, programHeaderArray[i].p_type);
		readFlag(flag, programHeaderArray[i].p_flags);
		printf("  %-14s %0#18lx %0#18lx %0#18lx\n", type, programHeaderArray[i].p_offset, programHeaderArray[i].p_vaddr, programHeaderArray[i].p_paddr);
		printf("                 %0#18lx %0#18lx  %-7s %#lx\n", programHeaderArray[i].p_filesz, programHeaderArray[i].p_memsz, flag, programHeaderArray[i].p_align);
		if(programHeaderArray[i].p_type==3)
			{
			printf("        [Requesting program interpreter: ");
			char karakter;
			fseek(filePointer, programHeaderArray[i].p_offset,SEEK_SET);
			while ((karakter = fgetc(filePointer)) != 0)
                printf("%c", karakter); 
            printf("]\n");
			}	
	}
	
	
	Elf64_Shdr sectionHeaderArray[elfHeader.e_shnum];
	fseek(filePointer, elfHeader.e_shoff, SEEK_SET);
    for(int i = 0; i < elfHeader.e_shnum; i++)
	{
		fread(&sectionHeaderArray[i], elfHeader.e_shentsize, 1, filePointer);
	}
	int offsetStringSekcije = sectionHeaderArray[elfHeader.e_shstrndx].sh_offset;
	printf(" Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	
	for(int i = 0 ; i < elfHeader.e_phnum; i++)
	{
		printf("   %02d     ",i);
		for(int j = 0; j < elfHeader.e_shnum; j++)
        {
        	//Ako sekcija pocinje i zavrsava unutar nekog segmenta 
            if(sectionHeaderArray[j].sh_offset >= programHeaderArray[i].p_offset && (sectionHeaderArray[j].sh_offset+sectionHeaderArray[j].sh_size) <= (programHeaderArray[i].p_offset+programHeaderArray[i].p_filesz))
            {  
            	//Citamo karakter po karakter, s tim sto se nazivi sekcija nalaze u string table sekciji
            	char karakter; 
                fseek(filePointer,offsetStringSekcije+sectionHeaderArray[j].sh_name,SEEK_SET);
                while ((karakter = fgetc(filePointer)) != 0)
                {
                	if(karakter == '.')
                		printf(" ");
                	printf("%c", karakter);
                }
            }            
        }
		printf("\n");
	}
}
