/*#include "Map.h"
#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include <utility>

#include "Utility.h"

using namespace naive;

class A
{
public:
    int& x;
    A() = default;
    A(const A&) = default;
    A(int f) : x(f)
    {}

    void operator=(const A& other)
    {
        x = other.x;
    }

    void operator=(A && other) noexcept
    {
        x = std::move(other.x);
    }

    bool operator==(const A& other) const
    {
        return x == other.x;
    }
};

int main()
{
    std::map<int, int> pp;
    pp.emplace(std::piecewise_construct, std::make_tuple(1), std::make_tuple(3));
    pp.insert({ 2, 4 });
    auto ppeit = pp.end();
    pp.erase(2);
    --ppeit;
    //a.emplace(3, 3);

    Map<int, std::string> map;

    Pair<const int, std::string> tmp(10, "b");

    map.insert(tmp);
    map.insert(MakePair(5, "a"));
    Pair<const short, std::string> tt = MakePair(3, "a");
    map.insert(std::move(tt));
    auto hint = map.insert(MakePair(20, "c")).first;

    const auto& v = *hint;
    map.insert(v);
    auto it = map.insert(hint, MakePair(30, "e")).first;
    hint == map.end();
    ++hint;
    hint++;
    map.insert(std::move(*it));

    Map<int, std::string>::ConstIterator chint = hint;

    map[40] = "f";

    map.emplace(1, "-");
    map.emplace(MakePair(25, "d"));

    hint = map.emplace(30, "x").first;
    map.emplace_hint(hint, 19, "+");

    map.emplace(PiecewiseConstruct, std::make_tuple(45), std::make_tuple("g"));

    Map<int, std::string>::Iterator beg = map.begin();

    auto cit = map.cbegin();
    while (cit != map.cend()) {
        std::cout << (*cit).second << std::endl;
        ++cit;
    }

    map.erase(hint);

    map.empty();
    map.size();

    Map<int, std::string> map2(map.begin(), map.end());
    Map<int, std::string> map3 = {tmp};
    map3 = {tmp};

    map2.swap(map3);

    auto fit = map.find(10);

    const auto map4 = map;
    map4.find(0);
    map4.count(30);

    auto r = map.equal_range(30);
    map4.equal_range(30);

    auto lb = map.lower_bound(10);
    map4.lower_bound(10);

    auto ub = map.upper_bound(10);
    map4.upper_bound(10);

    map == map4;

    swap(map, map3);

    bool cmp = map < map3;

    map3.erase(map3.begin());
    bool cmp2 = map3 < map;
    bool cmp3 = map < map3;

    auto eit = map.end();
    --eit;

    Map<int, std::string> rmap;
    rmap[10] = "a";
    rmap[20] = "b";
    rmap[30] = "c";
    rmap[40] = "d";
    rmap[50] = "e";
    rmap[60] = "f";
    rmap[70] = "g";
    auto rit = rmap.rcbegin();
    while (rit != rmap.rcend()) {
        std::cout << (*rit).second << std::endl;
        ++rit;
    }
    rmap.erase(10);
    rmap.erase(70);
    
    int b = 0;
    auto a = MakePair(b, b);

    int x = 5;
    const double f = 2.0;
    //Pair<int, double> p(x, 2.0);
    Pair<A, double> p(x, f);

    Pair<A, double> j(p);

    bool c = (p != j);

    swap(p, j);

    std::cin.get();
    return 0;
}*/