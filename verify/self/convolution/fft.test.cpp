#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "convolution/fft.hpp"
#include "random/generator.hpp"
std::vector<double>naive(std::vector<double>a,std::vector<double>b){
  std::vector<double>res(a.size()+b.size()-1);
  for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)res[i+j]+=a[i]*b[j];
  return res;
}
double emax=0;
void test(int n,int m){
  std::vector<double>a(n),b(m);
  for(int i=0;i<n;i++){
    a[i]=Random::get<double>()*1000000;
  }
  for(int i=0;i<m;i++){
    b[i]=Random::get<double>()*1000000;
  }
  std::vector<double>ans=fft_convolution(a,b);
  std::vector<double>na=naive(a,b);
  assert(ans.size()==na.size());
  for(int i=0;i<(int)ans.size();i++){
    double d=std::abs(ans[i]-na[i])/std::max((double)1,na[i]);
    emax=std::max(emax,d);
    assert(d<1e-8);
  }
}
int main(){
  for(int i=0;i<100;i++){
    test(Random::range(1,5000),Random::range(1,5000));
  }
  for(int n=1;n<=10;n++){
    for(int i=0;i<10;i++){
      test(n,Random::range(1,1000));
      test(Random::range(1,1000),n);
    }
  }
  for(int n=1;n<=10;n++)for(int m=1;m<=10;m++)test(n,m);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}