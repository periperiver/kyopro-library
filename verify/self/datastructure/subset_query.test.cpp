#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "datastructure/subset_query.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename T>
void test1(int n,int q){
  SubsetQuery sq(n);
  std::vector<T>naive(1<<n);
  std::vector<T>f(1<<n);
  while(q--){
    int t=Random::range(2);
    if(t==0){
      int id=Random::range(1<<n);
      T val=Random::get<unsigned>();
      naive[id]+=val;
      sq.get_vs(id,[&](int x){f[x]+=val;});
    }
    else{
      int l=0,r=0;
      for(int i=0;i<n;i++){
        int j=Random::range(3);
        if(j==1)r|=1<<i;
        else if(j==2)l|=1<<i,r|=1<<i;
      }
      assert((l&r)==l);
      T ans=0,na=0;
      for(int i=0;i<(1<<n);i++){
        if((l&i)==l&&(r&i)==i)na+=naive[i];
      }
      sq.get_range(l,r,[&](int id,bool rev){
        if(rev)ans-=f[id];
        else ans+=f[id];
      });
      assert(ans==na);
    }
  }
}
template<typename T>
void test2(int n,int q){
  SubsetQuery sq(n);
  std::vector<T>naive(1<<n);
  std::vector<T>f(1<<n);
  while(q--){
    int t=Random::range(2);
    if(t==0){
      int id=Random::range(1<<n);
      T ans=0;
      T na=naive[id];
      sq.get_vs(id,[&](int x){ans+=f[x];});
      assert(ans==na);
    }
    else{
      int l=0,r=0;
      for(int i=0;i<n;i++){
        int j=Random::range(3);
        if(j==1)r|=1<<i;
        else if(j==2)l|=1<<i,r|=1<<i;
      }
      T val=Random::get<unsigned>();
      for(int i=0;i<(1<<n);i++){
        if((l&i)==l&&(r&i)==i)naive[i]+=val;
      }
      sq.get_range(l,r,[&](int id,bool rev){
        if(rev)f[id]-=val;
        else f[id]+=val;
      });
    }
  }
}
int main(){
  for(int n:{1,2,3,10,20}){
    for(int i=0;i<100;i++){
      int q=100;
      test1<unsigned>(n,q);
      test2<unsigned>(n,q);
      test1<mint998>(n,q);
      test2<mint998>(n,q);
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}