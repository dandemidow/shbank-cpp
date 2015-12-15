#ifndef _MSGPOLICY_H_
#define _MSGPOLICY_H_

#include "msgerrors.hpp"

namespace msg {

  struct _add_exception {
    template <class T, class UB>
    struct resolver {
      static void exc(...);
    };

    template <class T, class Ret>
    struct resolver <T, Ret(T::*)(error&)> {
      typedef Ret(T::*F)(error&);

      static Ret exc(T *obj, F f) throw(exception){
        error err;
        auto result = (obj->*f)(err);
        if(err)
            throw(exception(err));
        return result;
      }
    };

    template <class T, class Ret, class ...A>
    struct resolver <T, Ret(T::*)(error&, A...)> {
      typedef Ret(T::*F)(error&, A...);
      static Ret exc(T *obj, F f, A... arg) throw(exception){
        error err;
        auto result = (obj->*f)(err, arg...);
        if(err)
            throw(exception(err));
        return result;
      }
    };

    template<class M, class T, class ...A>
    static auto exc(T *obj, M m, A ...a) throw(exception) -> decltype(resolver<T, M>::exc(obj, m, a...))  {
      return resolver<T, M>::exc(obj, m, a...);
    }
  };

  struct MsgBasic {
    MsgBasic(shared_mem_t *mem, msg_bank_t *bank) :
      mem(mem), bank(bank) {}
  protected:
    shared_mem_t *mem;
    msg_bank_t *bank;
    bool check(error &err)
    {
        bool res = mem != nullptr && bank != nullptr;
        err.set(res ? error::success : error::bank_not_init);
        return res;
    }
  };

  struct MsgPolicy : public MsgBasic {
    typedef bool(MsgPolicy::*PushType)(error &, const msgblk_t &);
    typedef msgblk_t(MsgPolicy::*PopType)(error&);

    // push
    bool push(error &err, const msgblk_t &msg) noexcept {
      return push(err,msg,0);
    }
    bool push(const msgblk_t &msg) throw(exception) {
      return _add_exception::exc<PushType>(this, &MsgPolicy::push, msg);
    }

    bool push_test(error &err, const msgblk_t &msg) noexcept{
        return push(err,msg,1);
    }

    bool push_test(const msgblk_t &msg) {
      return _add_exception::exc<PushType>(this, &MsgPolicy::push_test, msg);
    }

    // pop
    msgblk_t pop(error &err) noexcept {
      return pop(err,0);
    }
    msgblk_t pop_test(error &err) noexcept {
      return pop(err,1);
    }

    msgblk_t pop() throw(exception) {
      return _add_exception::exc<PopType>(this, &MsgPolicy::pop);
    }

    msgblk_t pop_test() {
      return _add_exception::exc<PopType>(this, &MsgPolicy::pop_test);
    }

    MsgPolicy(shared_mem_t *mem, msg_bank_t *bank) : MsgBasic(mem, bank) {}

  private:
    msgblk_t pop(error& err,int num)
    {
        auto res = check(err);
        if(!res)return msgblk_t();
        int status;
        auto msg = pop_msg(mem, bank, num, &status);
        err.set(status);
        if(msg == nullptr)
            return msgblk_t();
        auto obj = *msg;
        free_msg(mem, msg);
        return obj;
    }
    bool push(error &err,const msgblk_t &msg,int num)
    {
        auto res = check(err);
        if(!res)return false;
        int result = push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg), num);
        err.set(result);
        return result >= 0;
    }
  };

  struct MsgPolicyRaw : public MsgBasic {
    typedef bool(MsgPolicyRaw::*PushType)(error &, msgblk_t *);
    typedef msgblk_t*(MsgPolicyRaw::*PopType)(error&);

    // push
    bool push(error &err, msgblk_t *msg) noexcept {
        return push(err,msg,0);
    }
    bool push(msgblk_t *msg) throw(exception) {
      return _add_exception::exc<PushType>(this, &MsgPolicyRaw::push, msg);
    }

    bool push_test(error &err, msgblk_t *msg) noexcept{
      return push(err,msg,1);
    }

    bool push_test(msgblk_t *msg) {
      return _add_exception::exc<PushType>(this, &MsgPolicyRaw::push_test, msg);
    }

    // prep
    msgblk_t *prep(error &err) noexcept {
      auto res = check(err);
      if(!res)return nullptr;
      int status;
      auto msg = prep_msg(mem, &status);
      err.set(status);
      return msg;
    }

    msgblk_t *prep() throw(exception) {
      return _add_exception::exc<PopType>(this, &MsgPolicyRaw::prep);
    }

    // pop
    msgblk_t *pop(error &err) noexcept {
        return pop(err,0);
    }
    msgblk_t *pop_test(error &err) noexcept {
        return pop(err,1);
    }

    typedef msgblk_t*(MsgPolicy::*PopPtrType)(error&);

    msgblk_t *pop() throw(exception) {
      return _add_exception::exc<PopType>(this, &MsgPolicyRaw::pop);
    }

    msgblk_t *pop_test() {
      return _add_exception::exc<PopType>(this, &MsgPolicyRaw::pop_test);
    }

    void free(msgblk_t *msg) noexcept {
      if ( msg ) free_msg(mem, msg);
    }

    MsgPolicyRaw(shared_mem_t *mem, msg_bank_t *bank) : MsgBasic(mem, bank) {}
  private:
    msgblk_t *pop(error &err,int num)
    {
        auto res = check(err);
        if(!res)return nullptr;
        int status;
        auto msg = pop_msg(mem, bank, num, &status);
        err.set(status);
        return msg;
    }
    bool push(error &err, msgblk_t *msg,int num) noexcept{
      auto res = check(err);
      if(!res)return false;
      int result = push_msg(mem, bank, msg, num);
      err.set(result);
      return result >= 0;
    }

  };

  template <class M> struct trait;
  template <> struct trait<msgblk_t> { typedef MsgPolicy type; };
  template <> struct trait<msgblk_t*> { typedef MsgPolicyRaw type; };
}
#endif // _MSGPOLICY_H_

