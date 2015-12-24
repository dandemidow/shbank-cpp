#ifndef MESSAGE_BANK
#define MESSAGE_BANK

#include <functional>

#include "memory.hpp"

class init_bank_exception : public std::exception{
public:
  virtual const char* what() const _GLIBCXX_USE_NOEXCEPT{
    return "initialize message bank";
  }
};

template<class Message>
class bank : public msg::trait<Message>::type {
  std::shared_ptr<shared_mem_t*> mem;
  const msg_bank_t *const _bank;
  std::function<void(std::shared_ptr<shared_mem_t*>, msg_bank_t*)> at_exit = nullptr;
  typedef typename msg::trait<Message>::type msgpolicy;
  bank(std::shared_ptr<shared_mem_t*> _m, const msg_bank_t *const _b) :
    msgpolicy(_m, _b), mem(_m), _bank(_b) {}

public:
  bank(const bank &) = delete;
  bank(bank &&obj):
    bank(obj.mem, obj._bank)
  { at_exit = std::move(obj.at_exit); /*obj.at_exit = nullptr;*/  }
  ~bank() {
      if (at_exit)
          at_exit(mem, const_cast<msg_bank_t*>(_bank));
  }
  /*template <class User>
  static bank create(std::shared_ptr<shared_mem_t*>mem, special_bank_tags tag, int count = 2, typename std::enable_if<std::is_same<User, Producer>::value>::type * = nullptr) {
    using userbank = typename shm::trait<User>::type::bank;
    int _tag = tag == special_bank_tags::playback?Playback:Capture;
    const msg_bank_t *const _b = userbank::init(mem, _tag, count);
    if(!_b)
        throw(init_bank_exception());
    bank &&tmp = std::move(bank(mem, _b));
    tmp.at_exit = std::bind(userbank::defer, std::placeholders::_1, std::placeholders::_2);
    return std::move(tmp);
  }*/

  template <class User>
  static bank create(std::shared_ptr<shared_mem_t*> mem, special_bank_tags tag, int count = 2) {
    using userbank = typename shm::trait<User>::type::bank;
    int _tag = tag == special_bank_tags::playback?Playback:Capture;
    msg_bank_t *_b = shm::trait<User>::type::bank::init(mem, _tag, count);
    if(!_b)
        throw(init_bank_exception());
    bank &&tmp = std::move(bank(mem, _b));
    tmp.at_exit = std::bind(userbank::defer, std::placeholders::_1, std::placeholders::_2);
    return std::move(tmp);
  }

  void activate() throw(memory_deleted_exception){
    if(! *mem)
        throw(memory_deleted_exception());
    active_msg_bank(const_cast<msg_bank_t *const>(_bank));
  }
  void deactivate() throw(memory_deleted_exception){
      if(! *mem)
          throw(memory_deleted_exception());
    deactive_msg_bank(const_cast<msg_bank_t *const>(_bank));
  }
  void wait_activating() throw(memory_deleted_exception){
      if(! *mem)
          throw(memory_deleted_exception());
    wait_bank_activate(_bank);
  }
  bool is_active() const throw(memory_deleted_exception) {
      if(! *mem)
          throw(memory_deleted_exception());
    return is_active_bank(_bank);
  }

  bool wait_timeout_activating(int seconds) throw(memory_deleted_exception){
      if(! *mem)
          throw(memory_deleted_exception());
      return wait_timeout_bank_activate(_bank,seconds) == 0;
  }
};

#endif // MESSAGE_BANK

