#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite"
#include<vector>
#include "datastructure/dynamic/linkcuttree.hpp"
#include "monoid/affine.hpp"
#include "math/modint.hpp"
using mint=mint998;
using M=MonoidAffine<mint>;
struct node{
  node *left=nullptr,*right=nullptr,*par=nullptr;
  M::S v,sum,mus;
  bool rev=false;
  void update(){
    sum=mus=v;
    if(left)sum=M::op(left->sum,sum),mus=M::op(mus,left->mus);
    if(right)sum=M::op(sum,right->sum),mus=M::op(right->mus,mus);
  }
  void reverse(){
    rev^=1;
    std::swap(left,right);
    std::swap(sum,mus);
  }
  void push(){
    if(rev){
      if(left)left->reverse();
      if(right)right->reverse();
      rev=false;
    }
  }
};
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<node>nds(n);
  for(int i=0;i<n;i++){
    mint a,b;
    std::cin>>a>>b;
    nds[i].v=nds[i].sum=nds[i].mus={a,b};
  }
  for(int i=0;i<n-1;i++){
    int u,v;
    std::cin>>u>>v;
    lct_link(&nds[u],&nds[v]);
  }
  while(q--){
    int t;
    std::cin>>t;
    if(t==0){
      int u,v,w,x;
      std::cin>>u>>v>>w>>x;
      lct_cut(&nds[u],&nds[v]);
      lct_link(&nds[w],&nds[x]);
    }
    else if(t==1){
      int p;
      mint c,d;
      std::cin>>p>>c>>d;
      lct_expose(&nds[p]);
      nds[p].v={c,d};
      nds[p].update();
    }
    else{
      int u,v;
      mint x;
      std::cin>>u>>v>>x;
      lct_expose(&nds[u]);
      nds[u].reverse();
      lct_expose(&nds[v]);
      auto [a,b]=nds[v].sum;
      std::cout<<a*x+b<<'\n';
    }
  }
}