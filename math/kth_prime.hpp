#pragma once
#include<cmath>
#include<cassert>
#include "count_prime.hpp"
#include "primality_test.hpp"
long long kth_prime(long long k){
  assert(1<=k);
  long long res=k;
  long long diff=count_prime(res)-k;
  while(std::abs(diff)>10){
    double lg=std::log(res);
    double dx=(lg-1)/(lg*lg);
    res-=diff/dx;
    diff=count_prime(res)-k;
  }
  while(diff>=0){
    diff-=isprime(res);
    res--;
  }
  while(diff<0){
    res++;
    if(isprime(res)){
      diff++;
      if(diff==0)break;
    }
  }
  return res;
}