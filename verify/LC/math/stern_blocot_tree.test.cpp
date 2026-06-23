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
    rep(i,ans.size())cout<<' '<<ans[i];
    cout<<endl;
  }
  else if(s=="DECODE_PATH"){
    int k;
    std::cin>>k;
    std::vector<std::pair<char,int>>path(k);
    std::cin>>path;
    SternBlocotTree<int>sbt(path);
    auto lr=sbt.val();
    cout<<lr.num<<' '<<lr.den<<endl;
  }
  else if(s=="LCA"){
    int a,b,c,d;
    cin>>a>>b>>c>>d;
    SternBlocotTree<int>sbt1(a,b),sbt2(c,d);
    auto ans=SternBlocotTree<int>::lca(sbt1,sbt2).val();
    cout<<ans.num<<' '<<ans.den<<endl;
  }
  else if(s=="ANCESTOR"){
    int k,a,b;
    cin>>k>>a>>b;
    SternBlocotTree<int>sbt(a,b);
    if(sbt.dep()<k)cout<<-1<<endl;
    else{
      sbt=sbt.up(sbt.dep()-k);
      cout<<sbt.val().num<<' '<<sbt.val().den<<endl;
    }
  }
  else if(s=="RANGE"){
    int a,b;
    cin>>a>>b;
    SternBlocotTree<int>sbt(a,b);
    auto ans=sbt.range();
    cout<<ans.first.num<<' '<<ans.first.den<<' '<<ans.second.num<<' '<<ans.second.den<<endl;
  }
  else assert(false);
}
int main(){
  int t;
  std::cin>>t;
  while(t--)solve();
}