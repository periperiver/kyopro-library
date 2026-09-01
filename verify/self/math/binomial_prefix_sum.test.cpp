#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/binomial_prefix_sum.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename T>
void test_small(int n){
  std::vector<std::vector<T>>a(n+1,std::vector<T>(n+1));
  for(int i=0;i<=n;i++){
    a[i][0]=a[i][i]=1;
    for(int j=1;j<i;j++)a[i][j]=a[i-1][j-1]+a[i-1][j];
  }
  for(int i=0;i<=n;i++)for(int j=1;j<=i;j++)a[i][j]+=a[i][j-1];
  for(int t=0;t<200;t++){
    int i=Random::range(n+1);
    int j=Random::range(i+1);
    int k=Random::range(i+1);
    if(j>k)std::swap(j,k);
    assert(BinomialPrefixSum<T>::get(i,j,k)==a[i][k]-(j==0?0:a[i][j-1]));
  }
}
template<typename T>
void test(int n,int k){
  T a=0;
  for(int i=0;i<=k;i++)a+=F<T>::C(n,i);
  assert(a==BinomialPrefixSum<T>::get(n,k));
}
int main(){
  test_small<mint998>(2000);
  test_small<mint107>(2000);
  test_small<mint61>(2000);
  for(int t=0;t<100;t++){
    int n=Random::range(200000);
    int k=Random::range(n+1);
    test<mint998>(n,k);
    test<mint107>(n,k);
    test<mint61>(n,k);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}