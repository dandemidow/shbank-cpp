#ifndef BUFERRORS
#define BUFERRORS

#include <sharedmem/buffer.h>

namespace shm {
class error {
  int val = BUF_SUCCESS;
  friend class exception;
public:
  enum error_type {
    success = BUF_SUCCESS,
    mmap = BUF_MMAP_ERR ,
    init_isem = BUF_INIT_ISEM_ERR,
    init_esem = BUF_INIT_ESEM_ERR,
    shm_open = BUF_SHM_OPEN_ERR,
    shm_trunc = BUF_SHM_TRUNC_ERR,
    exit_isem = BUF_EXIT_ISEM_ERR,
    exit_esem = BUF_EXIT_ESEM_ERR,
    shm_close = BUF_SHM_CLOSE_ERR,
    unmap = BUF_UNMMAP_ERR
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
        case error::mmap:
        return "buffer mmap error";
        case error::init_isem:
        return "buffer init isem error";
        case error::init_esem:
        return "buffer init esem error";
        case error::shm_open:
        return "buffer shm open error";
        case error::shm_trunc:
        return "buffer shm trunc error";
        case error::exit_isem:
        return "buffer exit isem error";
        case error::exit_esem:
        return "buffer exit esem error";
        case error::shm_close:
        return "buffer shm close error";
        case error::unmap:
        return "buffer unmap error";
        case error::success:
        return "success";
        default:
        return "unknown error";
    }
  }
};
}


#endif // BUFERRORS

