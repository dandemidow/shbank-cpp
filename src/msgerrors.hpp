#ifndef _SHMOBANK_MSGERRORS_H_
#define _SHMOBANK_MSGERRORS_H_

#include <exception>
extern "C" {
#include <shmobank/monmsg.h>
}


namespace msg {
  class error {
    int value = MSG_SUCESS;
    friend class exception;
  public:
    error() = default;
    void set(int val) noexcept { this->value = val; }
    operator bool () {
      return value != MSG_SUCESS;
    }
  };

  class exception : public std::exception{
    int value = MSG_SUCESS;
  public:
    exception(const error &err) : value(err.value) {}
    virtual const char* what() const noexcept{
      return "msg error";
    }
  };
}

#endif  // _SHMOBANK_MSGERRORS_H_
