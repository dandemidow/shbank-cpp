#ifndef _SHMOBANK_MSGERRORS_H_
#define _SHMOBANK_MSGERRORS_H_

#include <exception>
#include <map>

extern "C" {
#include <shmobank/monmsg.h>
}
#include <boost/system/error_code.hpp>
namespace msg {
  class error {
    int val = MSG_SUCESS;
    friend class exception;
  public:
    enum error_type{
        success = 0,
        not_active_bank = -1,
        null_ptr = -2,
        not_shared_ptr = -3,
        no_shared_mem = 4,
        no_queue = -5
    };
    error() = default;
    void set(int val) noexcept { this->val = val; }
    operator bool () {
      return val != MSG_SUCESS;
    }
    operator error_type(){
        return value();
    }
    error_type value()const{
        return static_cast<error_type>(val);
    }
  };

  class exception : public std::exception{
    int value = MSG_SUCESS;

  public:
    exception(const error &err) : value(err.val)
    {

    }
    virtual const char* what() const noexcept{
      error::error_type val = static_cast<error::error_type>(value);
      switch(val)
      {
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
      }
      return "unknown error";
    }
  };
}

#endif  // _SHMOBANK_MSGERRORS_H_
