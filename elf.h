#ifndef ELF_H
#define ELF_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define EI_NIDENT 16
#define ELFCLASSNONE "None (Invalid class)"
#define ELFCLASS32 "32-bit objects"
#define ELFCLASS64 "64-bit objects"
#define ELFDATANONE "None (Invalid encoding)"
#define ELFDATA2LSB "LSB (Little Endian)"
#define ELFDATA2MSB "MSB (Big Endian)"

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000


typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Off;

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
}__attribute__((packed)) Elf32_Ehdr;

typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
}__attribute__((packed)) Elf32_Shdr;

typedef struct {
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
}__attribute__((packed)) Elf32_Sym;

typedef struct {
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
}__attribute__((packed)) Elf32_Phdr;

typedef struct {
    Elf32_Ehdr e_hdr;
    Elf32_Shdr *s_hdr;
    Elf32_Phdr *p_hdr;
} ELF32File;

void ELF32Read(FILE*, ELF32File*);
void ELF32Free(ELF32File*);
void ELF32Analyze(ELF32File, FILE*);

#endif