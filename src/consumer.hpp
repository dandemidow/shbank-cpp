#ifndef _CONSUMER_H_
#define _CONSUMER_H_

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
};

template <class User> struct trait;
template <> struct trait<Consumer> { typedef consumer type; };
}

#endif  // _CONSUMER_H_
