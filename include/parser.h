/*
 * パーサーで共通で使うstructやconceptを定義している。
 * meta programming のほとんどの実装は、やいとさん (@yaito3014)
 * が教えてくれたものである。 コメントは lemolatoon がつけた。
 */
#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <string>
#include <type_traits>

template <class T> struct ParserResult {
  T value;
  std::string_view remaining;
};

/*
 * TT: 任意個の型引数を取るクラステンプレートを受け取るテンプレート引数
 * T: TTに包まれているかどうか判定されるテンプレート型引数
 * 例:)
 * is_specialization_of<std::vector, std::optional<int>> : std::false_type
 * is_specialization_of<std::tuple, std::tuple<int, char>> : std::true_type
 *
 * note: `:` は継承のこと
 */
template <template <class...> class TT, class T>
struct is_specialization_of : std::false_type {};
template <template <class...> class TT, class... Ts>
struct is_specialization_of<TT, TT<Ts...>> : std::true_type {};

template <template <class...> class TT, class... Ts>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<TT, Ts...>::value;
static_assert(!is_specialization_of_v<std::optional, std::pair<int, char>>);
static_assert(is_specialization_of_v<std::tuple, std::tuple<int, char>>);

/*
 * テンプレートの部分特殊化を用いて、`ParserResult<T>` から `T` を取り出す。
 */
template <class PR> struct UnwrapParserResult {};
template <class T>
struct UnwrapParserResult<ParserResult<T>> : std::type_identity<T> {};

/*
 * パーサーの型制約
 * `T = void` のときには、パース結果の型については制約しない。
 *
 *
 *
 * 実装の解説:
 *
 * `std::conditional` ... 型の三項演算子
 *
 * std::conjunction` ... 'かつ' を表す
 *
 * `std::invocable<F, std::string_view>` ...
 * `std::string_view` を引数に取り、呼び出し可能
 *
 * std::specialization_of<std::optional,
 *                        std::invoke_result_t<F, std::string_view>
 *
 * ... 呼び出しの戻り値の型が `std::optional`
 * に包まれている
 *
 * is_specialization_of< ParserResult, typename
 * std::invoke_result_t<F, std::string_view>::value_type>>,
 *
 * ... 呼び出しの戻り値の型の `std::optional` の中身の型が `ParserResult`
 * に包まれている
 *
 * `std::conditional`の結果自体は型なので、`::value` で boolean
 * の値を取り出している。
 */
template <class F, class T = void>
concept Parser = std::conditional_t<
    std::is_void_v<T>,
    std::conjunction<
        std::is_invocable<F, std::string_view>,
        is_specialization_of<std::optional,
                             std::invoke_result_t<F, std::string_view>>,
        is_specialization_of<
            ParserResult,
            typename std::invoke_result_t<F, std::string_view>::value_type>>,
    std::is_invocable_r<std::optional<ParserResult<T>>, F,
                        std::string_view>>::value;

/*
 * concept Parser
 * を満たすテンプレート型引数を受け取り、そのパーサーの呼び出しの戻り値を返す。
 * つまり `Parser<T>` から `T` を取り出す。
 */
template <Parser F>
using ParserReturnType = UnwrapParserResult<
    typename std::invoke_result_t<F, std::string_view>::value_type>::type;

#endif // PARSER_H