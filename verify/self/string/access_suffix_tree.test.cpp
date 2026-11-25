#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "string/access_suffix_tree.hpp"
#include "random/generator.hpp"
#include<iostream>
#include<cassert>
#include "io.hpp"
std::vector<int>gen(int n,int sigma){
  std::vector<int>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range(sigma);
  return res;
}
template<int sigma>
void test(std::vector<int>s,int q){
  int n=s.size();
  AccessSuffixTree<sigma>st(s);
  while(q--){
    auto [l,r]=Random::distinct(n+1);
    int v=l;
    int len=n-r;
    while(true){
      if(len<st[v].edge_length())break;
      len-=st[v].edge_length();
      v=st[v].par;
    }
    assert(v==st.query(l,r));
  }
}
int main(){
  for(int n=1;n<=200;n++){
    for(int t=0;t<10;t++){
      test<2>(gen(n,2),100);
      test<10>(gen(n,10),100);
      test<26>(gen(n,26),100);
      test<64>(gen(n,64),100);
    }
  }
  test<64>(gen(10000,64),1000);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}