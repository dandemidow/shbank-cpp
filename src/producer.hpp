#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include <memory>

#include "user_types.hpp"

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm {
struct producer {
  shared_mem_t *init(const std::string &name) {
    // FIXME put status instead NULL
    return open_shared_banks(const_cast<char*>(name.c_str()), NULL);
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
