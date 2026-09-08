#pragma once
#include <mpfr.h>
#include <gmp.h>

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
using namespace std;
class Interval {
public:
    mpfr_t lo, hi;

    static constexpr mpfr_prec_t PREC = 256;

    Interval() {
        mpfr_init2(lo, PREC);
        mpfr_init2(hi, PREC);
        mpfr_set_zero(lo, 0);
        mpfr_set_zero(hi, 0);
    }

    explicit Interval(const char* x) {
        mpfr_init2(lo, PREC);
        mpfr_init2(hi, PREC);
        mpfr_set_str(lo, x, 10, MPFR_RNDD);
        mpfr_set_str(hi, x, 10, MPFR_RNDU);
    }
    explicit Interval(string s):Interval(s.c_str()){}
    template<typename T>
    explicit Interval(T n):Interval(to_string(n)){}
    Interval(const char* l, const char* r) {
        mpfr_init2(lo, PREC);
        mpfr_init2(hi, PREC);
        mpfr_set_str(lo, l, 10, MPFR_RNDD);
        mpfr_set_str(hi, r, 10, MPFR_RNDU);
    }

    Interval(const Interval& x) {
        mpfr_init2(lo, mpfr_get_prec(x.lo));
        mpfr_init2(hi, mpfr_get_prec(x.hi));
        mpfr_set(lo, x.lo, MPFR_RNDD);
        mpfr_set(hi, x.hi, MPFR_RNDU);
    }

    Interval& operator=(const Interval& x) {
        if (this != &x) {
            mpfr_set(lo, x.lo, MPFR_RNDD);
            mpfr_set(hi, x.hi, MPFR_RNDU);
        }
        return *this;
    }

    ~Interval() {
        mpfr_clear(lo);
        mpfr_clear(hi);
    }

    void set(double x) {
        mpfr_set_d(lo, x, MPFR_RNDD);
        mpfr_set_d(hi, x, MPFR_RNDU);
    }

    void print(int digits = 30) const {
        mpfr_printf("[%.*Rf, %.*Rf]\n",
                    digits, lo, digits, hi);
    }

    bool contains_zero() const {
        return mpfr_sgn(lo) <= 0 && mpfr_sgn(hi) >= 0;
    }

    bool is_empty() const {
        return mpfr_greater_p(lo, hi);
    }

    void print_diff(int digits=30)const{
      mpfr_t d;
      mpfr_init2(d,PREC);
      mpfr_sub(d,hi,lo,MPFR_RNDU);
      mpfr_printf("%.*Rf\n",digits,d);
      mpfr_clear(d);
    }
    static Interval entire() {
        Interval x;
        mpfr_set_inf(x.lo, -1);
        mpfr_set_inf(x.hi, +1);
        return x;
    }
    static Interval get_pi(){
      Interval x;
      mpfr_const_pi(x.lo,MPFR_RNDD);
      mpfr_const_pi(x.hi,MPFR_RNDU);
      return x;
    }
    friend bool operator<(const Interval& a, const Interval& b) {
    return mpfr_less_p(a.hi, b.lo);
    }

    friend bool operator<=(const Interval& a, const Interval& b) {
        return mpfr_lessequal_p(a.hi, b.lo);
    }

    friend bool operator==(const Interval& a, const Interval& b) {
        return mpfr_equal_p(a.lo, a.hi) &&
              mpfr_equal_p(b.lo, b.hi) &&
              mpfr_equal_p(a.lo, b.lo);
    }
    friend Interval operator-(const Interval& a) {
        Interval res;

        mpfr_neg(res.lo, a.hi, MPFR_RNDD);
        mpfr_neg(res.hi, a.lo, MPFR_RNDU);

        return res;
    }
    bool contains(const Interval&a){
      return mpfr_lessequal_p(lo,a.lo)&&mpfr_lessequal_p(a.hi,hi);
    }
};
Interval operator+(const Interval& a, const Interval& b) {
    Interval c;

    mpfr_add(c.lo, a.lo, b.lo, MPFR_RNDD);
    mpfr_add(c.hi, a.hi, b.hi, MPFR_RNDU);

    return c;
}
Interval operator-(const Interval& a, const Interval& b) {
    Interval c;

    mpfr_sub(c.lo, a.lo, b.hi, MPFR_RNDD);
    mpfr_sub(c.hi, a.hi, b.lo, MPFR_RNDU);

    return c;
}
Interval operator*(const Interval& a, const Interval& b) {
    Interval c;
    mpfr_t x, y;

    mpfr_init2(x, Interval::PREC);
    mpfr_init2(y, Interval::PREC);

    // 下限
    mpfr_mul(x, a.lo, b.lo, MPFR_RNDD);
    mpfr_mul(y, a.lo, b.hi, MPFR_RNDD);
    mpfr_set(c.lo, x, MPFR_RNDD);
    if (mpfr_less_p(y, c.lo))
        mpfr_set(c.lo, y, MPFR_RNDD);

    mpfr_mul(x, a.hi, b.lo, MPFR_RNDD);
    if (mpfr_less_p(x, c.lo))
        mpfr_set(c.lo, x, MPFR_RNDD);

    mpfr_mul(y, a.hi, b.hi, MPFR_RNDD);
    if (mpfr_less_p(y, c.lo))
        mpfr_set(c.lo, y, MPFR_RNDD);

    // 上限
    mpfr_mul(x, a.lo, b.lo, MPFR_RNDU);
    mpfr_mul(y, a.lo, b.hi, MPFR_RNDU);
    mpfr_set(c.hi, x, MPFR_RNDU);
    if (mpfr_greater_p(y, c.hi))
        mpfr_set(c.hi, y, MPFR_RNDU);

    mpfr_mul(x, a.hi, b.lo, MPFR_RNDU);
    if (mpfr_greater_p(x, c.hi))
        mpfr_set(c.hi, x, MPFR_RNDU);

    mpfr_mul(y, a.hi, b.hi, MPFR_RNDU);
    if (mpfr_greater_p(y, c.hi))
        mpfr_set(c.hi, y, MPFR_RNDU);

    mpfr_clear(x);
    mpfr_clear(y);

    return c;
}
Interval operator/(const Interval& a, const Interval& b) {
    if (b.contains_zero()) {
        return Interval::entire();
    }

    Interval c;
    mpfr_t x, y;

    mpfr_init2(x, Interval::PREC);
    mpfr_init2(y, Interval::PREC);

    // 下限
    mpfr_div(x, a.lo, b.lo, MPFR_RNDD);
    mpfr_div(y, a.lo, b.hi, MPFR_RNDD);
    mpfr_set(c.lo, x, MPFR_RNDD);

    if (mpfr_less_p(y, c.lo))
        mpfr_set(c.lo, y, MPFR_RNDD);

    mpfr_div(x, a.hi, b.lo, MPFR_RNDD);
    if (mpfr_less_p(x, c.lo))
        mpfr_set(c.lo, x, MPFR_RNDD);

    mpfr_div(y, a.hi, b.hi, MPFR_RNDD);
    if (mpfr_less_p(y, c.lo))
        mpfr_set(c.lo, y, MPFR_RNDD);

    // 上限
    mpfr_div(x, a.lo, b.lo, MPFR_RNDU);
    mpfr_div(y, a.lo, b.hi, MPFR_RNDU);
    mpfr_set(c.hi, x, MPFR_RNDU);

    if (mpfr_greater_p(y, c.hi))
        mpfr_set(c.hi, y, MPFR_RNDU);

    mpfr_div(x, a.hi, b.lo, MPFR_RNDU);
    if (mpfr_greater_p(x, c.hi))
        mpfr_set(c.hi, x, MPFR_RNDU);

    mpfr_div(y, a.hi, b.hi, MPFR_RNDU);
    if (mpfr_greater_p(y, c.hi))
        mpfr_set(c.hi, y, MPFR_RNDU);

    mpfr_clear(x);
    mpfr_clear(y);

    return c;
}
Interval sqrt(const Interval& a) {
    if (mpfr_sgn(a.hi) < 0)
        throw std::domain_error("sqrt of negative interval");

    Interval c;

    if (mpfr_sgn(a.lo) <= 0)
        mpfr_set_zero(c.lo, 0);
    else
        mpfr_sqrt(c.lo, a.lo, MPFR_RNDD);

    mpfr_sqrt(c.hi, a.hi, MPFR_RNDU);

    return c;
}
// x = [lo, hi] に対する sin(x)
Interval sin(const Interval& x) {
    Interval res;

    mpfr_t pi_lo, pi_hi, two_pi_lo, two_pi_hi;
    mpfr_inits2(
        mpfr_get_prec(x.lo),
        pi_lo, pi_hi, two_pi_lo, two_pi_hi,
        (mpfr_ptr)0
    );

    mpfr_const_pi(pi_lo, MPFR_RNDD);
    mpfr_const_pi(pi_hi, MPFR_RNDU);

    mpfr_mul_ui(two_pi_lo, pi_lo, 2, MPFR_RNDD);
    mpfr_mul_ui(two_pi_hi, pi_hi, 2, MPFR_RNDU);

    // 幅 >= 2π なら [-1, 1]
    mpfr_t width;
    mpfr_init2(width, mpfr_get_prec(x.lo));

    mpfr_sub(width, x.hi, x.lo, MPFR_RNDU);

    if (mpfr_greaterequal_p(width, two_pi_hi)) {
        mpfr_set_si(res.lo, -1, MPFR_RNDD);
        mpfr_set_si(res.hi,  1, MPFR_RNDU);

        mpfr_clears(
            pi_lo, pi_hi, two_pi_lo, two_pi_hi,
            width, (mpfr_ptr)0
        );
        return res;
    }

    // 端点
    mpfr_t t;
    mpfr_init2(t, mpfr_get_prec(x.lo));

    mpfr_sin(res.lo, x.lo, MPFR_RNDD);
    mpfr_sin(t,     x.hi, MPFR_RNDD);
    mpfr_min(res.lo, res.lo, t, MPFR_RNDD);

    mpfr_sin(res.hi, x.lo, MPFR_RNDU);
    mpfr_sin(t,      x.hi, MPFR_RNDU);
    mpfr_max(res.hi, res.hi, t, MPFR_RNDU);


    // ------------------------------------------------------------
    // sin(x) = 1 となる点
    //
    // x = π/2 + 2kπ
    // ------------------------------------------------------------

    mpfr_t qlo, qhi, k;
    mpfr_inits2(
        mpfr_get_prec(x.lo),
        qlo, qhi, k,
        (mpfr_ptr)0
    );

    /*
        q = (x - π/2) / (2π)

        π/2 の下限・上限を作る
    */
    mpfr_t half_pi_lo, half_pi_hi;
    mpfr_inits2(
        mpfr_get_prec(x.lo),
        half_pi_lo, half_pi_hi,
        (mpfr_ptr)0
    );

    mpfr_div_ui(half_pi_lo, pi_lo, 2, MPFR_RNDD);
    mpfr_div_ui(half_pi_hi, pi_hi, 2, MPFR_RNDU);

    // qlo = (x.lo - π/2) / (2π)
    mpfr_sub(qlo, x.lo, half_pi_hi, MPFR_RNDD);
    mpfr_div(qlo, qlo, two_pi_hi, MPFR_RNDD);

    // qhi = (x.hi - π/2) / (2π)
    mpfr_sub(qhi, x.hi, half_pi_lo, MPFR_RNDU);
    mpfr_div(qhi, qhi, two_pi_lo, MPFR_RNDU);

    // ceil(qlo) <= floor(qhi)
    mpfr_ceil(k, qlo);

    mpfr_t kmax;
    mpfr_init2(kmax, mpfr_get_prec(x.lo));
    mpfr_floor(kmax, qhi);

    if (mpfr_lessequal_p(k, kmax)) {
        mpfr_set_si(res.hi, 1, MPFR_RNDU);
    }


    // ------------------------------------------------------------
    // sin(x) = -1 となる点
    //
    // x = 3π/2 + 2kπ
    // ------------------------------------------------------------

    mpfr_t three_half_pi_lo, three_half_pi_hi;

    mpfr_inits2(
        mpfr_get_prec(x.lo),
        three_half_pi_lo, three_half_pi_hi,
        (mpfr_ptr)0
    );

    mpfr_mul_ui(
        three_half_pi_lo, half_pi_lo, 3, MPFR_RNDD
    );

    mpfr_mul_ui(
        three_half_pi_hi, half_pi_hi, 3, MPFR_RNDU
    );

    // qlo = (x.lo - 3π/2) / (2π)
    mpfr_sub(
        qlo, x.lo, three_half_pi_hi, MPFR_RNDD
    );
    mpfr_div(
        qlo, qlo, two_pi_hi, MPFR_RNDD
    );

    // qhi = (x.hi - 3π/2) / (2π)
    mpfr_sub(
        qhi, x.hi, three_half_pi_lo, MPFR_RNDU
    );
    mpfr_div(
        qhi, qhi, two_pi_lo, MPFR_RNDU
    );

    mpfr_ceil(k, qlo);
    mpfr_floor(kmax, qhi);

    if (mpfr_lessequal_p(k, kmax)) {
        mpfr_set_si(res.lo, -1, MPFR_RNDD);
    }

    mpfr_clears(
        pi_lo, pi_hi,
        two_pi_lo, two_pi_hi,
        width, t,
        qlo, qhi, k, kmax,
        half_pi_lo, half_pi_hi,
        three_half_pi_lo, three_half_pi_hi,
        (mpfr_ptr)0
    );

    return res;
}

// x = [lo, hi] に対する cos(x)
Interval cos(const Interval& x) {
    Interval res;

    mpfr_t width, two_pi;
    mpfr_inits2(mpfr_get_prec(x.lo), width, two_pi, (mpfr_ptr)0);

    mpfr_sub(width, x.hi, x.lo, MPFR_RNDU);
    mpfr_const_pi(two_pi, MPFR_RNDU);
    mpfr_mul_2ui(two_pi, two_pi, 1, MPFR_RNDU);

    if (mpfr_greaterequal_p(width, two_pi)) {
        mpfr_set_si(res.lo, -1, MPFR_RNDD);
        mpfr_set_si(res.hi,  1, MPFR_RNDU);
        mpfr_clears(width, two_pi, (mpfr_ptr)0);
        return res;
    }

    mpfr_cos(res.lo, x.lo, MPFR_RNDD);
    mpfr_cos(res.hi, x.lo, MPFR_RNDU);

    mpfr_t t;
    mpfr_init2(t, mpfr_get_prec(x.lo));

    mpfr_cos(t, x.hi, MPFR_RNDD);
    mpfr_min(res.lo, res.lo, t, MPFR_RNDD);

    mpfr_cos(t, x.hi, MPFR_RNDU);
    mpfr_max(res.hi, res.hi, t, MPFR_RNDU);

    mpfr_t pi_lo, pi_hi;
    mpfr_inits2(mpfr_get_prec(x.lo), pi_lo, pi_hi, (mpfr_ptr)0);
    mpfr_const_pi(pi_lo, MPFR_RNDD);
    mpfr_const_pi(pi_hi, MPFR_RNDU);

    mpfr_t a, b, k1, k2;
    mpfr_inits2(mpfr_get_prec(x.lo), a, b, k1, k2, (mpfr_ptr)0);

    // 最大値 1:
    // x = 2kπ
    mpfr_div(a, x.lo, two_pi, MPFR_RNDD);
    mpfr_div(b, x.hi, two_pi, MPFR_RNDU);

    mpfr_ceil(k1, a);
    mpfr_floor(k2, b);

    if (mpfr_lessequal_p(k1, k2)) {
        mpfr_set_si(res.hi, 1, MPFR_RNDU);
    }

    // 最小値 -1:
    // x = π + 2kπ
    mpfr_sub(a, x.lo, pi_hi, MPFR_RNDD);
    mpfr_div(a, a, two_pi, MPFR_RNDD);

    mpfr_sub(b, x.hi, pi_lo, MPFR_RNDU);
    mpfr_div(b, b, two_pi, MPFR_RNDU);

    mpfr_ceil(k1, a);
    mpfr_floor(k2, b);

    if (mpfr_lessequal_p(k1, k2)) {
        mpfr_set_si(res.lo, -1, MPFR_RNDD);
    }

    mpfr_clears(width, two_pi, t,
                pi_lo, pi_hi, a, b, k1, k2,
                (mpfr_ptr)0);

    return res;
}


// x = [lo, hi] に対する tan(x)
Interval tan(const Interval& x) {
    Interval res;

    mpfr_t pi_lo, pi_hi, two_pi;
    mpfr_inits2(mpfr_get_prec(x.lo), pi_lo, pi_hi, two_pi, (mpfr_ptr)0);

    mpfr_const_pi(pi_lo, MPFR_RNDD);
    mpfr_const_pi(pi_hi, MPFR_RNDU);
    mpfr_mul_2ui(two_pi, pi_hi, 1, MPFR_RNDU);

    // tan の極:
    // x = π/2 + kπ
    //
    // k = (x - π/2) / π
    mpfr_t a, b, k1, k2;
    mpfr_inits2(mpfr_get_prec(x.lo), a, b, k1, k2, (mpfr_ptr)0);

    mpfr_sub(a, x.lo, pi_hi, MPFR_RNDD);
    mpfr_mul_2ui(a, a, 1, MPFR_RNDD);
    mpfr_div(a, a, two_pi, MPFR_RNDD);

    mpfr_sub(b, x.hi, pi_lo, MPFR_RNDU);
    mpfr_mul_2ui(b, b, 1, MPFR_RNDU);
    mpfr_div(b, b, two_pi, MPFR_RNDU);

    mpfr_ceil(k1, a);
    mpfr_floor(k2, b);

    // 極を含むなら [-∞, +∞]
    if (mpfr_lessequal_p(k1, k2)) {
        mpfr_set_inf(res.lo, -1);
        mpfr_set_inf(res.hi,  1);

        mpfr_clears(pi_lo, pi_hi, two_pi,
                    a, b, k1, k2,
                    (mpfr_ptr)0);
        return res;
    }

    // 極を含まなければ tan は単調増加
    mpfr_t t;

    mpfr_init2(t, mpfr_get_prec(x.lo));
    mpfr_tan(res.lo, x.lo, MPFR_RNDD);
    mpfr_tan(t,     x.hi, MPFR_RNDD);
    mpfr_min(res.lo, res.lo, t, MPFR_RNDD);

    mpfr_tan(res.hi, x.hi, MPFR_RNDU);
    mpfr_tan(t,      x.lo, MPFR_RNDU);
    mpfr_max(res.hi, res.hi, t, MPFR_RNDU);

    mpfr_clears(pi_lo, pi_hi, two_pi,
                a, b, k1, k2, t,
                (mpfr_ptr)0);

    return res;
}
Interval acos(const Interval& x) {
    Interval res;

    mpfr_t neg_one, pos_one;
    mpfr_inits2(mpfr_get_prec(x.lo),
                neg_one, pos_one, (mpfr_ptr)0);

    mpfr_set_si(neg_one, -1, MPFR_RNDD);
    mpfr_set_si(pos_one,  1, MPFR_RNDU);

    // [x.lo, x.hi] と [-1,1] が交わらない
    if (mpfr_less_p(x.hi, neg_one) ||
        mpfr_greater_p(x.lo, pos_one)) {

        mpfr_set_nan(res.lo);
        mpfr_set_nan(res.hi);

        mpfr_clears(neg_one, pos_one, (mpfr_ptr)0);
        return res;
    }

    mpfr_t lo, hi;
    mpfr_inits2(mpfr_get_prec(x.lo), lo, hi, (mpfr_ptr)0);

    // 定義域 [-1,1] にクリップ
    mpfr_max(lo, x.lo, neg_one, MPFR_RNDD);
    mpfr_min(hi, x.hi, pos_one, MPFR_RNDU);

    // acos は単調減少
    mpfr_acos(res.lo, hi, MPFR_RNDD);
    mpfr_acos(res.hi, lo, MPFR_RNDU);

    mpfr_clears(neg_one, pos_one, lo, hi,
                (mpfr_ptr)0);

    return res;
}
enum class IntervalCmp {
    LESS,
    EQUAL,
    GREATER,
    OVERLAP
};

IntervalCmp compare(const Interval& a, const Interval& b) {
    if (mpfr_less_p(a.hi, b.lo))
        return IntervalCmp::LESS;

    if (mpfr_greater_p(a.lo, b.hi))
        return IntervalCmp::GREATER;

    if (mpfr_equal_p(a.lo, a.hi) &&
        mpfr_equal_p(b.lo, b.hi) &&
        mpfr_equal_p(a.lo, b.lo))
        return IntervalCmp::EQUAL;

    return IntervalCmp::OVERLAP;
}
Interval min(const Interval& a, const Interval& b) {
    Interval res;

    mpfr_min(res.lo, a.lo, b.lo, MPFR_RNDD);
    mpfr_min(res.hi, a.hi, b.hi, MPFR_RNDU);

    return res;
}

Interval max(const Interval& a, const Interval& b) {
    Interval res;

    mpfr_max(res.lo, a.lo, b.lo, MPFR_RNDD);
    mpfr_max(res.hi, a.hi, b.hi, MPFR_RNDU);

    return res;
}
Interval merge(const Interval&a,const Interval&b){
  Interval res;
  mpfr_min(res.lo,a.lo,b.lo,MPFR_RNDD);
  mpfr_max(res.hi,a.hi,b.hi,MPFR_RNDU);
  return res;
}
Interval abs(const Interval& x) {
    Interval res;
    mpfr_t zero;
    mpfr_init2(zero, mpfr_get_prec(x.lo));
    mpfr_set_zero(zero, 0);
    // x >= 0
    if (mpfr_greaterequal_p(x.lo, zero)) {
      mpfr_set(res.lo, x.lo, MPFR_RNDD);
      mpfr_set(res.hi, x.hi, MPFR_RNDU);
      return res;
    }

    // x <= 0
    if (mpfr_lessequal_p(x.hi, zero)) {
        mpfr_neg(res.lo, x.hi, MPFR_RNDD);
        mpfr_neg(res.hi, x.lo, MPFR_RNDU);
        return res;
    }
    // x.lo < 0 < x.hi
    mpfr_set_zero(res.lo, 0);

    mpfr_t a, b;
    mpfr_inits2(mpfr_get_prec(x.lo), a, b, (mpfr_ptr)0);

    mpfr_neg(a, x.lo, MPFR_RNDU);
    mpfr_set(b, x.hi, MPFR_RNDU);

    mpfr_max(res.hi, a, b, MPFR_RNDU);

    mpfr_clears(a, b, (mpfr_ptr)0);

    return res;
}