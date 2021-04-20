
#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
	
	unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */
  Elf64_Half    e_type;                 /* Object file type */
  Elf64_Half    e_machine;              /* Architecture */
  Elf64_Word    e_version;              /* Object file version */
  Elf64_Addr    e_entry;                /* Entry point virtual address */
  Elf64_Off     e_phoff;                /* Program header table file offset */
  Elf64_Off     e_shoff;                /* Section header table file offset */
  Elf64_Word    e_flags;                /* Processor-specific flags */
  Elf64_Half    e_ehsize;               /* ELF header size in bytes */
  Elf64_Half    e_phentsize;            /* Program header table entry size */
  Elf64_Half    e_phnum;                /* Program header table entry count */
  Elf64_Half    e_shentsize;            /* Section header table entry size */
  Elf64_Half    e_shnum;                /* Section header table entry count */
  Elf64_Half    e_shstrndx;             /* Section header string table index */
} Elf64Hdr;

void write_elf_header(Elf64Hdr header)
{

	puts("====================================================================");
	puts("\t\t\tELF FILE HEADER");
	
	printf("Magic: %x %x %x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x %0#2x\n",header.e_ident[0], header.e_ident[1], header.e_ident[2], header.e_ident[3], header.e_ident[4], header.e_ident[5], header.e_ident[6], header.e_ident[7], header.e_ident[8], header.e_ident[9], header.e_ident[10], header.e_ident[11], header.e_ident[12],header.e_ident[13],header.e_ident[14],header.e_ident[15]);
	
	if(header.e_ident[4] == 0)
		puts("Class: \t \t \t \t  ELF None");
	else if(header.e_ident[4] == 1)
		puts("Class: \t \t \t \t  ELF 32");
	else if(header.e_ident[4] == 2)
		puts("Class: \t \t \t \t  ELF 64");
		
	if(header.e_ident[5] == 0)
		puts("Data: \t \t \t \t  Invalid data encoding");
	else if(header.e_ident[5] == 1)
		puts("Data: \t \t \t \t  2's complement, little endian");
	else if(header.e_ident[5] == 2)
		puts("Data: \t \t \t \t  2's complement, big endian");
		
	printf("Version: \t \t \t  %x\n", header.e_ident[6]);
	
	puts("OS/ABI \t \t \t \t  UNIX - System V");
	puts("ABI Version: \t \t \t  0");
	
	
	
	if(header.e_type == 0)
		puts("Type: \t \t \t \t  No file type");
	else if(header.e_type == 1)
		puts("Type: \t \t \t \t  Relocalable file");
	else if(header.e_type == 2)
		puts("Type: \t \t \t \t  Executable file");
	else if(header.e_type == 3)
		puts("Type: \t \t \t \t  Shared object file");
	else if(header.e_type == 4)
		puts("Type: \t \t \t \t  Core file");
	else
		puts("Type: \t \t \t \t  Processor-specific");
		
	if(header.e_machine == 0)
		puts("Machine \t \t \t  EM NONE (No machine)");
	else if(header.e_machine == 1)
		puts("Machine \t \t \t  EM M32 (AT&T WE)");
	else if(header.e_machine == 2)
		puts("Machine \t \t \t  EM SPARC (SPARC)");
	else if(header.e_machine == 3)
		puts("Machine \t \t \t  EM 386 (Intel 80386)");
	else if(header.e_machine == 4)
		puts("Machine \t \t \t  EM 68K (Motorola 68000)");
	else if(header.e_machine == 5)
		puts("Machine \t \t \t  EM 88K (Motorola 88000)");
	else if(header.e_machine == 7)
		puts("Machine \t \t \t  860 (Intel 80860)");
	else if(header.e_machine == 8)
		puts("Machine \t \t \t  EM MIPS (RS3000)");
	else
		puts("Machine \t \t \t  Advanced Micro Devices X86-64");
		 
	printf("Size of this header: \t \t  %u \n",header.e_ehsize);
	
	
	if(header.e_version == 0)
		puts("Version: \t \t \t  0 (Invalid version)");
	else if(header.e_version == 1)
		puts("Version: \t \t \t  1 (Current version)");
		
	printf("Entry point address: \t \t  %0#lx\n", header.e_entry);
	
	printf("Start of program headers: \t  %lu (bytes into file)\n", header.e_phoff);
	
	printf("Start of section headers: \t  %lu (bytes into file)\n", header.e_shoff);
	
	printf("Flags: \t \t \t \t  0x%0#2x\n", header.e_flags);
	
	
	printf("Size of program headers: \t  %u(bytes)\n", header.e_phentsize);
	
	printf("Number of program headers: \t  %u\n", header.e_phnum);
	
	printf("Size of section headers: \t  %u(bytes)\n", header.e_shentsize);
	
	printf("Number of section headers: \t  %u\n", header.e_shnum);
	
	printf("Section header string table index:%u\n",header.e_shstrndx);
	
	
	
		
}

void read_elf_header(const char* elfFile) {
	
	Elf64Hdr header;

  FILE* file = fopen(elfFile, "rb");
  
  if(file) {
	  
	  
    
    fread(&header, 1, sizeof(header), file);

		   
	 if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) 
	 {
       
       //puts("Ispravan fajl");
       write_elf_header(header);
	 }
	 else
		puts("File is not ELF!");
       
       
    
    fclose(file);
  }
  else
	puts("ERROR: Mising File!!");
	
	
  
}


int main(void)
{
	char *s;
      int ln;
      puts("--> Inter name of the file! ");
      scanf("%s", s);
      ln = strlen(s);
      char *dyn_s = (char*) malloc (strlen(s)+1); 
      dyn_s = s;
      dyn_s[strlen(s)] = '\0';
      
	
	read_elf_header(dyn_s);
	
	return 0;
}
