#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <elf.h>

#define ELF_FILE "mmap"

char *flags(int n)
{
    char *flag = (char*) malloc(3);
    switch(n)
    {
        case 0: flag="   ";
            break;
        case 1: flag="  E";
            break;
        case 2: flag=" W ";
            break;
        case 3: flag=" WE";
            break;
        case 4: flag="R  ";
            break;
        case 5: flag="R E";
            break;
        case 6: flag="RW ";
            break;
        case 7: flag="RWE";
            break;
        default: flag=NULL;
    }
    return flag;
}


int main()
{
    short e_phentsize;      /*  Size  of  program  header  entry  */
    short e_phnum;          /*  Number  of  program  header  entries  */
    int64_t e_phoff;        /*  Program  header  offset  */
    FILE *fp;
    if((fp=fopen(ELF_FILE,"r"))==NULL)
    {
        printf("Error,file not found.\n");
    }else
    {

        fseek(fp,32,0);
        fread(&e_phoff,8,1,fp);
        
        fseek(fp,54,0);
        fread(&e_phentsize,2,1,fp);
        
        fseek(fp,56,0);
        fread(&e_phnum,2,1,fp);
       
        Elf64_Phdr *p_header=(Elf64_Phdr*) malloc(e_phnum*e_phentsize); //ukupna velicina prog header table= e_phnum*e_phentsize 

        fseek(fp,e_phoff,0);  /*offsetujem za broj bajtova e_phoff jer e_phoff ima info o poziciji program headera u ELF datoteci*/
        fread(p_header,e_phentsize,e_phnum,fp);

   
    printf("There are %d program headers, starting at offset %ld \n",e_phnum,e_phoff);
    printf("Program Headers:\n%-*s",16,"Type");
    printf("%-*s",20,"Offset");
    printf("%-*s",20,"VirtAddr");
    printf("%-*s\n",16,"PhysAddr");
    printf("%-20s%s","","FileSiz");
    printf("%*s",19,"MemSiz");
    printf("%*s     %s\n",20," Flags", "Align");
    for(int i=0; i<e_phnum; i++)
    {
      
       switch(p_header[i].p_type)
    {
        case 0x0: printf("NULL    \t");   /* Program header table entry unused */
            break;
        case 0x1: printf("LOAD    \t"); /* Loadable program segment */
            break;
        case 0x2: printf("DYNAMIC    \t"); /* Dynamic linking information */
            break;
        case 0x3: printf("INTERP    \t");  /* Program interpreter */
            break;
        case 0x4: printf("NOTE    \t"); /* Auxiliary information */
            break;
        case 0x5: printf("SHLIB    \t");  /* Reserved, unspecified semantics */
            break;
        case 0x6: printf("PHDR    \t");  /* Entry for header table itself */
            break;
        case 0x7: printf("TLS    \t"); /* Thread local storage segment */
            break;
        case 0x70000000: printf("LOPROC\t");   /* Processor-specific */
            break;
        case 0x7fffffff: printf("HIPROC\t");    /* Processor-specific */
            break;
        case 0x60000000: printf("LOOS\t");   /* OS-specific */
            break;
        case 0x6fffffff: printf("HIOS\t");  /* OS-specific */
            break;
        case 1685382480: printf("GNU_EH_FRAME\t"); /*PT_LOOS + 0x474e550)  Frame unwind information */
            break;       
        case 1685382481: printf("GNU_STACK\t");    /*PT_LOOS + 0x474e551)  Stack flags */
            break;        
        case 1685382482: printf("GNU_RELRO\t");    /*(PT_LOOS + 0x474e552) Read-only after relocation */
            break;
        case 0x6474e553: printf("GNU_PROPERTY\t");    /* .note.gnu.property notes sections.*/
            break;
        default: printf("Unknown type\t");
    }

	
        printf("    0x%016lx  ",p_header[i].p_offset);
        printf("0x%016lx  ",p_header[i].p_vaddr);
        printf("0x%016lx\n",p_header[i].p_paddr);
        printf("%-20s0x%016lx  ","",p_header[i].p_filesz);
        printf("0x%016lx  ",p_header[i].p_memsz);
        printf("%-8s",flags(p_header[i].p_flags));
        printf("0x%lx\n",p_header[i].p_align);
        
        if(p_header[i].p_type==0x3)
            printf("        [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]\n");
    }
    
    fclose(fp);
    }
    return 0;
}


