#ifndef _MSGPOLICY_H_
#define _MSGPOLICY_H_
#include <memory>

#include "msgerrors.hpp"
#include "memory.hpp"
#include "exceptionizer.hpp"

namespace msg {

#define EXC_ALIAS(exception) \
  template<class F, class ...Arg> \
  auto _exc_alias(F &&f, Arg ...a) -> \
  decltype(_add_exception::exc<F,exception>(this, std::forward<F>(f), a...)) { \
    return _add_exception::exc<F,exception>(this, std::forward<F>(f), a...); \
  }

  struct MsgBasic {
    MsgBasic(std::shared_ptr<shared_mem_t*> mem, const msg_bank_t *const bank) :
      mem(mem), bank(bank) {}
  protected:
    std::shared_ptr<shared_mem_t*> mem;
    const msg_bank_t *const bank;
    typedef exception ExceptionType;
  };

  struct MsgPolicy : public MsgBasic {
    typedef bool(MsgPolicy::*PushType)(error &, const msgblk_t &);
    typedef msgblk_t(MsgPolicy::*PopType)(error&);


    // push
    bool push(error &err, const msgblk_t &msg) throw(memory_deleted_exception) {
      return push(err,msg,0);
    }
    bool push(const msgblk_t &msg) throw(exception,memory_deleted_exception) {
      return _exc_alias<PushType>(&MsgPolicy::push,msg);
    }

    bool push_test(error &err, const msgblk_t &msg) throw(memory_deleted_exception){
        return push(err,msg,1);
    }

    bool push_test(const msgblk_t &msg) throw(exception,memory_deleted_exception) {
      return  _exc_alias<PushType>(&MsgPolicy::push_test,msg);
    }

    // pop
    msgblk_t pop(error &err) throw(memory_deleted_exception) {
      return pop(err,0);
    }
    msgblk_t pop_test(error &err) throw(memory_deleted_exception) {
      return pop(err,1);
    }

    msgblk_t pop() throw(exception,memory_deleted_exception) {
      return  _exc_alias<PopType>(&MsgPolicy::pop);
    }

    msgblk_t pop_test() throw(exception,memory_deleted_exception){
      //return _add_exception::exc<PopType,ExceptionType>(this, &MsgPolicy::pop_test);
      return _exc_alias<PopType>(&MsgPolicy::pop_test);
    }

    MsgPolicy(std::shared_ptr<shared_mem_t*> mem, const msg_bank_t *const bank) : MsgBasic(mem, bank) {}

  private:
    msgblk_t pop(error& err,int num) throw(memory_deleted_exception)
    {
        if(! *mem)
            throw(memory_deleted_exception());
        int status;
        auto msg = pop_msg(*mem, bank, num, &status);
        err.set(status);
        if(!msg)
            return msgblk_t();
        auto obj = *msg;
        free_msg(*mem.get(), msg);
        return obj;
    }
    bool push(error &err,const msgblk_t &msg,int num)throw(memory_deleted_exception)
    {
        if(! *mem)
            throw(memory_deleted_exception());
        int result = push_msg_copy(*mem, bank, const_cast<msgblk_t*>(&msg), num);
        err.set(result);
        return result >= 0;
    }
    EXC_ALIAS(exception)
  };

  struct MsgPolicyRaw : public MsgBasic {
    typedef bool(MsgPolicyRaw::*PushType)(error &, msgblk_t *);
    typedef msgblk_t*(MsgPolicyRaw::*PopType)(error&);

    // push
    bool push(error &err, msgblk_t *msg) throw(memory_deleted_exception) {
        return push(err,msg,0);
    }

    bool push(msgblk_t *msg) throw(exception,memory_deleted_exception) {
      //return _add_exception::exc<PushType,ExceptionType>(this, &MsgPolicyRaw::push, msg);
      return _exc_alias<PushType>(&MsgPolicyRaw::push, msg);
    }

    bool push_test(error &err, msgblk_t *msg) throw(memory_deleted_exception){
      return push(err,msg,1);
    }

    bool push_test(msgblk_t *msg) throw(exception,memory_deleted_exception) {
      return _exc_alias<PushType>(&MsgPolicyRaw::push_test,msg);
    }

    // prep
    msgblk_t *prep(error &err) throw(memory_deleted_exception) {
        if(! *mem)
            throw(memory_deleted_exception());
      int status;
      auto msg = prep_msg(*mem, &status);
      err.set(status);
      return msg;
    }

    msgblk_t *prep()throw(exception,memory_deleted_exception){
      return _exc_alias<PopType>(&MsgPolicyRaw::prep);
    }

    // pop
    msgblk_t *pop(error &err) throw(memory_deleted_exception) {
        return pop(err,0);
    }
    msgblk_t *pop_test(error &err) throw(memory_deleted_exception) {
        return pop(err,1);
    }

    typedef msgblk_t*(MsgPolicy::*PopPtrType)(error&);

    msgblk_t *pop() throw(exception,memory_deleted_exception) {
      return _exc_alias<PopType>(&MsgPolicyRaw::pop);
    }

    msgblk_t *pop_test() throw(exception,memory_deleted_exception) {
      return _exc_alias<PopType>(&MsgPolicyRaw::pop_test);
    }

    void free(msgblk_t *msg) throw(memory_deleted_exception) {
        if(! *mem)
            throw(memory_deleted_exception());
      if ( msg ) free_msg(*mem, msg);
    }

    MsgPolicyRaw(std::shared_ptr<shared_mem_t*> mem, const msg_bank_t *const bank) : MsgBasic(mem, bank) {}
  private:
    msgblk_t *pop(error &err,int num)
    {
        if(! *mem)
            throw(memory_deleted_exception());
        int status;
        auto msg = pop_msg(*mem, bank, num, &status);
        err.set(status);
        return msg;
    }
    bool push(error &err, msgblk_t *msg,int num) throw(memory_deleted_exception){
        if(! *mem)
            throw(memory_deleted_exception());
      int result = push_msg(*mem, bank, msg, num);
      err.set(result);
      return result >= 0;
    }
    EXC_ALIAS(exception)
  };

  template <class M> struct trait;
  template <> struct trait<msgblk_t> { typedef MsgPolicy type; };
  template <> struct trait<msgblk_t*> { typedef MsgPolicyRaw type; };
}
#endif // _MSGPOLICY_H_

