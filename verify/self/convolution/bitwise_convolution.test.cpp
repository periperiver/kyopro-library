#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "convolution/bitwise_convolution.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename T>
std::vector<T>gen(int n){
  std::vector<T>res(1<<n);
  for(T&x:res)x=Random::range(T::mod());
  return res;
}
template<typename T>
std::vector<T>naive(std::vector<T>a,std::vector<T>b,int bit){
  std::vector<T>res(a.size());
  int op[2][2];
  op[0][0]=bit>>0&1;
  op[0][1]=bit>>1&1;
  op[1][0]=bit>>2&1;
  op[1][1]=bit>>3&1;
  int lg=msb(a.size());
  for(int i=0;i<(int)a.size();i++){
    for(int j=0;j<(int)b.size();j++){
      int k=0;
      for(int l=0;l<lg;l++)if(op[i>>l&1][j>>l&1])k|=1<<l;
      res[k]+=a[i]*b[j];
    }
  }
  return res;
}
template<typename T>
void test(int n){
  std::vector<T>a=gen<T>(n),b=gen<T>(n);
  for(int i=0;i<1<<4;i++){
    std::vector<T>c=bitwise_convolution(a,b,i);
    std::vector<T>d=naive(a,b,i);
    assert(c==d);
  }
}
int main(){
  for(int n=0;n<10;n++){
    for(int i=0;i<10;i++){
      test<mint998>(n);
      test<mint107>(n);
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}