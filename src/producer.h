#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include "user_types.h"

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm {
struct producer {
  shared_mem_t *init(const std::string &name) { return open_shared_banks(const_cast<char*>(name.c_str())); }
  void exit(shared_mem_t *mem) { close_shared_banks(mem); }
};

template <class User> struct trait;
template <> struct trait<Producer> { typedef producer type; };
}
#endif  // _PRODUCER_H_
