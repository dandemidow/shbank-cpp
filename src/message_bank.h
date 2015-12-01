#ifndef MESSAGE_BANK
#define MESSAGE_BANK

#include "user_types.h"
#include "shared_memory.h"

class init_bank_exception : public std::exception{
public:
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT{
        return "initialize message bank";
    }
};
template<class User,class Message>
class message_bank{

    shared_memory &mem;
    msg_bank_t *bank;
    enum {shm_size = SHM_MEM_SIZE};

public:
    typedef Message message_type;
    typedef User user_type;

    template<class TagType>
    message_bank(shared_memory &mem,TagType tag,std::size_t size = 2) : mem(mem){
        bank = user_type::init_bank(mem,tag,size);
    }
    void wait_connect(){
        user_type::connect(mem);
    }
    bool is_active()const{
        return is_active_bank(bank);
    }
    void activate(){
        active_msg_bank(bank);
    }
    void deactivate(){
        deactive_msg_bank(bank);
    }
    message_type pop_message(){
        msgblk_t *msg = pop_msg(mem.raw(), bank);
        auto result = message_type::transform(msg);
        free_msg(mem.raw(), msg);
        return result;
    }
    void push_message(const message_type &message){
        msgblk_t *msg = prep_msg(mem.raw());
        msg = message_type::transform(message);
        push_msg(mem.raw(),bank, msg);
    }
    void wait_activate(){
        wait_bank_activate(bank);
    }
    ~message_bank(){
        user_type::free_mem(mem,bank);
    }
};

#endif // MESSAGE_BANK

