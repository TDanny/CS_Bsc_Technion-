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

int getIndexOfSection(Elf64_Shdr *section_header, Elf64_Ehdr *elf_header, char *section_header_string_table, char *section_name){
    for (int i = 0; i < elf_header->e_shnum; i++){
        if (strcmp(section_header_string_table + section_header[i].sh_name, section_name) == 0){
            return i;
        }
    }
    return -1;
}

uint32_t getIndexOfSymInDynamic(Elf64_Shdr *dynsym_table_header, char *dynstr_table, char *dynsym_table, char *symbol_input){
    int i = 0;
    for (; i <  dynsym_table_header->sh_size / dynsym_table_header->sh_entsize; ++i){
        Elf64_Sym *symbol = (Elf64_Sym *)(dynsym_table + i * dynsym_table_header->sh_entsize);
        char *symbol_name = dynstr_table + symbol->st_name;
        if (strcmp(symbol_name, symbol_input) == 0){
            return i;
        }
    }
    return -1;
}

unsigned long getRelAddress(FILE *file, Elf64_Shdr *section_headers, Elf64_Ehdr *elf_header, char *section_header_str_tab, char *symbol_name){
    unsigned long got_entry_addr = 0;
    
	//extract the relllocation table into rel_table
	int rel_index = getIndexOfSection(section_headers, elf_header, section_header_str_tab, ".rela.plt");
    Elf64_Shdr rel_header = section_headers[rel_index];
	Elf64_Rela *rel_table = (Elf64_Rela *)malloc(rel_header.sh_size);
    fseek(file, rel_header.sh_offset, SEEK_SET);
    fread(rel_table, rel_header.sh_size, 1, file);
	
	//extract the dynamic symbol table table into dynsym_able
    int dynsym_index = getIndexOfSection(section_headers, elf_header, section_header_str_tab, ".dynsym");
    Elf64_Shdr dynsym_header = section_headers[dynsym_index];
	Elf64_Shdr *dynsym_table = (Elf64_Shdr *)malloc(dynsym_header.sh_size);
    fseek(file, dynsym_header.sh_offset, SEEK_SET);
    fread(dynsym_table, dynsym_header.sh_size, 1, file);
	
	//extract the dynamic string table table into dynstr_table
    int dynstr_index = getIndexOfSection(section_headers, elf_header, section_header_str_tab, ".dynstr");
    Elf64_Shdr dynstr_header = section_headers[dynstr_index];
    char *dynstr_table = (char *)malloc(dynstr_header.sh_size);
    fseek(file, dynstr_header.sh_offset, SEEK_SET);
    fread(dynstr_table, dynstr_header.sh_size, 1, file);

    

	//looks for the place of the symbol in the dynamic symbol table
    int indexSym = -1;
    for (int i = 0; i <  dynsym_header.sh_size / dynsym_header.sh_entsize; ++i){
        Elf64_Sym *symbol = (Elf64_Sym *)((char*)dynsym_table + i * dynsym_header.sh_entsize);
        char *symbol_name1 = dynstr_table + symbol->st_name;
        if (strcmp(symbol_name1, symbol_name) == 0){
            indexSym = i;
        }
    }
	
    for (int i = 0; i < rel_header.sh_size / rel_header.sh_entsize; ++i){
        Elf64_Rela rel = rel_table[i];
		//if the symbol's pointer of the rellocation entry equal to index_of_sym, this is the entry we are looking for
        if (ELF64_R_SYM(rel.r_info) == indexSym){
            got_entry_addr = rel.r_offset;
            free(dynstr_table);
            free(dynsym_table);
            free(rel_table);
            return got_entry_addr;
        }
    }
    free(dynstr_table);
    free(dynsym_table);
    free(rel_table);
    return got_entry_addr;
}




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
        symtab_index = -1;
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
		unsigned long address = getRelAddress(fd, section_headers, &elf_header, section_header_str_tab, symbol_name);
		free(section_header_str_tab);
        free(sym_tab);
        free(str_tab);
        fclose(fd);
		return address;
	}
    //we found the symbol ,Symbol is global and defined as executable
	*error_val = 1;
	free(section_header_str_tab);
    free(sym_tab);
    free(str_tab);
    fclose(fd);
	return symbol->st_value;
}
    
long putBreakpoint(unsigned long func_address, pid_t child_pid){
    long data = ptrace(PTRACE_PEEKTEXT, child_pid, (void *)func_address, NULL);
    unsigned long data_trap = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
    ptrace(PTRACE_POKETEXT, child_pid, (void *)func_address, (void *)data_trap);
    return data;
}

void removeBreakpoint(pid_t child_pid, unsigned long func_addr, unsigned long data){
    ptrace(PTRACE_POKETEXT, child_pid, (void *)func_addr, (void *)data);
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
    regs.rip -= 1;
    ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
}

bool backToTheCalee(pid_t child_pid, unsigned long stack_address) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
    return (regs.rsp == stack_address);
}

void getRetAddress(pid_t child_pid, struct user_regs_struct *regs, unsigned long *ret_address){
    *ret_address = ptrace(PTRACE_PEEKTEXT, child_pid, regs->rsp, NULL);
}

void run_counter_debugger(pid_t child_pid, unsigned long func_addr, bool is_relocatable){
    int wait_status=0;
    int counter = 0;
	unsigned long stack_address = 0;
    unsigned long GOT_entry_addr = 0;
    struct user_regs_struct regs;
    unsigned long return_address = 0;
    long return_data = 0;
    waitpid(child_pid, &wait_status, 0);

	//if is relocatable we need to extract the true function address
    if (is_relocatable){
        GOT_entry_addr = func_addr;
        func_addr = ptrace(PTRACE_PEEKTEXT, child_pid, (void *)GOT_entry_addr, NULL);
    }

	//puts a breakpoint in the start of the function
    long first_command = putBreakpoint(func_addr, child_pid);
    ptrace(PTRACE_CONT, child_pid, NULL, NULL);
    wait(&wait_status);
    while (WIFSTOPPED(wait_status)){
        ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	

		//put the breakpoint in the calee func, so we 
        stack_address = regs.rsp + 8;
        getRetAddress(child_pid, &regs, &return_address);
        return_data = putBreakpoint(return_address, child_pid);
        removeBreakpoint(child_pid, func_addr, first_command);
        ptrace(PTRACE_CONT, child_pid, NULL, NULL);
        waitpid(child_pid, &wait_status, 0);

		//if we are in recorsive call we will continue without increasing the counter
        while (!backToTheCalee(child_pid, stack_address) && WIFSTOPPED(wait_status)){
            removeBreakpoint(child_pid, return_address, return_data);
            ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL);
            wait(&wait_status);
            return_data = putBreakpoint(return_address, child_pid);
            ptrace(PTRACE_CONT, child_pid, NULL, NULL);
            waitpid(child_pid, &wait_status, 0);
        }
		
		//if we got here, we need to increase the counter
        if (WIFSTOPPED(wait_status)){
            counter++;
            ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
            printf("PRF:: run #%d returned with %lu\n", counter, (long int)regs.rax);
            removeBreakpoint(child_pid, return_address, return_data);
            if (counter == 1 && is_relocatable){
                func_addr = ptrace(PTRACE_PEEKTEXT, child_pid, (void *)GOT_entry_addr, NULL);
            }
            first_command = putBreakpoint(func_addr, child_pid);
            ptrace(PTRACE_CONT, child_pid, NULL, NULL);
            waitpid(child_pid, &wait_status, 0);
        }
    }
}

pid_t run_target(char *const argv[]){
    pid_t pid = fork();
    if (pid > 0){
        return pid;
    }
    else if (pid == 0){
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0){
            perror("ptrace");
            exit(1);
        }
        if (execv(argv[2], argv+2) < 0){
            perror("execv");
            exit(1);
        }
    }
    else{
        perror("fork");
        exit(1);
    }
}


int main(int argc, char *const argv[]) {
	int err = 0;
	unsigned long addr = find_symbol(argv[1], argv[2], &err);
	if (err == -1)
		printf("PRF:: %s not found!\n", argv[1]);
	else if (err == -2)
		printf("PRF:: %s is not a global symbol! :(\n", argv[1]);
	else if (err == -3)
		printf("PRF:: %s not an executable! :(\n", argv[2]);
	else{
		pid_t child_pid = run_target(argv);
        run_counter_debugger(child_pid, addr, err == -4);
	}
	
	return 0;
}
