#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <memory>

#include "user_types.hpp"

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm {
struct consumer {
  shared_mem_t *init(const std::string &name) {
    // FIXME put status instead NULL
    return join_to_shared_banks(const_cast<char*>(name.c_str()), NULL);
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
