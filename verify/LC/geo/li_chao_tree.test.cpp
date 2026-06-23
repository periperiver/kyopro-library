#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"
#include "fastio.hpp"
#include "geo/li_chao_tree.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  LiChaoTree<long long>lct(-1ll<<30,1ll<<30);
  for(int i=0;i<n;i++){
    int l,r;
    long long a,b;
    rd(l),rd(r),rd(a),rd(b);
    lct.add_segment(l,r,a,b);
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int l,r;
      long long a,b;
      rd(l),rd(r),rd(a),rd(b);
      lct.add_segment(l,r,a,b);
    }
    else{
      long long x;
      rd(x);
      long long ans=lct.query(x);
      if(ans>9e18)wt("INFINITY\n");
      else wt(ans),wt('\n');
    }
  }
}