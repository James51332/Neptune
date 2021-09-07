#pragma once

#include <mutex>

namespace Neptune
{

using Mutex = ::std::mutex;

template <typename T>
using Guard = ::std::lock_guard<T>;

} // namespace Neptune
