/*#include "Tuple.h"
#include <iostream>
#include <string>
#include <tuple>

using namespace naive;

int main()
{
    Tuple<int, std::string> t;
    Tuple<int, std::string> t2(3, std::string("a"));
    Tuple<int, std::string> t3(3.0, "a");

    Tuple<double, std::string> td(1.0, "a");
    Tuple<int, std::string> t4(td);

    Tuple<int, std::string> t5(Tuple<double, std::string>(5.0, "b"));

    Pair<double, std::string> p(1, "b");
    Tuple<int, std::string> t6(p);

    Pair<double, std::string> p2(2.0, "x");
    Tuple<int, std::string> t7(std::move(p2));

    Tuple<int, std::string> t8(t7);

    Tuple<int, std::string> t9(std::move(t7));

    t = t2;
    t = std::move(t2);
    
    Tuple<int, double, std::string> ttd(0, 1.0, "a");
    Tuple<double, int, std::string> ttest;
    ttest = ttd;
    ttest = std::move(ttd);

    Pair<int, std::string> pp(1, "fg");
    Tuple<int, std::string> ts;
    ts = pp;
    ts = std::move(pp);

    t6.Swap(t7);

    int i = 5;
    auto mtt = MakeTuple(1, 2.0, std::string("s"), std::reference_wrapper<int>(i));

    int n = 3;
    double f = 2.0;
    auto tie_tuple = Tie(n, f);

    auto forwarded_tuple = ForwardAsTuple(n, 5);

    Tuple<int&, std::string, double> t3t(n, "0", 3.0);
    auto tc = TupleCat(t3t, t4, t5);

    std::tuple<int&, std::string> st3(n, "0");
    std::tuple<int, std::string> st4(1.0, "a");
    std::tuple<int, std::string> st5(5.0, "b");
    auto xxx = std::tuple_cat(st3, st4, st5);

    auto& a = Get<0>(t2);
    auto& b = Get<1>(t2);

    const Tuple<int, std::string> t10(t7);
    const auto& c = Get<0>(t10);

    auto&& d = Get<0>(Tuple<int, std::string>(5, "abc"));

    const Tuple<int, std::string>&& rvalue_tuple = Tuple<int, std::string>(5, "abc");
    const auto&& e = Get<0>(std::move(rvalue_tuple));

    t8 == t9;
    t8 < t9;

    Swap(t8, t9);

    int iii = 5;
    Tuple<int, std::string> tuple_ignore(5, "abc");
    auto x = Tie(iii, Ignore);
    Tie(iii, Ignore) = tuple_ignore;

    std::cin.get();

    return 0;
}*/
