#ifndef _MSGPOLICY_H_
#define _MSGPOLICY_H_

extern "C" {
#include <shmobank/monmsg.h>
}

namespace msg {
struct MsgPolicy {
  bool push(const msgblk_t &msg) { return push_msg_copy(mem, bank, const_cast<msgblk_t*>(&msg))>=0; }
  bool push(msgblk_t *msg) { return push_msg(mem, bank, msg)>=0; }
  msgblk_t *pop() { return pop_msg(mem, bank); }
  msgblk_t *prep() { return prep_msg(mem); }
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

