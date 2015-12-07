#ifndef _MSGPOLICY_H_
#define _MSGPOLICY_H_

#include "msgerrors.hpp"

namespace msg {

  struct MsgPolicy {
    // push
    bool push(const msgblk_t &msg, error &err) noexcept {
      int result = push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg), 0);
      err.set(result);
      return result >= 0;
    }

    bool push(const msgblk_t &msg) throw(exception) {
      return _msg_arg_exception<decltype(msg)>(&MsgPolicy::push, msg);
    }

    bool push(msgblk_t *msg, error &err) noexcept {
      int result = push_msg(mem, bank, msg, 0);
      err.set(result);
      return result >= 0;
    }

    bool push(msgblk_t *msg) throw(exception) {
      return _msg_arg_exception<decltype(msg)>(&MsgPolicy::push, msg);
    }

    bool push_test(const msgblk_t &msg, error &err) {
      int result = push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg), 1);
      err.set(result);
      return result >= 0;
    }

    bool push_test(const msgblk_t &msg) {
        return _msg_arg_exception<decltype(msg)>(&MsgPolicy::push_test, msg);
    }

    bool push_test(msgblk_t *msg, error &err) {
      int result = push_msg(mem, bank, msg, 1);
      err.set(result);
      return result >= 0;
    }

    bool push_test(msgblk_t *msg) {
      return _msg_arg_exception<decltype(msg)>(&MsgPolicy::push_test, msg);
    }

    // pop
    msgblk_t *pop(error &err) noexcept {
      int status;
      auto msg = pop_msg(mem, bank, 0, &status);
      err.set(status);
      return msg;
    }
    msgblk_t *pop_test(error &err) noexcept {
      int status;
      auto msg = pop_msg(mem, bank, 1, &status);
      err.set(status);
      return msg;
    }

    msgblk_t *pop() throw(exception) {
      return _msg_void_exception(&MsgPolicy::pop);
    }

    msgblk_t *pop_test() {
      return _msg_void_exception(&MsgPolicy::pop_test);
    }

    //prep
    msgblk_t *prep(error &err) noexcept {
      int status;
      auto msg = prep_msg(mem, &status);
      err.set(status);
      return msg;
    }

    msgblk_t *prep() throw(exception) {
      return _msg_void_exception(&MsgPolicy::prep);
    }

    void free(msgblk_t *msg) noexcept {
      if ( msg ) free_msg(mem, msg);
    }

    MsgPolicy(shared_mem_t *mem, msg_bank_t *bank) :
      mem(mem), bank(bank) {}
  private:
    shared_mem_t *mem;
    msg_bank_t *bank;

    template <class MSG, class F>
    F _msg_arg_exception(F (MsgPolicy::*f)(MSG, error&), MSG msg) throw(exception) {
      error err;
      auto result = (this->*f)(msg, err);
      if ( err ) throw exception(err);
      return result;
    }

    template <class F>
    F _msg_void_exception(F (MsgPolicy::*f)(error&)) throw(exception) {
      error err;
      auto result = (this->*f)(err);
      if ( err ) throw exception(err);
      return result;
    }

  };

  template <class M> struct trait;
  template <> struct trait<msgblk_t> { typedef MsgPolicy type; };
}
#endif // _MSGPOLICY_H_

