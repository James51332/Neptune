#pragma once

#include <mutex>
#include <condition_variable>

namespace Neptune
{

using Mutex = ::std::mutex;

template <typename T>
using Guard = ::std::lock_guard<T>;

template <typename T>
using UniqueLock = ::std::unique_lock<T>;

using ConditionVariable = ::std::condition_variable;

} // namespace Neptune
