#define PROBLEM "https://judge.yosupo.jp/problem/persistent_range_affine_range_sum"
#include "fastio.hpp"
#include "datastructure/persistent/weight_balanced_tree.hpp"
#include "math/modint.hpp"
using mint=mint998;
struct node{
  using np=std::shared_ptr<node>;
  np left,right;
  int sz;
  mint a,b;
  mint sum;
  void propagate(mint aa,mint bb){
    b=b*aa+bb;
    a*=aa;
    sum=sum*aa+mint::raw(sz)*bb;
  }
  static np update(np lnd,np rnd){
    np res=std::make_shared<node>();
    res->left=lnd,res->right=rnd;
    res->sz=lnd->sz+rnd->sz;
    res->a=mint::raw(1),res->b=mint::raw(0);
    res->sum=lnd->sum+rnd->sum;
    return res;
  }
  static np clone(np nd){
    np res=std::make_shared<node>();
    *res=*nd;
    return res;
  }
  template<int need=3>
  static std::pair<np,np>push(np nd){
    np lnd=nullptr,rnd=nullptr;
    if(need&1){
      lnd=clone(nd->left);
      lnd->propagate(nd->a,nd->b);
    }
    if(need&2){
      rnd=clone(nd->right);
      rnd->propagate(nd->a,nd->b);
    }
    return {lnd,rnd};
  }
  static np single(mint x){
    np res=std::make_shared<node>();
    res->left=res->right=nullptr;
    res->sz=1;
    res->a=mint::raw(1),res->b=mint::raw(0);
    res->sum=x;
    return res;
  }
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<mint>a(n);
  for(mint&x:a){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  std::vector<PersistentWeightBalancedTree<node>>seg(q+1);
  seg[0]=PersistentWeightBalancedTree<node>(a);
  for(int i=1;i<=q;i++){
    int t;
    rd(t);
    if(t==0){
      int k,l,r,b,c;
      rd(k),rd(l),rd(r),rd(b),rd(c);
      k++;
      seg[i]=seg[k];
      auto [lnd,rnd]=seg[i].split(l);
      auto [rnd1,rnd2]=rnd.split(r-l);
      rnd1->propagate(b,c);
      seg[i]=lnd.merge(rnd1).merge(rnd2);
    }
    else if(t==1){
      int k,s,l,r;
      rd(k),rd(s),rd(l),rd(r);
      k++,s++;
      auto [lnd,rnd]=seg[k].split(l);
      rnd=rnd.split(r-l).second;
      auto mid=seg[s].split<2>(l).second.split<1>(r-l).first;
      seg[i]=lnd.merge(mid).merge(rnd);
    }
    else{
      int k,l,r;
      rd(k),rd(l),rd(r);
      k++;
      wt(seg[k].split(l).second.split(r-l).first->sum.val()),wt('\n');
    }
  }
}