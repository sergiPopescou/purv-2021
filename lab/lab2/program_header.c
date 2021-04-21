#include <stdio.h>
#include <elf.h>
#include <inttypes.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    FILE* elf_file=fopen(argv[1],"rb");
   
    if(elf_file)
    {
        Elf64_Ehdr elf_header;
        fread(&elf_header,sizeof(elf_header),1,elf_file);
       
        if(elf_header.e_ident[0]==0x7F && elf_header.e_ident[1]=='E' && elf_header.e_ident[2]=='L' && elf_header.e_ident[3]=='F')
        {
            Elf64_Phdr prog_header;
            Elf64_Phdr niz[elf_header.e_phnum];
            rewind(elf_file);

            printf("Elf file type is ");
            switch (elf_header.e_type)
            {
            case ET_NONE:
                printf("NONE (An Unkown type)\n");
                break;
            case ET_REL:
                printf("REL (Relocatable file)\n");
                break;
            case ET_CORE:
                printf("CORE (A core file)\n");
                break;
            case ET_DYN:
                printf("DYN (A shared object)\n");
                break;
            case ET_EXEC:
                printf("EXEC (An executable file)\n");
                break;     
            default:
                break;
            }

            printf("Entery point is 0x%"PRIx64"\n",elf_header.e_entry);
            printf("There are %d program headers starting at offset %"PRIu64"\n",elf_header.e_phnum,elf_header.e_phoff);
            printf("\nProgram Headers:\nType   Offset   VirtAddr   PhysAddr   FileSiz   MemSiz   Flags   Align\n");

            for(int i=0;i<elf_header.e_phnum;++i)
            { 
                fseek(elf_file,elf_header.e_phoff+i*sizeof(prog_header),SEEK_SET);
                fread(&prog_header,sizeof(prog_header),1,elf_file);
                niz[i]=prog_header;
                switch (prog_header.p_type)
                {
                case PT_NULL:
                    printf("\nNULL ");
                    break;
                case PT_LOAD:
                    printf("\nLOAD ");
                    break;
                case PT_DYNAMIC:
                    printf("\nDYNAMIC ");
                    break;
                case PT_INTERP:
                    printf("\nINTERP ");
                    break;
                case PT_NOTE:
                    printf("\nNOTE ");
                    break;
                case PT_SHLIB:
                    printf("\nSHLIB ");
                    break;
                case PT_PHDR:
                    printf("\nPHDR ");
                    break;
                case PT_GNU_EH_FRAME:
                    printf("\nGNU_EH_FRAME ");
                    break;
                case PT_GNU_STACK:
                    printf("\nGNU_STACK ");
                    break;
                case PT_GNU_RELRO:
                    printf("\nGNU_RELRO ");
                    break;
                default:
                    printf("\nSPECIFIC ");
                    break;
                }

                printf("0x%"PRIx64" ",prog_header.p_offset);
                printf("0x%"PRIx64" ",prog_header.p_vaddr);
                printf("0x%"PRIx64" ",prog_header.p_paddr);
                printf("0x%"PRIx64" ",prog_header.p_filesz);
                printf("0x%"PRIx64" ",prog_header.p_memsz);
                switch (prog_header.p_flags)
                {
                case PF_X:
                    printf("X ");
                    break;
                case PF_R:
                    printf("R ");
                    break;
                case PF_W:
                    printf("W ");
                    break;
                default:
                    break;
                }
                printf("0x%"PRIx64"\n",prog_header.p_align);
            }
            
            rewind(elf_file);
            
            printf("\nSegment to section mapping:\n");
            printf("Segment sections...\n");
            for(int i=0;i<elf_header.e_phnum;++i)
            {
                printf("%d ",i);
                char* sect_names = NULL;
                Elf64_Shdr section;

                fseek(elf_file, elf_header.e_shoff + elf_header.e_shstrndx * sizeof(section), SEEK_SET);
                fread(&section,sizeof(section),1, elf_file);

                sect_names = (char*)malloc(section.sh_size);
                fseek(elf_file, section.sh_offset, SEEK_SET);
                fread(sect_names,section.sh_size,1, elf_file);

                for (int j = 0; j < elf_header.e_shnum; j++)
                {
                    const char* name = "";

                    fseek(elf_file, elf_header.e_shoff + j * sizeof(section), SEEK_SET);
                    fread(&section, sizeof(section),1, elf_file);

                    if (section.sh_offset>=niz[i].p_offset && section.sh_offset<niz[i].p_offset+niz[i].p_filesz)
                    {
                        name = sect_names + section.sh_name;
                        printf("%s ", name);
                    }
                }
                printf("\n");
                free(sect_names);
            }
        } 
        else
        {
            printf("Ovo nije elf fajl!");
        }
    }
    else
    {
        printf("Greska!");
    }
    return 0;
}