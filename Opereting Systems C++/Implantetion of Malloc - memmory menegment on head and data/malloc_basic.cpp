#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#define MAX_SIZE 100000000

typedef struct MallocMetadata {
    size_t size;
    bool is_free;
    MallocMetadata* next_by_size;
    MallocMetadata* next_by_size;
    void* addr;
}*Meta;

typedef class Block_list {
    public:
        Meta head=nullptr;
        Meta tail=nullptr;
        size_t num_free_blocks = 0;
        size_t num_free_bytes = 0;
        size_t num_allocated_blocks = 0;
        size_t num_allocated_bytes = 0;

        void* insert(size_t size);
        void* checkIfAllocForNumElementsExist(size_t size);
        void freeBlock(void* addr);
        

}*Blist;

Blist block_list = (Blist)sbrk(sizeof(*block_list));

//Return the number of bytes of a single meta-data structure in your system.
size_t _size_meta_data(){
    return sizeof(MallocMetadata);
}

//Returns the number of allocated blocks in the heap that are currently free.
size_t _num_free_blocks(){
    return block_list->num_free_blocks;
}

//Returns the number of bytes in all allocated blocks in the heap that are currently free, excluding the bytes used by the meta-data structs.
size_t _num_free_bytes(){
    return block_list->num_free_bytes;
}

//Returns the overall (free and used) number of allocated blocks in the heap.
size_t _num_allocated_blocks(){
    return block_list->num_allocated_blocks;
}

//Returns the overall number (free and used) of allocated bytes in the heap, excluding the bytes used by the meta-data structs.
size_t _num_allocated_bytes(){
    return block_list->num_allocated_bytes;
}

//Returns the overall number of meta-data bytes currently in the heap.
size_t _num_meta_data_bytes(){
    return (block_list->num_allocated_blocks) * sizeof(MallocMetadata);
}

/*----------------------Block_list methods-----------------------*/

void* Block_list::insert(size_t size){
    Meta new_block = (Meta)sbrk(_size_meta_data()+size);
    if(new_block == (void*)(-1))
        return nullptr;

    //prepare the new node:
    new_block->is_free=false;
    new_block->next_by_size=nullptr;
    new_block->next_by_size=nullptr;
    new_block->size=size;
    new_block->addr = (void*)((size_t)new_block + _size_meta_data());


    Meta curr = block_list->head , tail = block_list->tail;
    //if block_list is empty:
    if(!curr){
        block_list->head=new_block;
        block_list->tail=new_block;
        block_list->num_allocated_blocks++;
        block_list->num_allocated_bytes+=size;

    }
    //we going to insert him at the tail of the blocklist
    else{
        tail->next_by_size=new_block;
        new_block->next_by_size=tail;
        block_list->tail=new_block;
        block_list->num_allocated_blocks++;
        block_list->num_allocated_bytes+= size;
    }
     
    return new_block->addr;
}

void* Block_list::checkIfAllocForNumElementsExist(size_t size){
    Meta curr = block_list->head;
    void* ptr_space = nullptr;
    while(curr){
        if(curr->size >= size && curr->is_free){
            ptr_space = curr->addr;
            return ptr_space;
        }
    curr = curr->next_by_size; 
    }
    return nullptr;
}

void Block_list::freeBlock(void* addr_to_free){
    Meta curr = block_list->head;
    while(curr){
        if(curr->addr == addr_to_free){
            curr->is_free=true;
            block_list->num_free_blocks++;
            block_list->num_free_bytes += curr->size;
            return;
        }
        curr = curr->next_by_size;
    }
}

/*----------------------------------------HW4 functions-----------------------------------------------------*/


void* smalloc(size_t size){
    if(size <= 0 || size> MAX_SIZE )
        return nullptr;
    //if blocklist isn't empty we need to iterate the blocklist and find free space with at least size
    Meta curr = block_list->head ;
    void* program_break = nullptr;
    while(curr){
        if(curr->size >= size && curr->is_free){
            curr->is_free=false;
            //curr->size=size;
            block_list->num_free_blocks--;
            block_list->num_free_bytes -= curr->size;
            program_break = curr->addr;
            return program_break;
        }
    curr = curr->next_by_size; 
    }
    //there is no allocated block which is free and big enghouh to contain the new block, so lets insert allocate new block and insert it
    if(!curr){
        program_break = block_list->insert(size);
    }
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
    if(!p)
        return;
    block_list->freeBlock(p);
}

//If ‘size’ is smaller than or equal to the current block’s size, reuses the same block.
// Otherwise, finds/allocates ‘size’ bytes for a new space, copies content of oldp into the new allocated space and frees the oldp.
void* srealloc(void* oldp, size_t size){
    if (size <= 0 || size > MAX_SIZE)
        return nullptr;
    void* addr_to_move_to = nullptr;
    if(!oldp){
        addr_to_move_to = smalloc(size);
        return addr_to_move_to;
    }
    Meta curr = block_list->head;
    while(curr){
        if(curr->addr == oldp){
            break;
        }
        curr = curr->next_by_size;
    }
    if(curr->size >= size)
        return oldp;
    addr_to_move_to = smalloc(size);
    if(addr_to_move_to && oldp)
        addr_to_move_to = memmove(addr_to_move_to, oldp , size);
    if(oldp)
        block_list->freeBlock(oldp);
    return addr_to_move_to;


}



