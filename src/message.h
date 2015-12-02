#ifndef MESSAGE
#define MESSAGE

#include "shmobank/msg.h"

#include <malloc.h>
#include <iostream>
#include <cstring>

class message{

public:
    message() = default;
    message(msgblk_t *msg){
        this->msg = (msgblk_t*)malloc(sizeof(*msg));
        std::memcpy(this->msg,msg,sizeof(*msg));
    }

    static msgblk_t* transform(const message &msg){
          msgblk_t* _msg = (msgblk_t*)malloc(sizeof(*msg.msg));
          std::memcpy(_msg,msg.msg,sizeof(*msg.msg));
          return _msg;
    }
    static message transform(msgblk_t *msg){
        message res;
        res.msg = (msgblk_t*)malloc(sizeof(*msg));
        std::memcpy(res.msg,msg,sizeof(*msg));
    }
private:

    msgblk_t *msg;
};

#endif // MESSAGE

