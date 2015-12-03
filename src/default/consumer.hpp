#ifndef _SHMOBANK_DEFAULT_CONSUMER_H_
#define _SHMOBANK_DEFAULT_CONSUMER_H_

#include "../msgpolicy.hpp"
#include "../consumer.hpp"
#include "bank.hpp"

namespace shmobank {
  using consumer = shared_memory<Consumer>;
}

#endif  // _SHMOBANK_DEFAULT_CONSUMER_H_
