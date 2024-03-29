#pragma once

#include <tuple>
#include <type_traits>

namespace naive {

// TODO: enable_if stuff, noexcept, constexpr

template <typename T, T... Ints>
struct IntegerSequence
{
    using ValueType = T;

    static constexpr size_t Size() noexcept
    { return sizeof...(Ints); }
};

template<size_t... Ints>
using IndexSequence = IntegerSequence<size_t, Ints...>;

template<typename T, size_t N, T... Ints>
struct _MakeIntegerSequence
{
    using Type = typename _MakeIntegerSequence<T, N - 1, Ints..., sizeof...(Ints)>::Type;
};

template<typename T, T... Ints>
struct _MakeIntegerSequence<T, 0, Ints...>
{
    using Type = IntegerSequence<T, Ints...>;
};

template<typename T, T N>
using MakeIntegerSequence = typename _MakeIntegerSequence<T, N>::Type;

template<size_t N>
using MakeIndexSequence = MakeIntegerSequence<size_t, N>;

template<class... T>
using IndexSequenceFor = MakeIndexSequence<sizeof...(T)>;

struct PiecewiseConstructT
{
    explicit PiecewiseConstructT() = default;
};

template <typename T1, typename T2>
class Pair
{
public:
    Pair() = default;
    Pair(const Pair& p) = default;
    Pair(Pair&& p) = default;

    Pair(const T1 & x, const T2 & y) :
        first(x),
        second(y)
    { }

    template<typename U1, typename U2>
    Pair(U1&& x, U2&& y) :
        first(std::forward<U1>(x)),
        second(std::forward<U2>(y))
    { }

    template<typename U1, typename U2>
    Pair(const Pair<U1, U2>& p) :
        first(p.first),
        second(p.second)
    { }

    template<typename U1, typename U2>
    Pair(Pair<U1, U2>&& p) :
        first(std::forward<U1>(p.first)),
        second(std::forward<U2>(p.second))
    { }

    template< class... Args1, class... Args2 >
    Pair(PiecewiseConstructT, std::tuple<Args1...> first_args, std::tuple<Args2...> second_args);

    Pair& operator=(const Pair& other)
    {
        if (&other != this) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    template<typename U1, typename U2>
    Pair& operator=(const Pair<U1, U2>& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    Pair& operator=(Pair&& other) /*noexcept*/
    {
        if (&other != this) {
            first = std::forward<T1>(other.first);
            second = std::forward<T2>(other.second);
        }
        return *this;
    }

    template< class U1, class U2 >
    Pair & operator=(Pair<U1, U2>&& other)
    {
        first = std::forward<U1>(other.first);
        second = std::forward<U2>(other.second);
        return *this;
    }

    void swap(Pair& other)
    {
        std::swap(first, other.first);
        std::swap(second, other.second);
    }

    T1 first;
    T2 second;

private:
    template <class Tuple1, class Tuple2, size_t ... Indexes1, size_t ... Indexes2>
    Pair(Tuple1& Val1, Tuple2& Val2, std::index_sequence<Indexes1...>, std::index_sequence<Indexes2...>);
};

template<typename T1, typename T2>
template<typename ... Args1, typename... Args2 >
Pair<T1, T2>::Pair(PiecewiseConstructT, std::tuple<Args1...> first_args, std::tuple<Args2...> second_args) :
    Pair(first_args, second_args, std::index_sequence_for<Args1...>{}, std::index_sequence_for<Args2...>{})
{
}

template<typename T1, typename T2>
template <class Tuple1, class Tuple2, size_t ... Indexes1, size_t ... Indexes2>
Pair<T1, T2>::Pair(Tuple1& Val1, Tuple2& Val2, std::index_sequence<Indexes1...>, std::index_sequence<Indexes2...>) :
    first(std::get<Indexes1>(std::move(Val1))...),
    second(std::get<Indexes2>(std::move(Val2))...)
{
}

template<typename T1, typename T2>
Pair<typename std::_Unrefwrap_t<T1>, typename std::_Unrefwrap_t<T2>>
MakePair(T1 && v, T2 && u)
{
    using MyPair = Pair<typename std::_Unrefwrap_t<T1>, typename std::_Unrefwrap_t<T2>>;
    return MyPair(std::forward<T1>(v), std::forward<T2>(u));
}

template<typename T1, typename T2>
bool operator==(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return left.first == right.first && left.second == right.second;
}

template<typename T1, typename T2>
bool operator!=(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return !(left == right);
}

template<typename T1, typename T2>
bool operator<(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return (left.first < right.first) || (!(right.first < left.first) && left.second < right.second);
}

template<typename T1, typename T2>
bool operator<=(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return !(right < left);
}

template<typename T1, typename T2>
bool operator>(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return right < left;
}

template<typename T1, typename T2>
bool operator>=(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
{
    return !(left < right);
}

template <typename T1, typename T2>
void swap(Pair<T1, T2>& left, Pair<T1, T2>& right) /*noexcept()*/
{
    left.swap(right);
}

// TODO: move to Tuple.h?

template <typename... Types>
class Tuple;

template <std::size_t I, typename _Tuple>
class TupleElement;

template<typename Head, typename... Tail>
struct TupleElement<0, Tuple<Head, Tail...>>
{
    using type = Head;
    using TupleType = Tuple<Head, Tail...>;
};

template<std::size_t I, typename Head, typename... Tail>
struct TupleElement<I, Tuple<Head, Tail...>> :
    TupleElement<I - 1, Tuple<Tail...>>
{
};

template <class _Tuple>
struct TupleSize;

template<typename... Types >
struct TupleSize<Tuple<Types...>> :
    std::integral_constant<std::size_t, sizeof...(Types)>
{
};

// TODO: get


} /*namespace naive*/
