#define PROBLEM "https://judge.yosupo.jp/problem/persistent_unionfind"
#include "fastio.hpp"
#include "datastructure/persistent/unionfind.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<PersistentUnionFind>uf(q+1);
  for(int i=1;i<=q;i++){
    int t,k,u,v;
    rd(t),rd(k),rd(u),rd(v);
    if(t==0)uf[i]=uf[k].merge(u,v);
    else wt(uf[k].same(u,v)),wt('\n');
  }
}