#pragma once

#include <spdlog/fmt/bundled/format.h>
#include "components/components.hpp"
#include "utils/debugging.hpp"
// template <typename T>
// concept HasName = requires { T::get_name; };

// template <typename C>
// class fmt::formatter<C, std::is_base_of<Formattable>> {
//   // format specification storage
//   char presentation_ = 'p';
// public:
//   // parse format specification and store it:
//   constexpr auto parse (format_parse_context& ctx) {
//       auto i = ctx.begin(), end = ctx.end();
//       if (i != end && (*i == 'f' || *i == 'e')) {
//           presentation_ = *i++;
//       }
//       if (i != end && *i != '}') {
//           throw format_error("invalid format");
//       }
//       return i;
//   }
//   // format a value using stored specification:
//   template <typename FmtContext>
//   constexpr auto format (std::vector<C> const& v, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
//       switch (presentation_) {
//         default:
//           // 'ctx.out()' is an output iterator
//         case 'f': {
//           std::string s = " ";
//           for(auto &element : v) {
//             s += std::to_string(element);
//             s += " ";
//           }
//           return format_to(ctx.out(), "[{}]", s);
//         }
//         case 'e': {
//           std::string s = " ";
//           for(auto &element : v) {
//             s += std::to_string(element);
//             s += " ";
//           }
//           return format_to(ctx.out(), "[{}]", s);
//         }
//       }
//   }
// };


template <typename C>
class fmt::formatter<std::vector<C>> {
  // format specification storage
  char presentation_ = 'p';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
      auto i = ctx.begin(), end = ctx.end();
      if (i != end && (*i == 'f' || *i == 'e')) {
          presentation_ = *i++;
      }
      if (i != end && *i != '}') {
          throw format_error("invalid format");
      }
      return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (std::vector<C> const& v, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
      switch (presentation_) {
        default:
          // 'ctx.out()' is an output iterator
        case 'f': {
          std::string s = " ";
          for(auto &element : v) {
            s += std::to_string(element);
            s += " ";
          }
          return format_to(ctx.out(), "[{}]", s);
        }
        case 'e': {
          std::string s = " ";
          for(auto &element : v) {
            s += std::to_string(element);
            s += " ";
          }
          return format_to(ctx.out(), "[{}]", s);
        }
      }
  }
};


template <>
class fmt::formatter<Pos> {
  // format specification storage
  char presentation_ = 'p';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
      auto i = ctx.begin(), end = ctx.end();
      if (i != end && (*i == 'f' || *i == 'e')) {
          presentation_ = *i++;
      }
      if (i != end && *i != '}') {
          throw format_error("invalid format");
      }
      return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (Pos const& p, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
      switch (presentation_) {
          default:
              // 'ctx.out()' is an output iterator
          case 'f': return format_to(ctx.out(), "({}, {})", p.x, p.y);
          case 'e': return format_to(ctx.out(), "({:e}, {:e})", p.x, p.y);
      }
  }
};



template <>
class fmt::formatter<olc::vf2d> {
  // format specification storage
  char presentation_ = 'p';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
      auto i = ctx.begin(), end = ctx.end();
      if (i != end && (*i == 'f' || *i == 'e')) {
          presentation_ = *i++;
      }
      if (i != end && *i != '}') {
          throw format_error("invalid format");
      }
      return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (olc::vf2d const& v, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
      switch (presentation_) {
          default:
              // 'ctx.out()' is an output iterator
          case 'f': return format_to(ctx.out(), "({:f}, {:f})", v.x, v.y);
          case 'e': return format_to(ctx.out(), "({:e}, {:e})", v.x, v.y);
      }
  }
};

template <>
class fmt::formatter<olc::vi2d> {
  // format specification storage
  char presentation_ = 'p';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
      auto i = ctx.begin(), end = ctx.end();
      if (i != end && (*i == 'f' || *i == 'e')) {
          presentation_ = *i++;
      }
      if (i != end && *i != '}') {
          throw format_error("invalid format");
      }
      return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (olc::vi2d const& v, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
      switch (presentation_) {
          default:
              // 'ctx.out()' is an output iterator
          case 'f': return format_to(ctx.out(), "({:f}, {:f})", v.x, v.y);
          case 'e': return format_to(ctx.out(), "({:e}, {:e})", v.x, v.y);
      }
  }
};
