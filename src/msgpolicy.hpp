#ifndef _MSGPOLICY_H_
#define _MSGPOLICY_H_

#include "msgerrors.hpp"

namespace msg {

struct MsgPolicy {
  bool push(const msgblk_t &msg,error &err) {
      int result = push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg), 0);
      err.set(result);
      return result >= 0;
  }
  bool push_test(const msgblk_t &msg,error &err) {
      int result = push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg), 1);
      err.set(result);
      return result >= 0;
  }
  bool push(const msgblk_t &msg) {
      error err;
      int result = push(msg,err);
      if(err)
         throw exception(err);
      return result >= 0;
  }
  bool push_test(const msgblk_t &msg) {
      error err;
      int result = push_test(msg,err);
      if(err)
         throw exception(err);
      return result >= 0;
  }
  bool push(msgblk_t *msg,error &err) noexcept {
      int result = push_msg(mem, bank, msg, 0);
      err.set(result);
      return result >= 0;
  }
  bool push_test(msgblk_t *msg,error &err) {
      int result = push_msg(mem, bank, msg, 1);
      err.set(result);
      return result >= 0;
  }
  bool push(msgblk_t *msg) throw(exception) {
      error err;
      int result = push(msg,err);
      if(err)
         throw exception(err);
      return result >= 0;
  }
  bool push_test(msgblk_t *msg) {
      error err;
      int result = push_test(msg,err);
      if(err)
         throw exception(err);
      return result >= 0;
  }

  msgblk_t *pop() {
    return pop_msg(mem, bank, 0, NULL);
  }
  msgblk_t *pop_test() {
    return pop_msg(mem, bank, 1, NULL);
  }
  msgblk_t *prep() {
    error err;
    auto msg = prep(err);
    if (err) {
      throw exception(err);
    }
    return msg;
  }
  msgblk_t *prep(error &err) {
    int status;
    auto msg = prep_msg(mem, &status);
    if ( msg ) return msg;
    err.set(status);
    return NULL;
  }
  void free(msgblk_t *msg) { if(msg)free_msg(mem, msg); }

  MsgPolicy(shared_mem_t *mem, msg_bank_t *bank) :
    mem(mem), bank(bank) {}
private:
  shared_mem_t *mem;
  msg_bank_t *bank;
};

template <class M> struct trait;
template <> struct trait<msgblk_t> { typedef MsgPolicy type; };
}
#endif // _MSGPOLICY_H_

