#pragma once
#include "lca.hpp"
template<typename M>
struct SparseTableTree{
private:
  using S=typename M::S;
  std::vector<std::vector<S>>sum,mus;
  LowestCommonAncestor lca;
  std::vector<int>dep;
public:
  SparseTableTree(){}
  SparseTableTree(const Tree<S>&t){
    int n=t.size();
    Tree<>g(n);
    std::vector<bool>dead(n,false);
    std::vector<int>sub(n);
    dep.resize(n);
    auto calc_sub=[&](auto self,int x,int p)->void {
      sub[x]=1;
      for(const Edge<S>&u:t[x])if(u.to!=p&&!dead[u.to]){
        self(self,u.to,x);
        sub[x]+=sub[u.to];
      }
    };
    auto centroid=[&](auto self,int x,int p,int s)->int {
      for(const Edge<S>&u:t[x])if(u.to!=p&&!dead[u.to]&&sub[u.to]>s)return self(self,u.to,x,s);
      return x;
    };
    auto dfs=[&](auto self,int x,int d)->int {
      calc_sub(calc_sub,x,-1);
      int c=centroid(centroid,x,-1,sub[x]/2);
      if(d==sum.size()){
        sum.push_back(std::vector<S>(n,M::e()));
        mus.push_back(std::vector<S>(n,M::e()));
      }
      dep[c]=d;
      dead[c]=true;
      for(const Edge<S>&u:t[c])if(!dead[u.to]){
        int ch=self(self,u.to,d+1);
        g.add_edge(c,ch);
      }
      auto calc=[&](auto self,int x,int p,S now,S won)->void {
        sum[d][x]=now;
        mus[d][x]=won;
        for(const Edge<S>&u:t[x])if(u.to!=p&&!dead[u.to])self(self,u.to,x,M::op(now,u.weight),M::op(u.weight,won));
      };
      dead[c]=false;
      calc(calc,c,-1,M::e(),M::e());
      return c;
    };
    int root=dfs(dfs,0,0);
    g.build();
    g.remove_parent(root);
    lca=LowestCommonAncestor(g);
  }
  S prod(int u,int v)const{
    if(u==v)return M::e();
    int l=lca.query(u,v);
    S left=mus[dep[l]][u];
    S right=sum[dep[l]][v];
    return M::op(left,right);
  }
};