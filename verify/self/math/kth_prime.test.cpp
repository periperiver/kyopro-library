#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "math/count_prime.hpp"
#include "math/kth_prime.hpp"
#include "math/prime_sieve.hpp"
#include "random/generator.hpp"
long long naive(long long k){
  auto check=[&](long long x)->bool {
    return count_prime(x)>=k;
  };
  long long e=k;
  while(!check(e))e*=2;
  long long ok=e,ng=e/2;
  while(ok-ng>1){
    long long mid=(ok+ng)/2;
    (check(mid)?ok:ng)=mid;
  }
  return ok;
}
void test(long long k){
  assert(kth_prime(k)==naive(k));
}
int main(){
  std::vector<int>primes=prime_sieve(100000000);
  for(int i=0;i<1000;i++){
    int k=Random::range(primes.size());
    assert(kth_prime(k+1)==primes[k]);
  }
  for(int t=0;t<5;t++){
    long long k=Random::range<long long>(1,10000000000ll);
    test(k);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}