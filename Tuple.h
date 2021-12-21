#pragma once

#include "Utility.h"

// TODO: constexpr the whole thing

namespace naive {

// Tag dispatch for unpacking from Tuple with other types
struct UnpackTag {
	explicit UnpackTag() = default;
};

template <class... Types>
class Tuple;

template <>
class Tuple<>
{
public:
	Tuple() = default;

	Tuple& operator=(const Tuple&) = default;

	void Swap(const Tuple&)
	{ }

	constexpr bool Equals(const Tuple<>& rhs) const
	{ return true; }

	constexpr bool Less(const Tuple<>& rhs) const
	{ return true; }
};

template <typename T, typename... Types>
class Tuple<T, Types...> : public Tuple<Types...>
{
public:
	Tuple() :
		Tuple<Types...>(),
		element()
	{ }

	Tuple(const T& e, const Types&... args) :
		Tuple<Types...>(args...),
		element(e)
	{ }

	template<typename U, typename... UTypes,
		std::enable_if_t<
		    std::conjunction_v<
		        std::_Tuple_perfect_val<std::tuple<T, Types...>, U, UTypes...>,
		        std::_Tuple_constructible_val<std::tuple<T, Types...>, U, UTypes...>
		    >,
		    int
		> = 0>
	Tuple(U&& u, UTypes&&... args) :
		Tuple<Types...>(std::forward<UTypes>(args)...),
		element(std::forward<U>(u))
	{ }

	template<typename... UTypes >
	Tuple(const Tuple<UTypes...>& other) :
		Tuple(
			UnpackTag(),
			other,
			std::make_index_sequence<TupleSize<Tuple<UTypes...>>::value>())
	{
	}

	template <typename... UTypes>
	Tuple(Tuple<UTypes...>&& other) :
		Tuple(
			UnpackTag(),
			std::forward<Tuple<UTypes...>>(other),
			std::make_index_sequence<TupleSize<Tuple<UTypes...>>::value>())
	{
	}

	template<typename U1, typename U2,
		std::enable_if_t<
			std::_Tuple_constructible_v<std::tuple<T, Types...>, U1, U2>,
			int
		> = 0>
	Tuple(const Pair<U1, U2>& p) :
		Tuple(p.first, p.second)
	{ }

	template<typename U1, typename U2,
		std::enable_if_t<
			std::_Tuple_constructible_v<std::tuple<T, Types...>, U1, U2>,
			int
		> = 0>
	Tuple(Pair<U1, U2>&& p):
		Tuple(std::forward<U1>(p.first), std::forward<U2>(p.second))
	{ }

	Tuple(const Tuple& other) = default;
	Tuple(Tuple&& other) = default;

public:
	Tuple& operator=(const Tuple& other)
	{
		element = other.element;
		static_cast<Tuple<Types...>&>(*this) = static_cast<const Tuple<Types...>&>(other);
		return *this;
	}

	Tuple& operator=(Tuple&& other)
	{
		element = std::move(other.element);
		static_cast<Tuple<Types...>&>(*this) = static_cast<Tuple<Types...>&&>(std::move(other));
		return *this;
	}

	template<typename U, typename... UTypes >
	Tuple& operator=(const Tuple<U, UTypes...>& other)
	{
		element = other.element;
		static_cast<Tuple<Types...>&>(*this) = static_cast<const Tuple<UTypes...>&>(other);
		return *this;
	}

	template<typename U, typename... UTypes >
	Tuple& operator=(Tuple<U, UTypes...>&& other)
	{
		element = std::move(other.element);
		static_cast<Tuple<Types...>&>(*this) = static_cast<Tuple<UTypes...>&&>(std::move(other));
		return *this;
	}

	template<class U1, class U2>
	Tuple& operator=(const Pair<U1, U2>& p)
	{
		element = p.first;
		static_cast<Tuple<Types...>&>(*this).element = p.second;
		return *this;
	}

	template<class U1, class U2>
	Tuple& operator=(Pair<U1, U2>&& p)
	{
		element = std::move(p.first);
		static_cast<Tuple<Types...>&>(*this).element = std::move(p.second);
		return *this;
	}

	void Swap(Tuple& other)
	{
		std::swap(element, other.element);
		static_cast<Tuple<Types...>&>(*this).Swap(static_cast<Tuple<Types...>&>(other));
	}

private:
	template<typename... UTypes, std::size_t... Indices>
	Tuple(UnpackTag, const Tuple<UTypes...>& other, std::index_sequence<Indices...>);

	template<typename... UTypes, std::size_t... Indices>
	Tuple(UnpackTag, Tuple<UTypes...>&& other, std::index_sequence<Indices...>);

protected:
	template <typename U, typename... UTypes>
	constexpr bool Equals(const Tuple<U, UTypes...>& rhs) const
	{ return element == rhs.element && Tuple<Types...>::Equals(static_cast<const Tuple<UTypes...>&>(rhs)); }

	template <class U, class... UTypes>
	constexpr bool Less(const Tuple<U, UTypes...>& rhs) const
	{ return element < rhs.element && Tuple<Types...>::Less(static_cast<const Tuple<UTypes...>&>(rhs)); }

private:
	template<std::size_t _Index, typename ... _Types>
	friend typename TupleElement<_Index, Tuple<_Types...>>::type& Get(Tuple<_Types...>& t) noexcept;

	template<std::size_t _Index, typename ... _Types>
	friend typename const TupleElement<_Index, Tuple<_Types...>>::type& Get(const Tuple<_Types...>& t) noexcept;

	template<std::size_t _Index, typename ... _Types>
	friend typename TupleElement<_Index, Tuple<_Types...>>::type&& Get(Tuple<_Types...>&& t) noexcept;

	template<std::size_t _Index, typename ... _Types>
	friend typename const TupleElement<_Index, Tuple<_Types...>>::type&& Get(const Tuple<_Types...>&& t) noexcept;

	template<class... TTypes, class... UTypes >
	friend constexpr bool operator==(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs);

	template<class... TTypes, class... UTypes >
	friend constexpr bool operator<(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs);

	// TODO get by type + constexpr

public:
	// TODO: extract for type

	T element;
};

template<typename T>
struct ReferenceUnwrapper
{
	using Type = T;
};

template<typename T>
struct ReferenceUnwrapper<std::reference_wrapper<T>>
{
	using Type = T&;
};

template<class T>
using UnwrapDecayT = typename ReferenceUnwrapper<typename std::decay_t<T>>::Type;

template<typename... Types>
Tuple<UnwrapDecayT<Types>...> MakeTuple(Types&&... args)
{
	return Tuple<UnwrapDecayT<Types>...>(std::forward<Types>(args)...);
}

template<typename... Types>
Tuple<Types&...> Tie(Types&... args) noexcept
{
	return Tuple<Types&...>(args...);
}

template<typename... Types>
Tuple<Types&&...> ForwardAsTuple(Types&&... args) noexcept
{
	return Tuple<Types&&...>(std::forward<Types>(args)...);
}

template<std::size_t I, typename... Types>
typename TupleElement<I, Tuple<Types...>>::type&
Get(Tuple<Types...>& t) noexcept
{
	using TupleType = typename TupleElement<I, Tuple<Types...>>::TupleType;
	return static_cast<TupleType&>(t).element;
}

template<std::size_t I, typename... Types>
typename const TupleElement<I, Tuple<Types...>>::type&
Get(const Tuple<Types...>& t) noexcept
{
	using TupleType = typename TupleElement<I, Tuple<Types...>>::TupleType;
	return static_cast<const TupleType&>(t).element;
}

template<std::size_t I, typename... Types>
typename TupleElement<I, Tuple<Types...>>::type&&
Get(Tuple<Types...>&& t) noexcept
{
	using TupleType = typename TupleElement<I, Tuple<Types...>>::TupleType;
	return static_cast<TupleType&&>(t).element;
}

template<std::size_t I, typename... Types>
const typename TupleElement<I, Tuple<Types...>>::type&&
Get(const Tuple<Types...>&& t) noexcept
{
	using TupleType = typename TupleElement<I, Tuple<Types...>>::TupleType;
	return static_cast<const TupleType&&>(t).element;
}

template <typename... Tuples>
struct TupleCatTypeExtractor;

template <>
struct TupleCatTypeExtractor<>
{
	using Type = Tuple<>;
};

template <typename... Args>
struct TupleCatTypeExtractor<Tuple<Args...>>
{
	using Type = Tuple<Args...>;
};

template<typename... Args1, typename... Args2, typename... Tuples>
struct TupleCatTypeExtractor<Tuple<Args1...>, Tuple<Args2...>, Tuples...>
{
	using Type = typename TupleCatTypeExtractor<Tuple<Args1..., Args2...>, Tuples...>::Type;
};

// TupleCatValueExtractor recursively does two things until we processed all input tuples:
// 1. extract elements from current input tuple
// 2. add extracted elements to the output tuple

template<typename... T>
struct TupleCatValueExtractor;

template<typename RetVal, std::size_t... Indices, typename Tuple>
struct TupleCatValueExtractor<RetVal, std::index_sequence<Indices...>, Tuple>
{
	using IndexSequenceType = decltype(std::make_index_sequence<TupleSize<std::remove_reference<Tuple>::type>::value>());

	static RetVal ExtractElements(Tuple&& tuple)
	{
		return RetVal(Get<Indices>(std::forward<Tuple>(tuple))...);
	}

	template<typename... Args>
	static RetVal ExtractElements(Tuple&& tuple, Args&&... args)
	{
		return RetVal(std::forward<Args>(args)..., Get<Indices>(std::forward<Tuple>(tuple))...);
	}

	template <typename... Args>
	static RetVal AddElementsToTuple(Tuple&& tuple, Args&&... args)
	{
		return TupleCatValueExtractor<RetVal, IndexSequenceType, Tuple>::ExtractElements(
			std::forward<Tuple>(tuple),
			std::forward<Args>(args)...);
	}
};

template<typename RetVal, std::size_t... Indices, typename T, typename... Tuples>
struct TupleCatValueExtractor<RetVal, std::index_sequence<Indices...>, T, Tuples...>
{
	using IndexSequenceType = decltype(std::make_index_sequence<TupleSize<std::remove_reference<T>::type>::value>());

	static RetVal ExtractElements(T&& tuple, Tuples&&... tuples)
	{
		return TupleCatValueExtractor<RetVal, IndexSequenceType, Tuples...>::AddElementsToTuple(
			std::forward<Tuples>(tuples)...,
			Get<Indices>(std::forward<T>(tuple))...);
	}

	template<typename... Args>
	static RetVal ExtractElements(T&& tuple, Tuples&&... tuples, Args&&... args)
	{
		return TupleCatValueExtractor<RetVal, IndexSequenceType, Tuples...>::AddElementsToTuple(
			std::forward<Tuples>(tuples)...,
			std::forward<Args>(args)...,
			Get<Indices>(std::forward<T>(tuple))...);
	}

	template <typename... Args>
	static RetVal AddElementsToTuple(T&& tuple, Tuples&&... tuples, Args&&... args)
	{
		return TupleCatValueExtractor<RetVal, IndexSequenceType, T, Tuples...>::ExtractElements(
			std::forward<T>(tuple),
			std::forward<Tuples>(tuples)...,
			std::forward<Args>(args)...);
	}
};

template<typename RetVal, typename T, typename... Tuples>
RetVal DoTupleCat(T&& tuple, Tuples&&... tuples)
{
	return TupleCatValueExtractor<
		RetVal,
		decltype(std::make_index_sequence<TupleSize<std::remove_reference<T>::type>::value>()),
		T,
		Tuples...>::ExtractElements(
			std::forward<T>(tuple),
			std::forward<Tuples>(tuples)...);
}

template<typename... Tuples>
typename TupleCatTypeExtractor<typename std::remove_reference_t<Tuples>...>::Type TupleCat(Tuples&&... tuples)
{
	using RetVal = typename TupleCatTypeExtractor<std::remove_reference_t<Tuples>...>::Type;
	return DoTupleCat<RetVal, Tuples...>(std::forward<Tuples>(tuples)...);
}

template <typename T, typename... Types>
template <typename... UTypes, std::size_t... Indices>
Tuple<T, Types...>::Tuple(UnpackTag, const Tuple<UTypes...>& other, std::index_sequence<Indices...>) :
	Tuple(Get<Indices>(other)...)
{
}

template <typename T, typename... Types>
template <typename... UTypes, std::size_t... Indices>
Tuple<T, Types...>::Tuple(UnpackTag, Tuple<UTypes...>&& other, std::index_sequence<Indices...>) :
	Tuple(Get<Indices>(std::forward<Tuple<UTypes...>>(other))...)
{
}

template<class... TTypes, class... UTypes >
constexpr bool operator==(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return lhs.Equals(rhs);
}

template<class... TTypes, class... UTypes >
constexpr bool operator!=(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return !(lhs == rhs);
}

template<class... TTypes, class... UTypes >
constexpr bool operator<(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return lhs.Less(rhs);
}

template<class... TTypes, class... UTypes >
constexpr bool operator<=(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return !(rhs < lhs);
}

template<class... TTypes, class... UTypes >
constexpr bool operator>(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return rhs < lhs;
}

template<class... TTypes, class... UTypes >
constexpr bool operator>=(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs)
{
	return !(lhs < rhs);
}

template<class... Types >
void Swap(Tuple<Types...>& lhs, Tuple<Types...>& rhs)
{
	lhs.Swap(rhs);
}

struct _IgnoreType
{
	template<typename T>
	constexpr const _IgnoreType& operator=(const T&) const noexcept
	{ return *this; }
};

constexpr _IgnoreType Ignore;

} /*namespace naive*/
