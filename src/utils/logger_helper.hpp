#pragma once

#include <spdlog/fmt/bundled/format.h>
#include "components/components.hpp"


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
          case 'f': return format_to(ctx.out(), "({:f}, {:f})", p.coordinates.x, p.coordinates.y);
          case 'e': return format_to(ctx.out(), "({:e}, {:e})", p.coordinates.x, p.coordinates.y);
      }
  }
};


template <>
class fmt::formatter<SizeCirc> {
  // format specification storage
  char presentation_ = 's';
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
  constexpr auto format (SizeCirc const& s, FmtContext& ctx) const {    // note: we can't use ternary operator '?:' in a constexpr
      switch (presentation_) {
          default:
              // 'ctx.out()' is an output iterator
          case 'f': return format_to(ctx.out(), "({:f})", s.r);
          case 'e': return format_to(ctx.out(), "({:e})", s.r);
      }
  }
};
