#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<optional>
#include "arbitrary_modint.hpp"
#include "arbitrary_modint64.hpp"
#include "other/type.hpp"
#include "primefactor.hpp"
#include "random/generator.hpp"
#include "matrix/arbitrary_linear_equations.hpp"
template<typename T,typename T2>
std::enable_if_t<is_modint_v<T2>,typename T::value_type>index_calculus(T a,T b,typename T::value_type ord){
  using I=typename T::value_type;
  static constexpr int prime[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213,2221,2237,2239,2243,2251,2267,2269,2273,2281,2287,2293,2297,2309,2311,2333,2339,2341,2347,2351,2357,2371,2377,2381,2383,2389,2393,2399,2411,2417,2423,2437,2441,2447,2459,2467,2473,2477,2503,2521,2531,2539,2543,2549,2551,2557,2579,2591,2593,2609,2617,2621,2633,2647,2657,2659,2663,2671,2677,2683,2687,2689,2693,2699,2707,2711,2713,2719,2729,2731,2741,2749,2753,2767,2777,2789,2791,2797,2801,2803,2819,2833,2837,2843,2851,2857,2861,2879,2887,2897,2903,2909,2917,2927,2939,2953,2957,2963,2969,2971,2999,3001,3011,3019,3023,3037,3041,3049,3061,3067,3079,3083,3089,3109,3119,3121,3137,3163,3167,3169,3181,3187,3191,3203,3209,3217,3221,3229,3251,3253,3257,3259,3271,3299,3301,3307,3313,3319,3323,3329,3331,3343,3347,3359,3361,3371,3373,3389,3391,3407,3413,3433,3449,3457,3461,3463,3467,3469,3491,3499,3511,3517,3527,3529,3533,3539,3541,3547,3557,3559,3571,3581,3583,3593,3607,3613,3617,3623,3631,3637,3643,3659,3671,3673,3677,3691,3697,3701,3709,3719,3727,3733,3739,3761,3767,3769,3779,3793,3797,3803,3821,3823,3833,3847,3851,3853,3863,3877,3881,3889,3907,3911,3917,3919,3923,3929,3931,3943,3947,3967,3989};
  static constexpr int p_size=550;
  const I p=T::mod();
  const int sz=std::pow(p-1,1/(2*std::sqrt(std::log(p-1)/std::log(std::log(p-1)))));
  if constexpr (is_dynamic_modint_v<T2>)T2::set_mod(p-1);
  else static_assert(T::mod()==T2::mod()+1);
  ArbitraryLinearEquations<T2>solver(sz+1);
  I res;
  while(true){
    I k=Random::range(ord);
    I now=(b*a.pow(k)).val();
    std::vector<T2>coef(sz+2);
    coef.back()=k;
    for(int i=0;i<sz;i++){
      while(now%prime[i]==0){
        now/=prime[i];
        coef[i]++;
      }
    }
    if(now!=1)continue;
    coef[sz]=-1;
    solver.add(coef);
    auto logb=solver.calc_back();
    if(logb){
      res=(*logb).val();
      break;
    }
  }
  return res%ord;
}
template<typename T>
std::enable_if_t<is_modint_v<T>,std::optional<typename T::value_type>>discrete_logarithm_naive(T a,T b,typename T::value_type ord){
  T now=T::raw(1);
  for(typename T::value_type i=0;i<ord;i++){
    if(now==b)return std::make_optional(i);
    now*=a;
  }
  return std::nullopt;
}
template<typename T>
std::enable_if_t<std::is_integral_v<T>,std::optional<T>>discrete_logarithm_prime(T a,T b,T p){
  a%=p,b%=p;
  if(a<0)a+=p;
  if(b<0)b+=p;
  if(b==0)return a==0?std::make_optional(1):std::nullopt;
  if(b==1)return std::make_optional(0);
  if(a<=1)return std::nullopt;
  auto f=factorize(p-1);
  std::sort(f.begin(),f.end());
  std::vector<T>left,middle,right;
  for(int l=0,r=0;l<f.size();l=r){
    T now=1;
    while(r<f.size()&&f[l]==f[r])r++,now*=f[l];
    left.push_back(now),right.push_back(now);
    middle.push_back(f[l]);
  }
  for(int i=1;i<left.size();i++)left[i]*=left[i-1];
  for(int i=right.size();i-->1;)right[i-1]*=right[i];
  T ord_a=1,ord_b=1;
  if(p<(1<<30)){
    using mint=arbitrary_modint<20241125>;
    using mint2=arbitrary_modint<20241126>;
    mint::set_mod(p);
    for(int i=0;i<middle.size();i++){
      T v=1;
      if(i!=0)v*=left[i-1];
      if(i+1!=middle.size())v*=right[i+1];
      mint now=mint(a).pow(v);
      while(now.val()!=1){
        now=now.pow(middle[i]);
        ord_a*=middle[i];
      }
      now=mint(b).pow(v);
      while(now.val()!=1){
        now=now.pow(middle[i]);
        ord_b*=middle[i];
      }
    }
    if(ord_a<=60)return discrete_logarithm_naive<mint>(a,b,ord_a);
    if(ord_a%ord_b!=0)return std::nullopt;
    else return std::make_optional(index_calculus<mint,mint2>(a,b,ord_a));
  }
  else{
    using mint=arbitrary_modint64<20241125>;
    using mint2=arbitrary_modint64<20241126>;
    mint::set_mod(p);
    for(int i=0;i<middle.size();i++){
      T v=1;
      if(i!=0)v*=left[i-1];
      if(i+1!=middle.size())v*=right[i+1];
      mint now=mint(a).pow(v);
      while(now.val()!=1){
        now=now.pow(middle[i]);
        ord_a*=middle[i];
      }
      now=mint(b).pow(v);
      while(now.val()!=1){
        now=now.pow(middle[i]);
        ord_b*=middle[i];
      }
    }
    if(ord_a%ord_b!=0)return std::nullopt;
    else return std::make_optional(index_calculus<mint,mint2>(a,b,ord_a));
  }
}