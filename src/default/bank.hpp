#ifndef _SHMOBANK_DEFAULT_BANK_H_
#define _SHMOBANK_DEFAULT_BANK_H_

#include "../bank.hpp"

namespace shmobank {
  using bank = bank<msgblk_t>;
  using tags = special_bank_tags;
}

#endif  // _SHMOBANK_DEFAULT_BANK_H_
