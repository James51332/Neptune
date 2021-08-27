#pragma once

#include <unordered_map>

namespace Neptune
{

template <typename T1, typename T2>
using HashMap = std::unordered_map<T1, T2>;

} // namespace Neptune
