#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "string/suffix_tree.hpp"
#include "random/generator.hpp"
#include<iostream>
#include<cassert>
std::vector<int>gen(int n,int sigma){
  std::vector<int>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range(sigma);
  return res;
}
template<int sigma>
void test(std::vector<int>s){
  SuffixTree<sigma>st(s);
  for(int i=0;i<(int)st.size();i++){
    for(int j=0;j<sigma;j++){
      if(st[i][j]!=-1){
        int c=st[i][j];
        assert(st[c].par==i);
      }
    }
  }
  for(int i=0;i<(int)s.size();i++)assert(st[i].len==(int)s.size()-i);
  assert(st[s.size()].par==-1);
  assert(st[s.size()].range==std::make_pair(-1,-1));
  auto dfs=[&](auto self,int nd,std::vector<int> t)->void {
    if(nd<(int)s.size()){
      assert(nd+(int)t.size()==(int)s.size());
      for(int i=0;i<(int)t.size();i++)assert(s[nd+i]==t[i]);
    }
    for(int j=0;j<sigma;j++){
      if(st[nd][j]!=-1){
        int c=st[nd][j];
        auto [l,r]=st[c].range;
        std::vector<int>nt(t);
        nt.insert(nt.end(),s.begin()+l,s.begin()+r);
        self(self,c,nt);
      }
    }
  };
  dfs(dfs,s.size(),{});
}
int main(){
  for(int n=1;n<=100;n++){
    for(int t=0;t<20;t++){
      test<2>(gen(n,2));
      test<10>(gen(n,10));
      test<26>(gen(n,26));
      test<64>(gen(n,64));
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}