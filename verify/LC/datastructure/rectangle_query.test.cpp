#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include "fastio.hpp"
#include "datastructure/rectangle_query.hpp"
#include "datastructure/bit.hpp"
#include "monoid/add.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<std::pair<int,int>>point(n);
  std::vector<int>val(n);
  for(int i=0;i<n;i++){
    int x,y,v;
    rd(x),rd(y),rd(v);
    point[i]={x,y};
    val[i]=v;
  }
  std::vector<std::tuple<int,int,int,int>>query(q);
  for(int i=0;i<q;i++){
    int t;
    rd(t);
    if(t==0){
      int x,y,w;
      rd(x),rd(y),rd(w);
      query[i]={-1,x,y,w};
      point.emplace_back(x,y);
    }
    else{
      int l,d,r,u;
      rd(l),rd(d),rd(r),rd(u);
      query[i]={l,d,r,u};
    }
  }
  RectangleQuery<int>rq(point);
  std::vector<long long>init(rq.size());
  for(int i=0;i<n;i++)rq.get_vs(point[i].first,point[i].second,[&](int x){init[x]+=val[i];});
  BinaryIndexedTree<MonoidAdd<long long>>BIT(std::move(init));
  for(auto [a,b,c,d]:query){
    if(a==-1)rq.get_vs(b,c,[&](int x){BIT.add(x,d);});
    else{
      long long ans=0;
      rq.get_range(a,c,b,d,[&](int l,int r){ans+=BIT.sum(l,r);});
      wt(ans),wt('\n');
    }
  }
}