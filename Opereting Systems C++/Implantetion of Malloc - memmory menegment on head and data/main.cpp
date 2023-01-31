#include <iostream>
#include "malloc_3.h"
#include <cmath>
#define PAGE_SIZE 131072
#define MIN_SPLIT_SIZE 128
#define MAX_ALLOCATION_SIZE (1e8)
#define MMAP_THRESHOLD (128 * 1024)

static inline size_t aligned_size(size_t size)
{
    return size;
}
template <typename T>
void populate_array(T *array, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        array[i] = (T)i;
    }
}



void print_stats(){
    std::cout <<"-----------------------------------" <<std::endl;
    std::cout <<"num of free block : "<<  _num_free_blocks()<< std::endl;
    std::cout <<"num of free byte (without meta): "<<  _num_free_bytes()<< std::endl;
    std::cout <<"num of allocate blocks : "<<  _num_allocated_blocks()<< std::endl;
    std::cout <<"num of allocate byte (without meta): "<<  _num_allocated_bytes()<< std::endl;
    std::cout <<"num of meta data bytes : "<<  _num_meta_data_bytes()<< std::endl;
    std::cout <<"size of one meta in bytes : "<<  _size_meta_data()<< std::endl;
    std::cout <<"-----------------------------------" <<std::endl;
}

int main() {
    // test failure
    if (smalloc(0) != nullptr)
    {
        std::cout << "[FAILED] allocated 0 bytes" << std::endl;
    }
    if (smalloc(pow(10,8) + 1) != nullptr){
        std::cout << "[FAILED] allocated 10^8 bytes" << std::endl;
    }
/*
    char *a = (char *)smalloc(MMAP_THRESHOLD);
    print_stats();
    char *b = (char *)smalloc(MMAP_THRESHOLD - 8);
    print_stats();
    sfree(a);
    print_stats();
    sfree(b);
    print_stats();
    */

    char *a = (char *)smalloc(32);
    print_stats();
    sfree(a);
    print_stats();
    char *b = (char *)smalloc(16);
    if(a != b)
        std::cout << "a != b" << std::endl;
    print_stats();
    char *c = (char *)srealloc(b, 32);
    if(c != b)
        std::cout << "c != b" << std::endl;
    print_stats();
    char *d = (char *)srealloc(c, 32);
    if(d != c)
        std::cout << "d != c" << std::endl;
    print_stats();  
    sfree(c);
    print_stats();

   /*----------------------------------------------------*/
   /*
    char *a = (char *)smalloc(32 + MIN_SPLIT_SIZE + _size_meta_data());
    print_stats();
    populate_array(a, 32 + MIN_SPLIT_SIZE + _size_meta_data());
    char *b = (char *)srealloc(a, 32);
    print_stats();
    populate_array(b, 32);
    print_stats();
    */

   /*-----------------------------------------------*/
    //char *c = (char *)srealloc(b, 32);
    //print_stats();

    //print_stats();
    //char *d = (char *)srealloc(c, 32);
    //print_stats();

    //sfree(b);
    //print_stats();
    /*
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
    }
    for (int i = 0; i < 10; i++)
    {
        if(a[i] != i)
            std::cout << "problem in " << i << " cell" << std::endl;
    }
    int *b = (int *)srealloc(a, aligned_size(100 * sizeof(int)));
    print_stats();
    for (int i = 0; i < 10; i++)
    {
        if(b[i] != i)
            std::cout << "problem in " << i << "cell" << std::endl;
    }

    sfree(b);
    print_stats();
    
    /*
    void* b = smalloc(1000);
    void* c = smalloc(1000);
    sfree(a);
    sfree(c);
    print_stats();
    srealloc(b, 2100);

    print_stats();
    std::cout << c  << " " << a << std::endl;
    */



    return 0;
}