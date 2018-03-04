#pragma once

#include "lisp/defines.h"

// C++
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // Remove when we move LLVM to it's own project
#endif

#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <stack>
#include <queue>
#include <set>
#include <chrono>
#include <memory>
#include <utility>
#include <type_traits>
#include <functional>

#ifdef _MSC_VER
#pragma warning( pop )
#endif

// Vendor
#include <spdlog/spdlog.h>

// Deps
#include "util/all.h"
#include "types/core.h"
