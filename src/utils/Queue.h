#pragma once

#include <concurrentQueue/concurrentqueue.h>

namespace Neptune
{

template <typename T>
using Queue = ::moodycamel::ConcurrentQueue<T>;

} // namespace Neptune
