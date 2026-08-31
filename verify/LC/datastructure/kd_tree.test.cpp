#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum"
#include "fastio.hpp"
#include "datastructure/kd_tree.hpp"
struct Query{
  int t;
  int lx,rx,ly,ry;
  int a,b;
  Query(){}
};
constexpr long long mod=998244353;
struct Monoid{
  struct M1{
    using S=std::pair<int,int>;
    static S op(S x,S y){
      int s=x.first+y.first;
      if(s>=mod)s-=mod;
      return {s,x.second+y.second};
    }
    static S e(){return {0,0};}
  };
  struct M2{
    using S=std::pair<int,int>;
    static S op(S x,S y){return {(long long)x.first*y.first%mod,((long long)x.second*y.first+y.second)%mod};}
    static S e(){return {1,0};}
  };
  static M1::S act(M1::S x,M2::S f){return {((long long)x.first*f.first+(long long)x.second*f.second)%mod,x.second};}
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<std::tuple<int,int,std::pair<int,int>>>init(n);
  std::vector<Query>query(q);
  for(int i=0;i<n;i++){
    int x,y,z;
    rd(x),rd(y),rd(z);
    init[i]={x,y,std::make_pair(z,1)};
  }
  for(int i=0;i<q;i++){
    int t;
    rd(t);
    if(t==0){
      int x,y,w;
      rd(x),rd(y),rd(w);
      init.emplace_back(x,y,std::make_pair(0,0));
      query[i].t=0;
      query[i].a=w;
    }
    else if(t==1){
      int x,w;
      rd(x),rd(w);
      query[i].t=1;
      query[i].lx=x;
      query[i].a=w;
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
      query[i].a=a,query[i].b=b;
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
      wt(seg.prod(a.lx,a.rx,a.ly,a.ry).first);
      wt('\n');
    }
    else{
      seg.apply(a.lx,a.rx,a.ly,a.ry,std::make_pair(a.a,a.b));
    }
  }
}