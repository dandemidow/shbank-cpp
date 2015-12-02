#ifndef USER_TYPES
#define USER_TYPES

#include <string>

extern "C"{
    #include <shmobank/bank.h>
    #include <shmobank/monmsg.h>
}
#include "shared_memory.h"

enum class special_bank_tags{playback = 0,capture};

struct consumer{
    template<class TagType>
    static msg_bank_t* init_bank(shared_memory &mem,TagType tag,std::size_t){
        return join_msg_bank(mem.raw(),static_cast<int>(tag));
    }
    static msg_bank_t* init_bank(shared_memory &mem,special_bank_tags tag,std::size_t){
        if(tag == special_bank_tags::playback)
            return join_playback_msg_bank(mem.raw());
        if(tag == special_bank_tags::capture)
            return join_capture_msg_bank(mem.raw());
    }
    static void free_mem(shared_memory &mem,msg_bank_t *bank){
        unjoin_shared_banks(mem.raw());
    }
    static void connect(){

    }
};
struct producer{
    template<class TagType>
    static msg_bank_t* init_bank(shared_memory &mem,TagType tag,std::size_t size){
        return init_msg_bank(mem.raw(),size,static_cast<int>(tag));
    }
    static msg_bank_t* init_bank(shared_memory &mem,special_bank_tags tag,std::size_t size){
        if(tag == special_bank_tags::playback)
            return init_playback_msg_bank(mem.raw(),size);
        if(tag == special_bank_tags::capture)
            return init_capture_msg_bank(mem.raw(),size);
    }
    static void free_mem(shared_memory &mem,msg_bank_t *bank){
        wait_banks_unjoin(mem.raw());
        close_shared_banks(mem.raw());
    }
    static void connect(shared_memory &mem){
        wait_banks_join(mem.raw());
    }
};

#endif // USER_TYPES

