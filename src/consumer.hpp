#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <memory>

#include "user_types.hpp"
#include "buferrors.hpp"
#include "exceptionizer.hpp"

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm {
  struct consumer {
    typedef shared_mem_t*(consumer::*InitType)(error&,const std::string&);
    typedef exception ExceptionType;

    shared_mem_t *init(error &err, const std::string &name) {
      int status;
      auto shm = join_to_shared_banks(const_cast<char*>(name.c_str()), &status);
      err.set(status);
      return shm;
    }
    shared_mem_t *init(const std::string &name) throw(ExceptionType)
    {
      return _add_exception::exc<InitType, ExceptionType>(this, &consumer::init, name);
    }

    void exit(shared_mem_t *mem) { unjoin_shared_banks(mem); }

    struct bank {
      static void defer(std::shared_ptr<shared_mem_t*> mem, msg_bank_t *b) {
        if ( *mem ) unjoin_msg_bank(b);
      }

      static msg_bank_t *init(std::shared_ptr<shared_mem_t*> mem, int tag, int count = 0) {
        (void)(count);
        return join_msg_bank(*mem.get(), tag);
      }
    };
  };

  template <class User> struct trait;
  template <> struct trait<Consumer> { typedef consumer type; };
}

#endif  // _CONSUMER_H_
