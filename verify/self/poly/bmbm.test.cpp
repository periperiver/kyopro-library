#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "poly/bmbm.hpp"
#include "math/arbitrary_modint.hpp"
#include "random/generator.hpp"
#include "matrix/matrix.hpp"
using mint=arbitrary_modint<1>;
std::pair<std::vector<mint>,mint>gen(int n,long long k){
  std::vector<mint>coef(n);
  for(int i=0;i<n;i++)coef[i]=Random::range(mint::mod());
  std::vector<mint>res(n*2+Random::range(10));
  res[0]=Random::range(mint::mod());
  for(int i=1;i<(int)res.size();i++){
    for(int j=0;j<(int)coef.size();j++){
      if(i-j-1<0)break;
      res[i]+=res[i-j-1]*coef[j];
    }
  }
  std::vector<std::vector<mint>>mat(n,std::vector<mint>(n));
  mat[0]=coef;
  for(int i=1;i<n;i++)mat[i][i-1]=1;
  mat=matrix_pow(mat,k);
  mint v=0;
  for(int i=0;i<n;i++)v+=mat[n-1][i]*res[n-1-i];
  return std::make_pair(res,v);
}
void test(int n,long long k,int mod){
  mint::set_mod(mod);
  auto [a,v]=gen(n,k);
  assert(kth_term(a,k)==v);
}
int main(){
  for(int mod=1;mod<=100;mod++){
    for(int n=1;n<10;n++){
      test(n,Random::range(1000000000000000000),mod);
    }
  }
  for(int i=0;i<100;i++){
    test(Random::range(1,50),Random::range(1000000000000000000),Random::range(1,1000000000));
  }
  for(int mod:{360,223092870,423263232,1000000000}){
    for(int i=0;i<100;i++){
      test(Random::range(1,50),Random::range(1000000000000000000),mod);
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}