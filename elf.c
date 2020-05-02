#include "elf.h"

char *getString(FILE *fp, uint32_t offset, uint16_t index)
{
    uint32_t position = offset+index;
    uint32_t size = 0;
    fseek(fp, position, SEEK_SET);

    while (!(fgetc(fp) == '\x00')) size++;
    fseek(fp, position, SEEK_SET);
    char *__retstr = malloc(size+1);
    memset(__retstr, 0, size+1);
    fread(__retstr, size, 1, fp);
    return __retstr;
}

void _ELF32HeaderInfo(ELF32File elf, Elf32_Ehdr header, FILE *fp)
{
    printf("ElfClass: %s\n", (header.e_ident[4] == 1) ? ELFCLASS32 : ((header.e_ident[4] == 2) ? ELFCLASS64 : ELFCLASSNONE));
    printf("Encoding: %s\n", (header.e_ident[5] == 1) ? ELFDATA2LSB : ((header.e_ident[5] == 2) ? ELFDATA2MSB : ELFDATANONE));
    printf("Version: 0x%x\n", header.e_ident[6]);
    
    printf("File type: ");
    switch (header.e_type) {
    case 0:
        printf("None (No file type)\n");
        break;
    case 1:
        printf("Relocatable file\n");
        break;
    case 2:
        printf("Executable file\n");
        break;
    case 3:
        printf("Shared object file\n");
        break;
    case 4:
        printf("Core file\n");
        break;
    default:
        printf("Unknown\n");
        break;
    }

    printf("Architecture: ");
    switch (header.e_machine) {
    case 0:
        printf("None (No machine)");
        break;
    case 1:
        printf("AT&T WE 32100\n");
        break;
    case 2:
        printf("SPARC\n");
        break;
    case 3:
        printf("Intel 80386\n");
        break;
    case 4:
        printf("Motorola 68000\n");
        break;
    case 5:
        printf("Motorola 88000\n");
        break;
    case 7:
        printf("Intel 80860\n");
        break;
    case 8:
        printf("MIPS RS3000\n");
        break;
    default:
        printf("Unknown\n");
        break;
    }

    if (header.e_entry != 0) {
        printf("Entry: 0x%04x\n", header.e_entry);
    }

    printf("Program Header Table offset: 0x%04x\n", header.e_phoff);
    printf("Section Header Table offset: 0x%04x\n", header.e_shoff);
    printf("Header Size: 0x%02x\n", header.e_ehsize);
    printf("Size of Header Entry: %d bytes\n", header.e_phentsize);
    printf("Header Entries: %d\n", header.e_phnum);
    printf("Size of Section Entry: %d bytes\n", header.e_shentsize);
    printf("Section Entries: %d\n", header.e_shnum);
}

void _ELF32SectionInfo(ELF32File elf, Elf32_Shdr section, FILE *fp)
{
    if (elf.e_hdr.e_shstrndx != 0) {
        char *sectionName = getString(fp, elf.s_hdr[elf.e_hdr.e_shstrndx].sh_offset, section.sh_name);
        printf("Name: %s\n", sectionName);
        free(sectionName);
    }

    printf("Type: ");
    switch (section.sh_type) {
    case SHT_NULL:
        printf("NULL\n");
        break;
    case SHT_PROGBITS:
        printf("PROGBITS\n");
        break;
    case SHT_SYMTAB:
        printf("SYMTAB\n");
        break;
    case SHT_STRTAB:
        printf("STRTAB\n");
        break;
    case SHT_RELA:
        printf("RELA\n");
        break;
    case SHT_HASH:
        printf("HASH\n");
        break;
    case SHT_DYNAMIC:
        printf("DYNAMIC\n");
        break;
    case SHT_NOTE:
        printf("NOTE\n");
        break;
    case SHT_NOBITS:
        printf("NOBITS\n");
        break;
    case SHT_REL:
        printf("REL\n");
        break;
    case SHT_SHLIB:
        printf("SHLIB\n");
        break;
    case SHT_DYNSYM:
        printf("DYNSYM\n");
        break;
    case SHT_LOPROC:
        printf("LOPROC\n");
        break;
    case SHT_HIPROC:
        printf("HIPROC\n");
        break;
    case SHT_LOUSER:
        printf("LOUSER\n");
        break;
    case SHT_HIUSER:
        printf("HIUSER\n");
        break;
    default:
        printf("Invalid\n");
        break;
    }

    printf("Section Flags:");
    if (section.sh_flags & SHF_WRITE) {
        printf(" SHF_WRITE");
    }
    if (section.sh_flags & SHF_ALLOC) {
        printf(" SHF_ALLOC");
    }
    if (section.sh_flags & SHF_EXECINSTR) {
        printf(" SHF_EXECINSTR");
    }
    if (section.sh_flags & SHF_MASKPROC) {
        printf(" SHF_MASKPROC");
    }
    printf("\n");

    printf("Memory Load Address: 0x%04x\n", section.sh_addr);
    printf("File Offset: 0x%04x\n", section.sh_offset);
    printf("Section size: %d bytes\n", section.sh_size);
    printf("Address Align: %d\n", section.sh_addralign);
    printf("Entry Size: %d bytes\n", section.sh_entsize);
}

void ELF32Analyze(ELF32File elf, FILE *fp)
{
    printf("-==========Header==========-\n");
    _ELF32HeaderInfo(elf, elf.e_hdr, fp);

    printf("\n-=========Sections=========-\n");
    for (uint16_t i=0;i<elf.e_hdr.e_shnum;i++) {
        printf("=== SECTION %d ===\n", i);
        _ELF32SectionInfo(elf, elf.s_hdr[i], fp);
        printf("\n");
    }
}

void ELF32Read(FILE *fp, ELF32File *elf)
{
    fread(&elf->e_hdr, sizeof(Elf32_Ehdr), 1, fp);

    // sections
    if (elf->e_hdr.e_shoff != 0) {
        fseek(fp, elf->e_hdr.e_shoff, SEEK_SET);
        elf->s_hdr = malloc(elf->e_hdr.e_shnum*elf->e_hdr.e_shentsize);
        
        for (uint16_t i=0;i<elf->e_hdr.e_shnum;i++) {
            fread(&elf->s_hdr[i], elf->e_hdr.e_shentsize, 1, fp);
        }
    }

    // program header
    if (elf->e_hdr.e_phoff != 0) {
        fseek(fp, elf->e_hdr.e_phoff, SEEK_SET);
        elf->p_hdr = malloc(elf->e_hdr.e_phnum*elf->e_hdr.e_phentsize);

        for (uint16_t i=0;i<elf->e_hdr.e_phnum;i++) {
            fread(&elf->p_hdr[i], elf->e_hdr.e_phentsize, 1, fp);
        }
    }
}

void ELF32Free(ELF32File *elf)
{
    if (elf->e_hdr.e_shoff) free(elf->s_hdr);
    if (elf->e_hdr.e_phoff) free(elf->p_hdr);
}