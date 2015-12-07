#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <string>

extern "C" {
#include <shmobank/monmsg.h>
}
class bad_memory_exception : public std::exception{
public:
  virtual const char* what() const _GLIBCXX_USE_NOEXCEPT{
    return "create shared memory";
  }
};

template <class User>
class shared_memory : shm::trait<User>::type {
  shared_mem_t *mem = nullptr;
  typedef typename shm::trait<User>::type policy;
public:
  shared_memory(const std::string &name):
    mem(policy::init(name))
  {
      if(mem == nullptr)
          throw(bad_memory_exception());
  }
  ~shared_memory() { policy::exit(mem); }
  shared_memory(const shared_memory &) = delete;
  shared_memory(const shared_memory &&) = delete;
  template <class Obj, class ...A>
  Obj create(A ...args) {
    return std::move(Obj::template create<User>(mem, args...));
  }
  void wait_join() {
    wait_banks_join(mem);
  }
  void wait_unjoin() {
    wait_banks_unjoin(mem);
  }
};
#endif  // _MEMORY_H_

