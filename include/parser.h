#include <string>
#include <type_traits>
#include <optional>

template <class F, class T>
concept Parser = std::is_invocable_r_v<std::optional<std::tuple<T, std::string_view>>, F, std::string_view>;