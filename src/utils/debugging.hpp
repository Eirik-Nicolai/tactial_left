#pragma once

#define NAMESPACE "::"
constexpr auto ns = "::";
#define GET_BASE(_1)                                \
  public:                                           \
  virtual std::string get_name() {                  \
    return #_1;                                     \
  }                                                 \
    //return typeid(*this).name(); this includes the namespace


#define GET_NAME(_1, _2)                        \
  public:                                       \
  std::string get_name() override {             \
    std::stringstream ss;                       \
    ss << _1::get_name() << "::" << #_2;                   \
    return ss.str();                            \
  }

// #define GET_NAME_IMPL(_1, _2) GET_NAME_IMPL_(_1, ns ## _2)
