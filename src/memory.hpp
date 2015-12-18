#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <string>
#include <memory>
#include <iostream>

extern "C" {
#include <shmobank/monmsg.h>
}
namespace shm{
    template <class User> struct trait;
}
class bad_memory_exception : public std::exception{
public:
  virtual const char* what() const noexcept{
    return "create shared memory";
  }
};
class memory_deleted_exception : public std::exception{
public:
  virtual const char* what() const noexcept{
    return "memory already delete";
  }
};

template <class User>
class shared_memory : shm::trait<User>::type {
  std::shared_ptr<shared_mem_t*>mem;
  typedef typename shm::trait<User>::type policy;
public:
  shared_memory(const std::string &name):
    mem(std::make_shared<shared_mem_t*>(policy::init(name))) {
      if(*mem.get() == nullptr)
          throw(bad_memory_exception());
  }
  ~shared_memory()
  {
      policy::exit(const_cast<shared_mem_t*>(*mem.get()));
      *mem.get() = nullptr;
  }
  shared_memory(const shared_memory &) = delete;
  shared_memory(const shared_memory &&) = delete;
  template <class Obj, class ...A>
  Obj create(A ...args) {
    return std::move(Obj::template create<User>(mem, args...));
  }
  void wait_join() {
    wait_banks_join(*mem.get());
  }
  void wait_unjoin() {
    wait_banks_unjoin(*mem.get());
  }
  gid_t pid() const {
    return get_master_pid(*mem.get());
  }
};
#endif  // _MEMORY_H_

