#ifndef SHARED_MEMORY
#define SHARED_MEMORY
#include <string>
#include <cstring>

extern "C" {
#include <shmobank/monmsg.h>
}

class shared_memory{
    shared_mem_t *mem;
    enum {mem_size = SHM_MEM_SIZE};
    enum{empty,joined,opened}state = empty;
public:
    shared_memory() = default;
    void join(std::string name){
        if(state == empty)
        {
            state = joined;
            mem = join_to_shared_banks(const_cast<char*>(name.c_str()));
        }
    }
    void open(std::string name){
        if(state == empty)
        {
            state = opened;
            mem = open_shared_banks(const_cast<char*>(name.c_str()));
        }
    }
    shared_mem_t *raw()const{
        return mem;
    }
    ~shared_memory(){
        close_shared_banks(mem);
    }
};
#endif // SHARED_MEMORY

