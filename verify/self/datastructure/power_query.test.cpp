#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "datastructure/power_query.hpp"
#include "matrix/constant.hpp"
#include "monoid/affine.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
using mint=mint998;
constexpr int mat_size=3;
struct M1{
  using S=ConstantMatrix<mint,mat_size>;
  static S op(S x,S y){return x*y;}
  static S e(){return S::E();}
  static S pow(S x,long long k){
    S res=e();
    while(k&1){
      if(k&1)res=op(res,x);
      x=op(x,x);
      k>>=1;
    }
    return res;
  }
};
using M2=MonoidAffine<mint>;
template<typename M>
void test(typename M::S x){
  PowerQuery<M>power(x);
  for(int i=0;i<1000;i++){
    long long k=Random::range(1ll<<60);
    assert(power(k)==M::pow(x,k));
  }
}
int main(){
  for(int i=0;i<100;i++){
    M1::S x;
    for(int i=0;i<mat_size;i++)for(int j=0;j<mat_size;j++)x[i][j]=Random::range(mint::mod());
    test<M1>(x);
  }
  for(int i=0;i<100;i++){
    M2::S x;
    x.first=Random::range(mint::mod());
    x.second=Random::range(mint::mod());
    test<M2>(x);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}