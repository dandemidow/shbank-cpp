#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include <memory>

#include "user_types.hpp"
#include "resolver.h"
#include "buferrors.hpp"

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm {
struct producer {
  typedef shared_mem_t*(producer::*InitType)(error&,const std::string&);
  typedef error ErrorType;
  typedef exception ExceptionType;
  typedef producer SelfType;

  shared_mem_t *init(error &err,const std::string &name) {
    int status;
    auto shm = open_shared_banks(const_cast<char*>(name.c_str()), &status);
    err.set(status);
    return shm;
  }
  shared_mem_t *init(const std::string &name) throw(ExceptionType)
  {
      return _add_exception::exc<InitType,SelfType,ErrorType,ExceptionType,decltype(name)>(this,&SelfType::init,name);
  }
  void exit(shared_mem_t *mem) { close_shared_banks(mem); }

  struct bank {
    static void defer(std::shared_ptr<shared_mem_t*> mem, msg_bank_t *b) {
      if ( *mem ) free_msg_bank(*mem, b);
    }

    static msg_bank_t *init(std::shared_ptr<shared_mem_t*> mem, int tag, int count = 2) {
      return init_msg_bank(*mem, count, tag);
    }
  };
};

template <class User> struct trait;
template <> struct trait<Producer> { typedef producer type; };
}
#endif  // _PRODUCER_H_
