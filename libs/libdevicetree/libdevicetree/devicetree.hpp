#pragma once

#include <iosfwd>
#include <string>

#include <libdevicetree/export.hpp>

namespace devicetree
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  LIBDEVICETREE_SYMEXPORT void
  say_hello (std::ostream&, const std::string& name);
}
