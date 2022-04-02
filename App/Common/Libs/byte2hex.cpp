//
// Created by hubert25632 on 10.02.2022.
//

#include "byte2hex.hpp"

std::string byte2hex(uint8_t byte) {
  char s[3];
  snprintf(s, 3, "%02x", byte);
  return s; // Implicit casting
}
