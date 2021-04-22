/*
 * section_headers.c
 * 
 * Copyright 2021 Luka Knezic <luka@luka-vm>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void print_sh_type(Elf64_Word sh_type)
{
	if(sh_type==0) printf("%-17s", "NULL");
	else if(sh_type==1) printf("%-17s", "PROGBITS");
	else if(sh_type==2) printf("%-17s", "SYMTAB");
	else if(sh_type==3) printf("%-17s", "STRTAB");
	else if(sh_type==4) printf("%-17s", "RELA");
	else if(sh_type==5) printf("%-17s", "HASH");
	else if(sh_type==6) printf("%-17s", "DYNAMIC");
	else if(sh_type==7) printf("%-17s", "NOTE");
	else if(sh_type==8) printf("%-17s", "NOBITS");
	else if(sh_type==9) printf("%-17s", "REL");
	else if(sh_type==10) printf("%-17s", "SHLIB");
	else if(sh_type==11) printf("%-17s", "DYNSYM");
	else if(sh_type==0x10) printf("%-17s", "PREINIT_ARRAY");
	else if(sh_type==0xe) printf("%-17s", "INIT_ARRAY");
	else if(sh_type==0xf) printf("%-17s", "FINI_ARRAY");
	else if(sh_type==0x6ffffff6) printf("%-17s", "GNU_HASH");
	else if(sh_type==0x6ffffffe) printf("%-17s", "VERNEED");
	else if(sh_type==0x6fffffff) printf("%-17s", "VERSYM");
	else if(sh_type==0x70000000) printf("%-17s","LOPROC");
	else if(sh_type==0x7fffffff) printf("%-17s","HIPROC");
	else if(sh_type==0x80000000) printf("%-17s","LOUSER");
	else if(sh_type==0xffffffff) printf("%-17s","HIUSER");
	else printf("%-17s","NO_TYPE");
}

void print_sh_flags(Elf64_Word sh_flag)
{
	if(sh_flag==0x1) printf("%-10s"," W");
	else if(sh_flag==0x2) printf("%-10s"," A");
	else if(sh_flag==0x4) printf("%-10s"," X");
	else if(sh_flag==0xf0000000) printf("%-10s"," M");
	else if(sh_flag==0x3) printf("%-10s","WA");
	else if(sh_flag==0x5) printf("%-10s","WE");
	else if(sh_flag==0x6) printf("%-10s","AX");
	else if(sh_flag==0x20) printf("%-10s"," S");
	else if(sh_flag==0x40) printf("%-10s"," I");
	else if(sh_flag==0x80) printf("%-10s"," L");
	else if(sh_flag==0x100) printf("%-10s"," O");
	else if(sh_flag==0x200) printf("%-10s"," G");
	else if(sh_flag==0x400) printf("%-10s"," T");
	else if(sh_flag==0x80000000) printf("%-10s"," E");
	else printf("%-10s"," ");
}

void show_section_headers(FILE *file)
{
	Elf64_Ehdr elf_header;
	fread(&elf_header,sizeof(elf_header),1,file);
	int number_of_sections=elf_header.e_shnum;
	int section_offset=elf_header.e_shoff;

	Elf64_Shdr *section_header;
	section_header=(Elf64_Shdr *)malloc(number_of_sections*sizeof(Elf64_Shdr));
	fseek(file,section_offset,SEEK_SET);
	fread(section_header,number_of_sections,sizeof(Elf64_Shdr),file);
	
	printf("There are %d section headers, starting at offset 0x%x:\n\n", number_of_sections,section_offset);
	printf("Section Headers:\n\n");
	printf("  [Nr] Name              Type             Address           Offset\n");
    printf("       Size              EntSize          Flags  Link  Info  Align\n");
    for(int i=0;i<number_of_sections;i++)
    {
		printf("  [%2d] %-18d", i,section_header[i].sh_name);
		print_sh_type(section_header[i].sh_type);
		printf("%016lx  %08lx\n", section_header[i].sh_addr,section_header[i].sh_offset);
		printf("       %016lx  %016lx ", section_header[i].sh_size,section_header[i].sh_entsize);
		print_sh_flags(section_header[i].sh_flags);
		printf("%-5d %-5d %lu\n", section_header[i].sh_link,section_header[i].sh_info,section_header[i].sh_addralign);
	}
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
	
	free(section_header);
}

int main(int argc, char **argv)
{
	if(strcmp(argv[1],"readelf")==0 & strcmp(argv[2],"--section-headers")==0)
	{
		FILE* file = fopen(argv[0], "rb");
		if (file!=NULL)
		{
			show_section_headers(file);
			fclose(file);
		}
		else
			printf("Error in file opening!\n");
	}
	else 
		printf("command not found\n");
	return 0;
}

