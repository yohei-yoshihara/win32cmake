#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <iosfwd>
#include <memory>

#include "Resource.h"

#include <spdlog/spdlog.h>
#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

namespace win32cmake {
  extern std::shared_ptr<spdlog::logger> logger;
}

#endif