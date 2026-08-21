#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<vector>
#include "datastructure/range_set.hpp"
#include "random/generator.hpp"
void test(int n,std::vector<std::tuple<int,int,int>>query){
  std::vector<int>a(n);
  RangeSet<int>s;
  for(auto [t,l,r]:query){
    if(t==0){
      s.insert(l,r);
      for(int i=l;i<r;i++)a[i]=1;
    }
    else if(t==1){
      s.erase(l,r);
      for(int i=l;i<r;i++)a[i]=0;
    }
    else if(t==2){
      bool ans=true;
      for(int i=l;i<r;i++)ans&=a[i];
      assert(ans==s.contains(l,r));
    }
    assert(s.sum==std::reduce(a.begin(),a.end()));
  }
}
int main(){
  for(int t=0;t<10;t++){
    int n=t<=2?10:Random::range(1,3000);
    int q=Random::range(1,3000);
    std::vector<std::tuple<int,int,int>>query(q);
    for(int i=0;i<q;i++){
      int type=Random::range(3);
      auto [l,r]=Random::distinct(n+1);
      query[i]={type,l,r};
    }
    test(n,query);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}