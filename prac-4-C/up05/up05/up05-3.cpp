#include <iostream>
#include <functional>
#include <vector>

#define rwrap std::reference_wrapper

template <typename It, typename Fp>
void myapply(It it_beg, It it_end, Fp prd_func) {
    for (It i = it_beg; i != it_end; i++)
        prd_func(*i);
}


template <typename Pt, typename Fp>
void myapply(Pt *p_beg, Pt *p_end, Fp prd_func) {
    for (Pt *p = p_beg; p != p_end; p++)
        prd_func(*p);
}


template <typename It, typename Fp>
auto myfilter2(It it_beg, It it_end, Fp prd_func) {
    std::vector<rwrap<typename It::value_type>> v;
    for (It i = it_beg; i != it_end; i++)
        if (prd_func(*i)) v.insert(v.end(), *i);
    return v;
}


template <typename Pt, typename Fp>
auto myfilter2(Pt *p_beg, Pt *p_end, Fp prd_func) {
    std::vector<rwrap<Pt>> v;
    for (Pt *p = p_beg; p != p_end; p++)
        if (prd_func(*p)) v.insert(v.end(), *p);
    return v;
}


