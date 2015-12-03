#ifndef _SHMOBANK_DEFAULT_PRODUCER_H_
#define _SHMOBANK_DEFAULT_PRODUCER_H_

#include "../msgpolicy.hpp"
#include "../producer.hpp"
#include "bank.hpp"

namespace shmobank {
  using producer = shared_memory<Producer>;
}

#endif  // _SHMOBANK_DEFAULT_PRODUCER_H_
