#ifndef _SHMOBANK_MSGERRORS_H_
#define _SHMOBANK_MSGERRORS_H_

#include <exception>
#include <map>

extern "C" {
#include <shmobank/monmsg.h>
}

namespace msg {
  class error {
    int val = MSG_SUCESS;
    friend class exception;
  public:
    enum error_type {
      success = MSG_SUCESS,
      not_active_bank = MSG_NOT_ACTIVE_BANK,
      null_ptr = MSG_NULL_PTR,
      not_shared_ptr = MSG_NOT_SHARED_PTR,
      no_shared_mem = MSG_NO_SHARED_MEM,
      no_queue = MSG_NO_QUEUE
    };
    error() = default;
    void set(int val) noexcept { this->val = val; }
    operator bool () {
      return val != error_type::success;
    }
    operator error_type() {
      return value();
    }
    error_type value() const {
      return static_cast<error_type>(val);
    }
  };

  class exception : public std::exception{
    int value = error::success;

  public:
    exception(const error &err) : value(err.val) {}
    virtual const char* what() const noexcept{
      error::error_type val = static_cast<error::error_type>(value);
      switch(val) {
        case error::not_active_bank:
        return "not active bank error";
        case error::null_ptr:
        return "null pointer error";
        case error::not_shared_ptr:
        return "not shared pointer error";
        case error::no_shared_mem:
        return "no shared memory error";
        case error::no_queue:
        return "no queue error";
        case error::success:
        return "success";
        default:
        return "unknown error";
      }
    }
  };
}

#endif  // _SHMOBANK_MSGERRORS_H_
