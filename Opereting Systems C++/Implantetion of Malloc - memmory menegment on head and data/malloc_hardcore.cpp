#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <cstring>
#include "malloc_hardcore.h"

#define MAX_SIZE 100000000
#define MIN_REMAINING 128
#define MMAP_MIN_SIZE 128*1024
#define METASIZE 72

typedef struct MallocMetadata {
    int upper_cookie;
    size_t size;
    bool is_free;
    MallocMetadata* next_by_size;
    MallocMetadata* prev_by_size;
    void* addr;
    MallocMetadata* prev_alloc_addr;
    MallocMetadata* next_alloc_addr;
    int lower_cookie;
}*Meta;

class Block_list {
    public:
        Meta head=nullptr;
        Meta tail=nullptr;
        size_t num_free_blocks = 0;
        size_t num_free_bytes = 0;
        size_t num_allocated_blocks = 0;
        size_t num_allocated_bytes = 0;
        Meta last_alloc_Meta = nullptr;

        Meta mmap_head = nullptr;
        Meta mmap_tail = nullptr;
        Meta last_alloc_mmap = nullptr;
        
        void* insert(size_t size);
        void* insertAux(Meta current, void* addr , size_t size , bool is_free, bool split_flag);
        void* checkIfAllocForNumElementsExist(size_t size);
        void freeBlock(void* addr);
        void* splitTheBlock(Meta block_to_split ,size_t size_part1, size_t size_of_part2);
        void mergeBlock(Meta from_p , Meta to_p);
        void* wilderness(size_t size);
        void* insertMmap(size_t size);
        void checkCookie(Meta block);

};



/*-------------------------------------STATIC ------------------------------ */

static Block_list block_list;
//static Blist block_list = (Blist)block_list_static;
static int cookie = rand();

/*-------------------------------------STAT FUNCTIONS ------------------------------ */


//Return the number of bytes of a single meta-data structure in your system.
size_t _size_meta_data(){
    return sizeof(MallocMetadata);
}

//Returns the number of allocated blocks in the heap that are currently free.
size_t _num_free_blocks(){
    return block_list.num_free_blocks;
}

//Returns the number of bytes in all allocated blocks in the heap that are currently free, excluding the bytes used by the meta-data structs.
size_t _num_free_bytes(){
    return block_list.num_free_bytes;
}

//Returns the overall (free and used) number of allocated blocks in the heap.
size_t _num_allocated_blocks(){
    return block_list.num_allocated_blocks;
}

//Returns the overall number (free and used) of allocated bytes in the heap, excluding the bytes used by the meta-data structs.
size_t _num_allocated_bytes(){
    return block_list.num_allocated_bytes;
}

//Returns the overall number of meta-data bytes currently in the heap.
size_t _num_meta_data_bytes(){
    return (block_list.num_allocated_blocks) * sizeof(MallocMetadata);
}


/*-------------------------------------MMAP METHODS ------------------------------ */


void* Block_list::insertMmap(size_t size){
    Meta new_block = (Meta)mmap(NULL,size+ METASIZE, PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if(new_block == (void *)(-1)){
        return nullptr;
    }
    if(last_alloc_mmap){
        std::cout << "[insert] list not empty connect to the last" << std::endl;
        Meta last_malloc_meta = last_alloc_mmap;
        last_malloc_meta->next_alloc_addr = new_block;
        new_block->prev_alloc_addr = last_malloc_meta;
    } else{
       std::cout << "[insert] malloc list is empty, first node" << std::endl;
        new_block->prev_alloc_addr=nullptr;
    }
    last_alloc_Meta = new_block;
    new_block->next_alloc_addr = nullptr;

    void* address = (void *)((size_t)new_block + METASIZE), *result =nullptr;
    result = insertAux(new_block , address , size , false, false);
    if(!result){
        return nullptr;
    }
    return new_block->addr;
}



/*----------------------Block_list methods-----------------------*/
void* Block_list::insertAux(Meta block, void* addr , size_t size , bool is_free, bool split_flag){
    std::cout << "[insertAux] updating list of nodes by size"<< std::endl;
    if(block == (void*)(-1))
        return nullptr;
    Meta itr = nullptr , meta_of_prev=nullptr , list_head = nullptr ;
    block->lower_cookie = cookie;
    block->upper_cookie = cookie;
    
    //CHOOSE WHAT KIND OF LIST WE DEAL WITH
    if(size<MMAP_MIN_SIZE){
        itr = block_list.head;
        list_head = block_list.head;

    }else{
        itr = block_list.mmap_head;
        list_head = block_list.mmap_head;

    }

    if(split_flag){
        std::cout << "[insertAux] split enable, splitting"<< std::endl;
        block->is_free = is_free;
        if((block->prev_by_size && block->prev_by_size->size <= block->size) && (block->next_by_size && block->next_by_size->size >= block->size))
            return block->addr;
        if(!block->next_by_size && (block->prev_by_size && block->prev_by_size->size <= block->size)){
            std::cout << "Case 2"<< std::endl;
            return block->addr;
        }
        if(!block->prev_by_size && (block->next_by_size && block->next_by_size->size >= block->size)){
            std::cout << "Case 3"<< std::endl;
            return block->addr;
        }
            
        

        //first I want to get the block out of the> block list :
        if(!(block->prev_by_size)){
            std::cout << "insertAux - block->next is nullptr becuse block is head" << std::endl;
            list_head = block->next_by_size;
            if(head)
                head->prev_by_size = nullptr;
        }else if(!(block->next_by_size)){
            std::cout << "insertAux - block->next is nullptr becuse block is tail" << std::endl;
            block_list.tail = block->prev_by_size;
            if(tail)
                tail->next_by_size = nullptr;
        }else{
            (block->prev_by_size)->next_by_size = block->next_by_size;
            (block->next_by_size)->prev_by_size = block->prev_by_size;
        }

    } else{ //INCASE WE DONT NEED TO SPLIT
        std::cout << "[insertAux] split disable"<< std::endl;
        //prepare the new node:
        block->is_free = false;
        block->next_by_size = nullptr;
        block->prev_by_size = nullptr;
        block->size = size;
        block->addr = addr;
        num_allocated_blocks++;
        num_allocated_bytes+=size;
    }
    //if block_list is empty:
    if(!itr && !split_flag){
        std::cout << "[insertAux] block list sorted is empty, insert first"<< std::endl;
        if(block->size < MMAP_MIN_SIZE){
            head=block;
            tail=block;
        } else{
            mmap_head=block;
            mmap_tail=block;
        }
        
        
        return block->addr;
    }
    std::cout << "[insertAux] block list sorted is not empty"<< std::endl;
    while(itr){
        if(itr->size > size){
                std::cout << "[insertAux] found place before end of sorted list"<< std::endl;
            if(itr == list_head) //need to enter first
            { 
                    std::cout << "[insertAux] found place in the begining"<< std::endl;
                block->next_by_size=itr;
                itr->prev_by_size = block;
                if(size < MMAP_MIN_SIZE ){
                    head = block;
                } else{
                    mmap_head = block;
                }

                return block->addr; 
            } else{
                std::cout << "[insertAux] found place in middle"<< std::endl;
                block->next_by_size = itr;
                block->prev_by_size = itr->prev_by_size;
                itr->prev_by_size->next_by_size = block;
                itr->prev_by_size = block;
                
                /*
                meta_of_prev = itr->prev_by_size;
                itr->prev_by_size = block;
                block->next_by_size = itr;
                block->prev_by_size = meta_of_prev;
                */
                return block->addr;   
            }
        }
        else if(itr->size == size){ //in that case we need to check which address is lower
            if(block->addr < itr->addr){
                    std::cout << "[insertAux] same size but lower adress"<< std::endl;
                if(itr == list_head){ //Incase we need to insert the new block before the head block:
                    block->next_by_size=itr;
                    block->prev_by_size = nullptr;
                    itr->prev_by_size = block;
                    list_head = block;
                    return block->addr;    
                }else //itr not first
                {
                    meta_of_prev = itr->prev_by_size;
                    itr->prev_by_size= block;
                    block->next_by_size = itr;
                    block->prev_by_size = meta_of_prev;
                    return block->addr;    
                }
            }
        } 
        
        itr = itr->next_by_size;
        
    }

    if(block->size >= MMAP_MIN_SIZE){
        meta_of_prev = mmap_tail;
        meta_of_prev->next_by_size = block;
        block->prev_by_size = meta_of_prev;
        mmap_tail = block;
    } else{
        std::cout << "[insertAux]add block to tail"<< std::endl;
        meta_of_prev = tail;
        meta_of_prev->next_by_size = block;
        block->prev_by_size = meta_of_prev;
        tail = block;
    }

    return block->addr;
}


void* Block_list::insert(size_t size){
std::cout << "[insert] insert new block to block list"<<std::endl;
    void* new_block_meta = sbrk(METASIZE+size);
    if (new_block_meta == (void*)(-1)){
        return nullptr;
    }
    
    Meta new_block = (Meta)new_block_meta;
    if(last_alloc_Meta){
        std::cout << "[insert] list not empty connect to the last" << std::endl;
        Meta last_malloc_meta = last_alloc_Meta;
        last_malloc_meta->next_alloc_addr = new_block;
        new_block->prev_alloc_addr = last_malloc_meta;
    } else{
       std::cout << "[insert] malloc list is empty, first node" << std::endl;
        new_block->prev_alloc_addr=nullptr;
    }
    last_alloc_Meta = new_block;
    new_block->next_alloc_addr = nullptr;

     
    void* address = (void*)((size_t)new_block_meta + METASIZE);
    return insertAux(new_block, address, size, false, false);
}
/*
void* Block_list::checkIfAllocForNumElementsExist(size_t size){
    Meta curr = block_list->head;
    void* ptr_space = nullptr;
    while(curr){
        if(curr->size >= size && curr->is_free){
            ptr_space = curr->addr;
            return ptr_space;
        }
    curr = curr->next; 
    }
    return nullptr;
}
*/

void Block_list::freeBlock(void* addr_to_free){
    if(!addr_to_free)
        return;
    Meta curr = nullptr;
    Meta block_to_remove = (Meta)((void*)((size_t)addr_to_free - METASIZE));
    if(block_to_remove->size < MMAP_MIN_SIZE){
            curr = head;
        while(curr){
            checkCookie(curr);
            if(curr->addr  == addr_to_free){ 
                if((curr->prev_alloc_addr && curr->prev_alloc_addr->is_free) && (curr->next_alloc_addr && curr->next_alloc_addr->is_free)){
                    std::cout << "[FreeBlock] Merge with Both sides" << std::endl; //check
                    mergeBlock(curr->prev_alloc_addr,curr->next_alloc_addr);
                    num_free_blocks--;
                }
                else if(curr->prev_alloc_addr && curr->prev_alloc_addr->is_free){
                    std::cout << "[FreeBlock] Merge with next" << std::endl; //check
                    mergeBlock(curr->prev_alloc_addr,curr);
                }
                else if(curr->next_alloc_addr && curr->next_alloc_addr->is_free){
                    std::cout << "[FreeBlock] Merge with Next" << std::endl; //check
                    mergeBlock(curr,curr->next_alloc_addr);
                } else{
                    std::cout << "[FreeBlock] Free without any merge" << std::endl; //check
                    curr->is_free=true;
                    num_free_blocks++;
                    num_free_bytes += curr->size;

                }            
                return;
            }
        
            curr = curr->next_by_size;
        }
        std::cout << "[FreeBlock] error: didnt find the node" << std::endl; //check
    } else{
        curr = mmap_head;
        if(block_to_remove == mmap_head && block_to_remove == mmap_tail){ //INCASE THE LIST HAS ONE MMAP-FILE
            mmap_head = nullptr;
            mmap_tail = nullptr;
        }
        else if(block_to_remove == mmap_head){
            block_to_remove->next_by_size->prev_by_size=nullptr;
            mmap_head = block_to_remove->next_by_size;
        }
        else if(block_to_remove == mmap_tail){
            block_to_remove->prev_by_size->next_by_size = nullptr;
            mmap_tail = block_to_remove->prev_by_size;
        }else {
            block_to_remove->prev_by_size->next_by_size = block_to_remove->next_by_size;
            block_to_remove->next_by_size->prev_by_size = block_to_remove->prev_by_size;
        }

        block_list.num_allocated_blocks--;
        block_list.num_allocated_bytes -= block_to_remove->size;
        size_t free_size = block_to_remove->size + METASIZE;
        munmap(block_to_remove, free_size);
    }
    

}


void* Block_list::splitTheBlock(Meta block_to_split ,size_t size_part1, size_t size_of_part2){
    size_t address_to_split = (size_t)(block_to_split->addr) + METASIZE +  size_part1;
    num_allocated_bytes -= METASIZE; //reduce because Meta going to be create
    //num_free_bytes -= block_to_split->size;
    std::cout << "[splitTheBlock] the address of the begining of the block BEFORE split"<< block_to_split->addr<<std::endl;
    if(num_free_bytes>= size_part1 + METASIZE)
        num_free_bytes -=  size_part1 + METASIZE; 
    else
        num_free_bytes=0;
    num_allocated_blocks++;
    block_to_split->is_free = false;

    //size_t old_size = block_to_split->size;
    // udpate next and prev alloc
    // remove block_to_split from free list
    // isert blocks to free list 
    void* curr_addr = block_to_split->addr;
    Meta new_block_part2 = (Meta)((void*)address_to_split); // cast to Meta
    //required adjestments :
    new_block_part2->prev_by_size = block_to_split;
    new_block_part2->prev_alloc_addr = block_to_split;
    new_block_part2->next_by_size = block_to_split->next_by_size;
    block_to_split->size = size_part1;
    block_to_split->next_by_size = new_block_part2;
    block_to_split->next_alloc_addr = new_block_part2;
    new_block_part2->size = size_of_part2;
    new_block_part2->addr = (void*)(address_to_split + METASIZE);
    if(block_to_split == last_alloc_Meta)
        last_alloc_Meta = new_block_part2;
    if(!new_block_part2->next_by_size)
        tail = new_block_part2;
    void* address = insertAux(block_to_split,curr_addr, size_part1, false, true);
    address = insertAux(new_block_part2, new_block_part2->addr, size_of_part2, true, true);
    std::cout << "[splitTheBlock] the address of the begining of the block AFTER split : " << (size_t)block_to_split->addr << std::endl;
    if(!address){
        std::cout << "[splitTheBlock] THE SECOND BLOCK ADDRESS FROM INSURT AUX IS NULLPTR" << std::endl;
        return nullptr;
    }
        

    return block_to_split->addr;

}


//the method will merge blocks which are free to one big block. 
//param: from_p  = the address to merge from, 
//       to_p = the address to last block which ned to be merged   
void Block_list::mergeBlock(Meta from_p , Meta to_p){
    if(!from_p || !to_p)//check
        std::cout << "[mergeBlock] merge function - the pointers are nullptr" << std::endl; //check
    size_t size_final_block = from_p->size;
    std::cout << "from_p size is : " << from_p->size << std::endl ;//check
    
    Meta curr = from_p->next_alloc_addr; 

    std::cout << "[mergeBlock] CURR->ADDR:  " << (size_t)curr->addr <<" addr_to_free = " << (size_t)to_p->addr << std::endl; //check
    std::cout << "[mergeBlock] BEFORE - num_allocted_blocks : " << num_allocated_blocks << std::endl; //check
    if(curr )//check
        std::cout << "[mergeBlock] True " << std::endl;//check

    if(!from_p->is_free){ //always check if from_p is the one to free , if it is - handle it before entring the while loop
        num_free_bytes += from_p->size;
        from_p->is_free = true;
    }
    
    while(curr != to_p->next_alloc_addr && curr ){
        std::cout << "[mergeBlock] i got insisde the loop" << std::endl; //check
        size_final_block += curr->size;
        size_final_block += METASIZE;
        num_allocated_bytes += METASIZE;
        num_free_bytes += METASIZE;
        std::cout << "b size is: " << curr->size << std::endl; 
        if(!curr->is_free)
            num_free_bytes += curr->size;
        curr->is_free = true;
        num_allocated_blocks--;
        curr = curr->next_alloc_addr;
    }
    from_p->size = size_final_block;  // the new size of the merged blocks
    std::cout << "[mergeBlock] AFTER - num_allocted_blocks : " << num_allocated_blocks << std::endl; //check
    // now i will handle the list and eject the merged block (not included the first one which we merge to)
    std::cout << "[mergeBlock] True 1" << std::endl;//check
    curr = from_p->next_alloc_addr;
    Meta tmp = nullptr;
    
    while(curr != to_p->next_alloc_addr && curr){    //Im adjesting the addr_list
        if(curr == last_alloc_Meta){
            (curr->prev_alloc_addr)->next_alloc_addr = nullptr;
            last_alloc_Meta = curr->prev_alloc_addr;
        } else{
            tmp = curr->next_alloc_addr;
            (curr->prev_alloc_addr)->next_alloc_addr = tmp;
            tmp->prev_alloc_addr = curr->prev_alloc_addr;
        }
        if(curr == tail){       //Im adjesting the block_list
            std::cout << " True 8-start - adjest the block list curr = tail" << std::endl;//check
            (curr->prev_by_size)->next_by_size = nullptr;
            std::cout << " True 8-end " << std::endl;//check
            tail = curr->prev_by_size;
            
        } else if(curr == head){
            curr->next_by_size->prev_by_size=nullptr;
            head = curr->next_by_size;
        } else{ //incase curr isn't tail meaning somewhere in the middle of the list
            tmp = curr->prev_by_size;
            (curr->next_by_size)->prev_by_size = tmp;
            tmp->next_by_size = curr->next_by_size;
        }
        curr = curr->next_alloc_addr;   
    }
    return;
}

// A new request has arrived, and no free memory chunk was found big enough,
// And the wilderness (last allocated block in the memmory) chunk is free, the:
// enlarge the wilderness chunk enough to store the new request.
void* Block_list::wilderness(size_t size){
    void* extra_space = sbrk(size - last_alloc_Meta->size);
    if(extra_space == (void *) (-1)){
        return nullptr;
    }
    num_allocated_bytes += size - last_alloc_Meta->size;
    num_free_bytes = 0;
    last_alloc_Meta->size = size;
    if(last_alloc_Meta->is_free)
        num_free_blocks--;

    return last_alloc_Meta->addr;
}

//Security megere to indicate a overflow attack, upperCookie and lowerCookie must have the same 
//cookie(code) as the global cookie we generated at the start of the program 
void Block_list::checkCookie(Meta block){
    if(!block)
        return;
    if(block->upper_cookie != cookie || block->lower_cookie != cookie)
        exit(0xdeadbeef);
    return;
}
/*----------------------------------------HW4 functions-----------------------------------------------------*/


void* smalloc(size_t size){
    if(size <= 0 || size> MAX_SIZE )
        return nullptr;
    Meta curr = nullptr;
    if(size >= MMAP_MIN_SIZE)
        curr = block_list.mmap_head;
    else
        curr = block_list.head ;
    void* program_break = nullptr;
    void* base=nullptr;
    base = sbrk(0);
    std::cout << "[smalloc] SBRK BEFORE ALLOCATION = " << (size_t)base << std::endl; //check
    
    while(curr)//looking in list for existing free allocated
    {
        block_list.checkCookie(curr);
        if(curr->is_free){
            if(curr->size == size){
                curr->is_free=false;
                block_list.num_free_blocks--;
                block_list.num_free_bytes -= curr->size;
                program_break = curr->addr;
                return program_break;
            }
            
            else if(curr->size > size){
                int64_t size_of_part2 = (curr->size)-(METASIZE+size);   
                if(size_of_part2 >= MIN_REMAINING){
                    std::cout << "[smalloc] splitting" << std::endl;//check
                    program_break = block_list.splitTheBlock(curr,size,(size_t)size_of_part2);
                    return program_break;
                }else{
                    curr->is_free=false;
                    block_list.num_free_blocks--;
                    block_list.num_free_bytes -= curr->size;
                    program_break = curr->addr;
                    return program_break;
                }
            }   
            
        }
        curr = curr->next_by_size; 
    }
    
    if(!curr)   //no free block that is big - allocate new
    {
        if(block_list.last_alloc_Meta && block_list.last_alloc_Meta->is_free){
            return block_list.wilderness(size);
        }
        std::cout << "[smalloc] no free block, allocate new"<<std::endl;
        if(size < MMAP_MIN_SIZE)
            program_break = block_list.insert(size);
        else
            program_break = block_list.insertMmap(size);
        size_t new_block = (size_t)program_break-METASIZE;
         std::cout << "[smalloc] THE ADDRESS OF THE NEW ALLOCATED BLOCK IS:"<<new_block<< std::endl;
        Meta new_block_meta = (Meta)((void*)(new_block)); //always remmeber the last address which is allocated
         std::cout << "[smalloc] THE SIZE OF THE NEWBLOCK IS:"<<new_block_meta->size<< std::endl;
    }
    base = sbrk(0); //check
    std::cout << "[smalloc] SBRK AFTER ALLOCATION "<<(size_t)base<< std::endl;
    return program_break;
}
//Searches for a free block of at least ‘num’ elements, each ‘size’ bytes that are all set to 0 or allocates if none are found.
// In other words, find/allocate size * num bytes and set all bytes to 0.
void* scalloc(size_t num, size_t size){
    void* program_break = smalloc(num * size);
    if(!program_break)
        return nullptr;
    memset(program_break, 0, num * size);
    return program_break;
}

//Releases the usage of the block that starts with the pointer ‘p’.
//If ‘p’ is NULL or already released, simply returns.
//Presume that all pointers ‘p’ truly points to the beginning of an allocated block.
void sfree(void* p){
    if(!p){
        std::cout << "[sfree] p is nullptr" << std::endl;
        return;
    }
        
    block_list.freeBlock(p);
}

//If ‘size’ is smaller than or equal to the current block’s size, reuses the same block.
// Otherwise, finds/allocates ‘size’ bytes for a new space, copies content of oldp into the new allocated space and frees the oldp.
void* srealloc(void* oldp, size_t size){
    if (size <= 0 || size > MAX_SIZE)
        return nullptr;
    void* addr_to_move_to = nullptr, *address = nullptr , *dest_addr=nullptr;
    if(!oldp){
        addr_to_move_to = smalloc(size);
        return addr_to_move_to;
    }
    Meta itr = (Meta)((void*)((size_t)oldp - METASIZE)) , dest_meta=nullptr;
    block_list.checkCookie(itr);
    size_t size_future_merge;
    //CASE A
    if(itr->size >= size){
        int64_t size_of_part2 = (itr->size)-(METASIZE+size);   
        if(size_of_part2 >= MIN_REMAINING){
            std::cout << "[smalloc - 627] splitting" << std::endl;//check
            address = block_list.splitTheBlock(itr,size,(size_t)size_of_part2);     
        }
        return oldp;
    }
        
    //CASE B
    if(itr->prev_alloc_addr && itr->prev_alloc_addr->is_free){
        std::cout << "[FreeBlock] Merge with next" << std::endl; //check
        size_future_merge = itr->size + itr->prev_alloc_addr->size;
        if(size_future_merge  >= size)
        {
            block_list.mergeBlock(itr->prev_alloc_addr,itr);
            dest_addr = itr->prev_alloc_addr->addr;
            address = std::memmove(dest_addr, oldp , size);
            dest_meta = (Meta)((size_t)address - METASIZE);
            int64_t size_of_part2 = (dest_meta->size)-(METASIZE+size);   
            if(size_of_part2 >= MIN_REMAINING){
                std::cout << "[smalloc - 627] splitting" << std::endl;//check
                address = block_list.splitTheBlock(dest_meta,size,(size_t)size_of_part2);
                
            }
            return address;
        }
        if(size_future_merge < size && block_list.last_alloc_Meta == itr){
            block_list.mergeBlock(itr->prev_alloc_addr,itr);
            address = block_list.wilderness(size); 
            return std::memmove(itr->addr, oldp , size);           
        }       
    }
    //CASE C 
    if(block_list.last_alloc_Meta == itr){
        address = block_list.wilderness(size);
        return address;
    }   
    //CASE D
    if(itr->next_alloc_addr && itr->next_alloc_addr->is_free){
        size_future_merge = itr->size + itr->next_alloc_addr->size;
        if(size_future_merge >= size){
            block_list.mergeBlock(itr,itr->next_alloc_addr);
            dest_addr = itr->next_alloc_addr->addr;
            address = std::memmove(dest_addr, oldp , size);
            dest_meta = (Meta)((size_t)address - METASIZE);
            int64_t size_of_part2 = (dest_meta->size)-(METASIZE+size);   
            if(size_of_part2 >= MIN_REMAINING){
                std::cout << "[smalloc - 652] splitting" << std::endl;//check
                address = block_list.splitTheBlock(dest_meta,size,(size_t)size_of_part2);
                
            }
            return address;
        }
    }
    //CASE E
    if((itr->prev_alloc_addr && itr->prev_alloc_addr->is_free) && (itr->next_alloc_addr && itr->next_alloc_addr->is_free)){
        size_future_merge = itr->size + itr->prev_alloc_addr->size + itr->next_alloc_addr->size;
        if(size_future_merge >= size){
            block_list.mergeBlock(itr->prev_alloc_addr,itr->next_alloc_addr);
            block_list.num_free_blocks--;
            dest_addr = itr->prev_alloc_addr->addr;
            address = std::memmove(dest_addr, oldp , size);
            return address;
        }
        if(size_future_merge < size && itr->next_alloc_addr == block_list.last_alloc_Meta){
            block_list.mergeBlock(itr->prev_alloc_addr,itr->next_alloc_addr);
            block_list.num_free_blocks--;
            dest_addr =  block_list.wilderness(size);
            return std::memmove(dest_addr, oldp , size);
        } 
    }
    //CASE F
    if(itr->next_alloc_addr && itr->next_alloc_addr->is_free){
        size_future_merge = itr->size + itr->next_alloc_addr->size;
        if(size_future_merge < size && (itr->next_alloc_addr == block_list.last_alloc_Meta)){
            block_list.mergeBlock(itr,itr->next_alloc_addr);
            dest_addr = block_list.wilderness(size);
            return std::memmove(dest_addr, oldp , size);
        }
    }

    //CASE G+H
    dest_addr = smalloc(size);
    address = std::memmove(dest_addr , oldp , size);
    sfree(oldp);
    return address;
}



