#define PROBLEM "https://judge.yosupo.jp/problem/stern_brocot_tree"
#include<iostream>
#include "math/stern_blocot_tree.hpp"
void solve(){
  std::string s;
  std::cin>>s;
  if(s=="ENCODE_PATH"){
    int a,b;
    std::cin>>a>>b;
    SternBlocotTree<int>sbt(a,b);
    auto ans=sbt.path();
    std::cout<<ans.size();
    for(int i=0;i<(int)ans.size();i++)std::cout<<' '<<ans[i].first<<' '<<ans[i].second;
    std::cout<<'\n';
  }
  else if(s=="DECODE_PATH"){
    int k;
    std::cin>>k;
    std::vector<std::pair<char,int>>path(k);
    for(auto&[a,b]:path)std::cin>>a>>b;
    SternBlocotTree<int>sbt(path);
    auto lr=sbt.val();
    std::cout<<lr.num<<' '<<lr.den<<'\n';
  }
  else if(s=="LCA"){
    int a,b,c,d;
    std::cin>>a>>b>>c>>d;
    SternBlocotTree<int>sbt1(a,b),sbt2(c,d);
    auto ans=SternBlocotTree<int>::lca(sbt1,sbt2).val();
    std::cout<<ans.num<<' '<<ans.den<<'\n';
  }
  else if(s=="ANCESTOR"){
    int k,a,b;
    std::cin>>k>>a>>b;
    SternBlocotTree<int>sbt(a,b);
    if(sbt.dep()<k)std::cout<<"-1\n";
    else{
      sbt=sbt.up(sbt.dep()-k);
      std::cout<<sbt.val().num<<' '<<sbt.val().den<<'\n';
    }
  }
  else if(s=="RANGE"){
    int a,b;
    std::cin>>a>>b;
    SternBlocotTree<int>sbt(a,b);
    auto ans=sbt.range();
    std::cout<<ans.first.num<<' '<<ans.first.den<<' '<<ans.second.num<<' '<<ans.second.den<<'\n';
  }
  else assert(false);
}
int main(){
  int t;
  std::cin>>t;
  while(t--)solve();
}