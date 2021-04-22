#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include <libelf.h>

int counter1=0;
int counter2=0;
long position_t;
int first1=1;
int first2=1;

FILE* file;

Elf64_Sym sym;
Elf64_Ehdr header;
Elf64_Shdr section_header;
Elf64_Shdr section_header_temp;


void ucitaj_symbole(FILE*);
void print(int,Elf64_Sym);
void print_dynsym();
void print_symtab();




int main(int argc, char* argv[])
{

    if(argc<2)
    {
        printf("Nije dobar broj argumenata\n");
        return -1;
    }

    file= fopen(argv[1], "rb");
    char arhitektura[5];
    if(file!=NULL) 
    {
        fread(&arhitektura,1,5,file);
        fseek(file,0,0);
        
        if(arhitektura[4]==1)
            printf("32bit\n");
        else if(arhitektura[4]==2)
        {
            printf("64bit\n");
            ucitaj_symbole(file);
        }
        fclose(file);
    }
    else 
        printf("Greska pri otvaranju fajla");
    return 0;
}

void print_symtab()
{
    printf("   Symbol table '.symtab'\n");
    printf("Num:        Value            Size       Type      Bind       Vis     Ndx        Name   \n");
}

void print_dynsym()
{
    printf("   Symbol table '.dynsym'\n");
    printf("Num:        Value            Size       Type      Bind       Vis     Ndx        Name   \n");
}


void print(int counter, Elf64_Sym sym)
{
    char bind[20];
    
    char temp=sym.st_info;
    temp=temp&0b00001111;
    switch(temp)
    {
        case 0:
            strcpy(bind,"LOCAL");
            break;
      case 1:
        strcpy(bind,"GLOBAL");
        break;
    case 2:
        strcpy(bind,"WEAK");
        break;
    case 10:
        strcpy(bind,"LOOS");
        break;
    case 12:
        strcpy(bind,"HIOS");
        break;
    case 13:
        strcpy(bind,"LOPROC");
        break;
    case 15:
        strcpy(bind,"HIPROC");
        break;
    }

    char type[20];
    temp=sym.st_info;
    temp=temp&0b11110000;
    temp=temp>>4;
    switch(temp)
    {
        case 0:
            strcpy(type,"NOTYPE");
            break;
        case 1:
            strcpy(type,"OBJECT");
            break;
        case 2:
            strcpy(type,"FUNC");
            break;
        case 3:
            strcpy(type,"SECTION");
            break;
        case 4:
            strcpy(type,"FILE");
            break;
        case 5:
            strcpy(type,"COMMON");
            break;
        case 6:
            strcpy(type,"TLS");
            break;
        case 10:
        strcpy(bind,"LOOS");
        break;
    case 12:
        strcpy(bind,"HIOS");
        break;
    case 13:
        strcpy(bind,"LOPROC");
        break;
    case 15:
        strcpy(bind,"HIPROC");
        break;
    }
    
    char visibility[20];
    temp=sym.st_other;
    switch(temp)
    {
    case 0:
        strcpy(visibility,"DEFAULT");
        break;
    case 1:
        strcpy(visibility,"INTERNAL");
        break;
    case 2:
        strcpy(visibility,"HIDDEN");
        break;
    case 3:
        strcpy(visibility,"PROTECTED");
        break;
    case 4:
        strcpy(visibility,"EXPORTED");
        break;
    case 5:
        strcpy(visibility,"SINGLETON");
        break;
    case 6:
        strcpy(visibility,"ELIMINATE");
    }
    
    char name[100],c;
    int k=0;
    long position=ftell(file);
    fseek(file,header.e_shoff,0);
    fseek(file,position_t*sizeof(Elf64_Shdr),1);
    fread(&section_header_temp,1,sizeof(Elf64_Shdr),file);
    fseek(file,section_header_temp.sh_offset+sym.st_name,0);
    
    do
    {
        fread(&c,1,1,file);
        name[k++]=c;
    }while(c!=0);
    name[k]=0;
    fseek(file,position,0);
    if(sym.st_shndx!=0)
        printf(" %3d: %016lx %10ld %12s %12s %12s %5d %s\n",counter, sym.st_value, sym.st_size, type, bind, visibility, sym.st_shndx, name);
    else
        printf(" %3d: %016lx %10ld %12s %12s %12s   UND %s\n",counter, sym.st_value, sym.st_size, type, bind, visibility, name);
}
    
    
    
void ucitaj_symbole(FILE* file)
{
    fread(&header,sizeof(header),1,file);
    Elf64_Off section_header_offset=header.e_shoff;
    fseek(file,section_header_offset,0);
    fread(&section_header,sizeof(section_header),1,file);
    long position=ftell(file);
    for(int i=1;i<header.e_shnum;i++)
    {
        Elf64_Word type=section_header.sh_type;
        if(type==2 || type==11)
        {
            if(type==2 && first1)
            {
                print_symtab();
                first1=0;
            }
            if(type==11 && first2)
            {
                print_dynsym();
                first2=0;
            }
            
            double size = section_header.sh_size/sizeof(Elf64_Sym);
            position_t = section_header.sh_link;
            fseek(file,section_header.sh_offset,0);
            
            for(int i=0;i<size; i++)
            {
                fread(&sym, sizeof(sym),1,file);
                
                if(type==2)
                    print(counter1++,sym);
                else
                    print(counter2++,sym);
            }
            
            fseek(file,position,0);
            fread(&section_header,sizeof(section_header),1,file);
            
        }
        else
        {
            fread(&section_header,sizeof(section_header),1,file);
            position=ftell(file);
        }
        
    }
}
                
    
