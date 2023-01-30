#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "elf64.h"



#define	ET_NONE	0	//No file type 
#define	ET_REL	1	//Relocatable file 
#define	ET_EXEC	2	//Executable file 
#define	ET_DYN	3	//Shared object file 
#define	ET_CORE	4	//Core file 
#define SHT_SYMTAB 2
#define STB_GLOBAL 1

/* symbol_name		- The symbol (maybe function) we need to search for.
 * exe_file_name	- The file where we search the symbol in.
 * error_val		- If  1: A global symbol was found, and defined in the given executable.
 * 			- If -1: Symbol not found.
 *			- If -2: Only a local symbol was found.
 * 			- If -3: File is not an executable.
 * 			- If -4: The symbol was found, it is global, but it is not defined in the executable.
 * return value		- The address which the symbol_name will be loaded to, if the symbol was found and is global.
 */

unsigned long find_symbol(char* symbol_name, char* exe_file_name, int* error_val) {
	FILE* fd = fopen(exe_file_name, "r");
	if (fd == NULL) {
        *error_val = -3;
        return 0;
    }	
	
	Elf64_Ehdr elf_header;
	fread(&elf_header, sizeof(Elf64_Ehdr), 1, fd);
	if(elf_header.e_type != ET_EXEC){  //check if file is executable or not
		*error_val = -3;
		fclose(fd);
		return 0;
	}

    Elf64_Shdr section_headers[elf_header.e_shnum];
	fseek(fd, elf_header.e_shoff ,SEEK_SET);
	fread(section_headers, elf_header.e_shentsize , elf_header.e_shnum, fd);

    // lets read string table
    char* section_header_str_tab;
    Elf64_Shdr header_str_tab = section_headers[elf_header.e_shstrndx];
    section_header_str_tab = (char*)malloc(header_str_tab.sh_size);
    fseek(fd, header_str_tab.sh_offset, SEEK_SET);
    fread(section_header_str_tab , header_str_tab.sh_size, 1, fd );

    // lets read symbol table
    char* sym_tab = NULL;
    int symtab_index ;
    for (int i = 0; i < elf_header.e_shnum; i++)
	{
		if (strcmp(section_header_str_tab + section_headers[i].sh_name, ".symtab") == 0)
		{
			symtab_index = i;
            break;
		}
        symtab_index = -1; //not relevant because it always gone find the string
	}

    Elf64_Shdr sym_tab_header = section_headers[symtab_index];
    sym_tab = (char*)malloc(sym_tab_header.sh_size);
    fseek(fd, sym_tab_header.sh_offset, SEEK_SET);
	fread(sym_tab, sym_tab_header.sh_size, 1, fd);

    //extarct the string table it self

    char* str_tab = NULL;
    int strtab_index , i;

    for (i = 0; i < elf_header.e_shnum; i++)
	{
		if (strcmp(section_header_str_tab + section_headers[i].sh_name, ".strtab") == 0)
		{
			strtab_index = i;
			break;
		}
		strtab_index =  -1;
	}

	Elf64_Shdr str_tab_header = section_headers[strtab_index];
	str_tab = (char*)malloc(str_tab_header.sh_size);
	fseek(fd, str_tab_header.sh_offset, SEEK_SET);
	fread(str_tab, str_tab_header.sh_size, 1, fd);

	//extract the symbol from the symbols table
    Elf64_Sym* symbol = NULL;
    int amount_of_symbols = sym_tab_header.sh_size / sym_tab_header.sh_entsize;
    for (i = 0; i < amount_of_symbols; ++i)
	{
		symbol = (Elf64_Sym*)(sym_tab + i * sym_tab_header.sh_entsize);
		char* curr_symbol_name = str_tab + symbol->st_name;
		if(strcmp(curr_symbol_name , symbol_name) == 0)
		{
			if(ELF64_ST_BIND(symbol->st_info) == STB_GLOBAL)
				break;
			if(ELF64_ST_BIND(symbol->st_info) != STB_GLOBAL)
				break;
			if(symbol->st_shndx == SHN_UNDEF)
				break;
		}
		if(i == amount_of_symbols-1)
			symbol = NULL;
	}
 
 
    if (symbol == NULL)
	{
		*error_val = -1;
		free(section_header_str_tab);
        free(sym_tab);
        free(str_tab);
        fclose(fd);
		return 0;
	}   

	if (ELF64_ST_BIND(symbol->st_info) != STB_GLOBAL)
	{
        // Symbol is local
		*error_val = -2;
		free(section_header_str_tab);
        free(sym_tab);
        free(str_tab);
        fclose(fd);
		return 0;
	}

	if (symbol->st_shndx == SHN_UNDEF)
	{
        // Symbol is global, but not defined in the executable
		*error_val = -4;
		free(section_header_str_tab);
        free(sym_tab);
        free(str_tab);
        fclose(fd);
		return 0;
	}

    //we found the symbol ,Symbol is global and defined as executable
	*error_val = 1;
	free(section_header_str_tab);
    free(sym_tab);
    free(str_tab);
    fclose(fd);
	return symbol->st_value;
}


int main(int argc, char *const argv[]) {
	int err = 0;
	unsigned long addr = find_symbol(argv[1], argv[2], &err);

	if (err > 0)
		printf("%s will be loaded to 0x%lx\n", argv[1], addr);
	else if (err == -2)
		printf("%s is not a global symbol! :(\n", argv[1]);
	else if (err == -1)
		printf("%s not found!\n", argv[1]);
	else if (err == -3)
		printf("%s not an executable! :(\n", argv[2]);
	else if (err == -4)
		printf("%s is a global symbol, but will come from a shared library\n", argv[1]);
	return 0;
}

