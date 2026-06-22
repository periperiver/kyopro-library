#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include "fastio.hpp"
#include "datastructure/veb_tree.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::string t;
  rd(t);
  VanEmdeBoasTree veb(n);
  for(int i=0;i<n;i++)if(t[i]=='1')veb.insert(i);
  while(q--){
    int c,k;
    rd(c),rd(k);
    if(c==0)veb.insert(k);
    else if(c==1)veb.erase(k);
    else if(c==2)wt(veb.contains(k)),wt('\n');
    else if(c==3){
      int ans=veb.successor(k);
      if(ans>=n)ans=-1;
      wt(ans),wt('\n');
    }
    else{
      wt(veb.predecessor(k)),wt('\n');
    }
  }
}