#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum"
#include "fastio.hpp"
#include "datastructure/kd_tree.hpp"
#include "math/modint.hpp"
using mint=mint998;
struct Query{
  int t;
  int lx,rx,ly,ry;
  mint a,b;
  Query(){}
};
struct Monoid{
  using S=std::pair<mint,int>;
  using F=std::pair<mint,mint>;
  static S op(S x,S y){return {x.first+y.first,x.second+y.second};}
  static S e(){return {0,0};}
  static S mapping(F f,S x,long long){return {x.first*f.first+x.second*f.second,x.second};}
  static F composition(F f,F g){return {f.first*g.first,f.second+f.first*g.second};}
  static F id(){return {mint::raw(1),mint::raw(0)};}
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<std::tuple<int,int,std::pair<mint,int>>>init(n);
  std::vector<Query>query(q);
  for(int i=0;i<n;i++){
    int x,y,z;
    rd(x),rd(y),rd(z);
    init[i]={x,y,std::make_pair(mint::raw(z),1)};
  }
  for(int i=0;i<q;i++){
    int t;
    rd(t);
    if(t==0){
      int x,y,w;
      rd(x),rd(y),rd(w);
      init.emplace_back(x,y,std::make_pair(mint::raw(0),0));
      query[i].t=0;
      query[i].a=mint::raw(w);
    }
    else if(t==1){
      int x,w;
      rd(x),rd(w);
      query[i].t=1;
      query[i].lx=x;
      query[i].a=mint::raw(w);
    }
    else if(t==2){
      int lx,rx,ly,ry;
      rd(lx),rd(ly),rd(rx),rd(ry);
      query[i].t=2;
      query[i].lx=lx,query[i].rx=rx,query[i].ly=ly,query[i].ry=ry;
    }
    else{
      int lx,rx,ly,ry,a,b;
      rd(lx),rd(ly),rd(rx),rd(ry),rd(a),rd(b);
      query[i].t=3;
      query[i].lx=lx,query[i].rx=rx,query[i].ly=ly,query[i].ry=ry;
      query[i].a=mint::raw(a),query[i].b=mint::raw(b);
    }
  }
  kdTree<int,Monoid>seg(init);
  int p=n;
  for(const Query&a:query){
    if(a.t==0){
      seg.set(p++,std::make_pair(a.a,1));
    }
    else if(a.t==1){
      seg.set(a.lx,std::make_pair(a.a,1));
    }
    else if(a.t==2){
      wt(seg.prod(a.lx,a.rx,a.ly,a.ry).first.val());
      wt('\n');
    }
    else{
      seg.apply(a.lx,a.rx,a.ly,a.ry,std::make_pair(a.a,a.b));
    }
  }
}