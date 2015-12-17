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
  const shared_mem_t *const mem;
  const msg_bank_t *const _bank;
  std::function<void(const shared_mem_t *const, msg_bank_t*)> at_exit = nullptr;
  typedef typename msg::trait<Message>::type msgpolicy;
  bank(const shared_mem_t *const _m, const msg_bank_t *const _b) :
    msgpolicy(_m, _b), mem(_m), _bank(_b) {}
public:
  bank(const bank &) = delete;
  bank(bank &&obj):
    bank(obj.mem, obj._bank)
  { at_exit = std::move(obj.at_exit); /*obj.at_exit = nullptr;*/  }
  ~bank() { if (at_exit) at_exit(mem, const_cast<msg_bank_t*>(_bank)); }
  template <class User>
  static bank create(const shared_mem_t *const mem, special_bank_tags tag, int count = 2, typename std::enable_if<std::is_same<User, Producer>::value>::type * = nullptr) {
    int _tag = tag == special_bank_tags::playback?Playback:Capture;
    const msg_bank_t *const _b = init_msg_bank(mem, count, _tag);
    if(!_b)
        throw(init_bank_exception());
    bank &&tmp = std::move(bank(mem, _b));
    tmp.at_exit = [](const shared_mem_t *const mem, msg_bank_t *b) { free_msg_bank(mem, b); };
    return std::move(tmp);
  }

  template <class User>
  static bank create(const shared_mem_t *const mem, special_bank_tags tag, int count = 0, typename std::enable_if<std::is_same<User, Consumer>::value>::type * = nullptr) {
    (void)(count);
    int _tag = tag == special_bank_tags::playback?Playback:Capture;
    msg_bank_t *_b = join_msg_bank(mem, _tag);
    if(!_b)
        throw(init_bank_exception());
    bank &&tmp = std::move(bank(mem, _b));
    tmp.at_exit = [](const shared_mem_t *const, msg_bank_t *b) { unjoin_msg_bank(b); };
    return std::move(tmp);
  }

  void activate() {
    active_msg_bank(const_cast<msg_bank_t *const>(_bank));
  }
  void deactivate(){
    deactive_msg_bank(const_cast<msg_bank_t *const>(_bank));
  }
  void wait_activating() {
    wait_bank_activate(_bank);
  }
  bool is_active() const {
    return is_active_bank(_bank);
  }

  bool wait_timeout_activating(int seconds){
      return wait_timeout_bank_activate(_bank,seconds) == 0;
  }
};

#endif // MESSAGE_BANK

