#include <stdio.h>
#include "elf.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage:\n%s [elf file]\n", argv[0]);
        return 0;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    ELF32File elf;
    ELF32Read(fp, &elf);

    ELF32Analyze(elf, fp);

    ELF32Free(&elf);
    fclose(fp);
    return 0;
}
