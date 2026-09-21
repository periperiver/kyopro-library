#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "poly/fps2d_kth_inv.hpp"
#include "random/generator.hpp"
#include "math/modint.hpp"
#include "io.hpp"
using mint1=mint998;
using mint2=modint<469762049>;
template<typename T>
std::vector<T>naive(fps2d<T>p,fps2d<T>q,int k,int deg){
  q.resize(k+1,deg);
  p/=q;
  std::vector<T>res(deg);
  for(int i=0;i<deg;i++)res[i]=p[k][i];
  return res;
}
template<typename T>
void test(fps2d<T>p,fps2d<T>q,int k,int deg){
  assert(fps2d_kth_inv(p,q,k,deg)==naive(p,q,k,deg));
}
template<typename T>
fps2d<T>gen(int n,int m,bool zero){
  fps2d<T>res(n,m);
  for(int i=0;i<n;i++)for(int j=0;j<m;j++){
    res[i][j]=Random::range(T::mod());
  }
  if(zero)res[0][0]=Random::range<int>(1,T::mod());
  return res;
}
template<typename T>
fps2d<T>sparse(int n,int m,bool zero){
  fps2d<T>res(n,m);
  int t=5;
  while(t--){
    int i=Random::range(n),j=Random::range(m);
    res[i][j]+=Random::range(T::mod());
  }
  if(zero)res[0][0]=Random::range<int>(1,T::mod());
  return res;
}
int main(){
  for(int n=1;n<=10;n++)for(int m=1;m<=10;m++){
    for(int k=0;k<=20;k++){
      int deg=Random::range(1,30);
      test<mint1>(gen<mint1>(n,m,false),gen<mint1>(n,m,true),k,deg);
      test<mint2>(gen<mint2>(n,m,false),gen<mint2>(n,m,true),k,deg);
      test<mint1>(sparse<mint1>(n,m,false),sparse<mint1>(n,m,true),k,deg);
      test<mint2>(sparse<mint2>(n,m,false),sparse<mint2>(n,m,true),k,deg);
    }
  }
  for(int t=0;t<100;t++){
    int n1=Random::range(1,30),m1=Random::range(1,30);
    int n2=Random::range(1,30),m2=Random::range(1,30);
    int k=Random::range(100);
    int deg=Random::range(1,100);
    test<mint1>(gen<mint1>(n1,m1,false),gen<mint1>(n2,m2,true),k,deg);
    test<mint2>(gen<mint2>(n1,m1,false),gen<mint2>(n2,m2,true),k,deg);
    test<mint1>(sparse<mint1>(n1,m1,false),sparse<mint1>(n2,m2,true),k,deg);
    test<mint2>(sparse<mint2>(n1,m1,false),sparse<mint2>(n2,m2,true),k,deg);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}