#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <cstdint>
#include <string>

namespace Debugging {
struct DebugName{
  DebugName() {};
  DebugName(std::string s) : name(s)
  { id=std::hash<std::string>{}(s); }

  std::string name;
  uint32_t id;
};
};


#endif // DEBUGGING_H_
