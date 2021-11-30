#pragma once

#include <cassert>
#include <stdexcept>

#include "RedBlackTree.h"

namespace naive {

template <typename Key, typename Value>
class Map :
    public RedBlackTree<Key, Value>
{
public:
    using Tree                 = RedBlackTree<Key, Value>;
    using ValueType            = Tree::ValueType;
    using Iterator             = Tree::Iterator;
    using ConstIterator        = Tree::ConstIterator;
    using ReverseIterator      = Tree::ReverseIterator;
    using ReverseConstIterator = Tree::ReverseConstIterator;

public:
    // Construct, destruct, assign
    Map() = default;

    Map(const Map& map) :
        Tree(map)
    { }

    Map(Map&& map) :
        Tree(std::move(map))
    { }

    template<class InputIt>
    Map(InputIt first, InputIt last) :
        Tree()
    { insert(first, last); }

    Map(std::initializer_list<ValueType> init) :
        Tree()
    { insert(init); }

    ~Map() = default;

    Map& operator=(const Map& map)
    {
        Tree::operator=(map);
        return *this;
    }

    Map& operator=(Map&& map)
    {
        Tree::operator=(std::move(map));
        return *this;
    }

    Map& operator=(std::initializer_list<ValueType> ilist)
    {
        clear();
        insert(ilist);
        return *this;
    }

public:
    // Element access
    Value& at(const Key& key)
    {
        TreeNode* node = do_find(key);
        if (node == nullptr) {
            throw std::out_of_range("");
        }
        return node->value();
    }

    const Value& at(const Key& key) const
    {
        TreeNode* node = do_find(key);
        if (node == nullptr) {
            throw std::out_of_range("");
        }
        return node->value();
    }

    Value& operator[](const Key& key)
    { return (emplace(key, Value()).first)->second; }

    Value& operator[](Key&& key)
    { return (emplace(std::forward<Key>(key), Value()).first)->second; }

public:
    // Iterators

    Iterator begin()
    { return Tree::begin(); }
    Iterator end()
    { return Tree::end(); }

    ConstIterator cbegin() const
    { return Tree::cbegin(); }
    ConstIterator cend() const
    { return Tree::cend(); }

    ReverseIterator rbegin()
    { return Tree::rbegin(); }
    ReverseIterator rend()
    { return Tree::rend();}

    ReverseConstIterator rcbegin() const
    { return Tree::rcbegin(); }
    ReverseConstIterator rcend() const
    { return Tree::rcend(); }

public:
    // Capacity

    bool empty() const
    { return Tree::empty(); }

    size_t size() const
    { return Tree::size(); }

public:
    // Modifiers

    void clear()
    { Tree::clear(); }

    Pair<Iterator, bool> insert(const ValueType& value)
    { return emplace(value); }

    template <class P, typename U = std::enable_if_t<std::is_constructible_v<ValueType, P>>>
    Pair<Iterator, bool> insert(P&& value)
    { return emplace(std::forward<P>(value)); }

    Pair<Iterator, bool> insert(ValueType&& value)
    { return emplace(std::forward<ValueType>(value)); }

    Pair<Iterator, bool> insert(ConstIterator hint, const ValueType& value)
    { return emplace_hint(hint, value); }

    template <class P, typename U = std::enable_if_t<std::is_constructible_v<ValueType, P>>>
    Pair<Iterator, bool> insert(ConstIterator hint, P&& value)
    { return emplace_hint(hint, std::forward<P>(value)); }

    Pair<Iterator, bool> insert(ConstIterator hint, ValueType && value)
    { return emplace_hint(hint, std::forward<ValueType>(value)); }

    template<class InputIt>
    void insert(InputIt first, InputIt last)
    {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    void insert(std::initializer_list<ValueType> ilist)
    { insert(ilist.begin(), ilist.end()); }

    template <typename ... Args>
    Pair<Iterator, bool> emplace(Args && ... args)
    { return Tree::emplace(std::forward<Args>(args)...); }

    template <typename ... Args>
    Pair<Iterator, bool> emplace_hint(ConstIterator hint, Args && ... args)
    { return Tree::emplace_hint(hint, std::forward<Args>(args)...); }

    Iterator erase(Iterator pos)
    { return Tree::erase(pos); }

    Iterator erase(ConstIterator pos)
    { return Tree::erase(pos); }

    Iterator erase(ConstIterator first, ConstIterator last)
    {
        while (first != last) {
            first = erase(first);
        }

        return first;
    }

    size_t erase(const Key& key)
    {
        auto it = find(key);
        if (it == end()) {
            return 0;
        }

        erase(it);
        return 1;
    }

    void swap(Map& other) noexcept
    { Tree::swap(other); }

public:
    // Lookup

    size_t count(const Key& key) const
    { return Tree::count(key); }

    ConstIterator find(const Key& key) const
    { return Tree::find(key); }

    Iterator find(const Key& key)
    { return Tree::find(key); }

    Pair<Iterator, Iterator> equal_range(const Key& key)
    { return Tree::equal_range(key); }

    Pair<ConstIterator, ConstIterator> equal_range(const Key& key) const
    { return Tree::equal_range(key); }

    Iterator lower_bound(const Key& key)
    { return Tree::lower_bound(key); }

    ConstIterator lower_bound(const Key& key) const
    { return Tree::lower_bound(key); }

    Iterator upper_bound(const Key& key)
    { return Tree::upper_bound(key); }

    ConstIterator upper_bound(const Key& key) const
    { return Tree::upper_bound(key); }

private:
    using TreeNode = Tree::TreeNode;
};

template<typename Key, typename Value>
bool operator==(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }
    
    auto lit  = lhs.cbegin();
    auto lend = lhs.cend();
    auto rit  = rhs.cbegin();

    for(; lit != lend; ++lit, ++rit) {
        if (*lit != *rit){
            return false;
        }
    }

    return true;
}

template<typename Key, typename Value>
bool operator!=(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    return !operator==(lhs, rhs);
}

template<typename Key, typename Value>
bool operator<(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<typename Key, typename Value>
bool operator<=(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    return !operator<(rhs, lhs);
}

template<typename Key, typename Value>
bool operator>(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    return operator<(rhs, lhs);
}

template<typename Key, typename Value>
bool operator>=(const Map<Key, Value>& lhs, const Map<Key, Value>& rhs)
{
    return !operator<(lhs, rhs);
}

template<typename Key, typename Value>
void swap(Map<Key, Value>& lhs, Map<Key, Value>& rhs)
{
    lhs.swap(rhs);
}

} /*namespace naive*/
