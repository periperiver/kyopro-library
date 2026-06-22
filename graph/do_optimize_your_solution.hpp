#pragma once
#include "../datastructure/static_toptree.hpp"
#include "lca.hpp"
//sum_{0≤i<j<n}dist1(i,j)*dist2(i,j)*w[i]*w[j]
template<typename T,typename U>
U do_optimize_your_solution(Tree<T>t1,Tree<T>t2,std::vector<U>w){
  static_assert(std::is_convertible_v<T,U>);
  assert(t1.size()==t2.size()&&t1.size()==w.size());
  struct Data{
    U sum1,sum2,sz1,sz2;
    U merge(const Data&rhs){
      U res=sum1*rhs.sz2+sum2*rhs.sz1+sz1*rhs.sum2+sz2*rhs.sum1;
      sum1+=rhs.sum1;
      sum2+=rhs.sum2;
      sz1+=rhs.sz1;
      sz2+=rhs.sz2;
      return res+res;
    }
    inline void clear(){sum1=sum2=sz1=sz2=U();}
    inline void set1(U sum,U sz){sum1=sum,sz1=sz,sum2=sz2=U();}
    inline void set2(U sum,U sz){sum2=sum,sz2=sz,sum1=sz1=U();}
  };
  int n=t1.size();
  if(n==0)return 0;
  t1.remove_parent(),t2.remove_parent();
  StaticTopTree stt(t1);
  LowestCommonAncestor lca1(t1),lca2(t2);
  std::vector<int>in=t2.in_out_order().first;
  std::vector<int>dep(n);
  std::vector<U>wdep1(n),wdep2(n);
  for(int x:t1.bfs_order()){
    for(const Edge<T>&e:t1[x]){
      wdep1[e.to]=wdep1[e.from]+U(e.weight);
    }
  }
  for(int x:t2.bfs_order()){
    for(const Edge<T>&e:t2[x]){
      dep[e.to]=dep[e.from]+1;
      wdep2[e.to]=wdep2[e.from]+U(e.weight);
    }
  }
  U res=0;
  std::vector<int>buf[2];
  buf[0]=buf[1]=std::vector<int>(n);
  std::vector<int>st;
  std::vector<Data>dp(n);
  std::vector<U>dst(n);
  st.reserve(n);
  auto query1=[&](int u,int v)->U {
    return wdep1[u]+wdep1[v]-wdep1[lca1.query(u,v)]*2;
  };
  auto dfs=[&](auto self,int nd,int t,int l)->int {
    if(nd<n){
      buf[t][l]=nd;
      return 1;
    }
    int szl=self(self,stt.left[nd],t^1,l);
    int szr=self(self,stt.right[nd],t^1,l+szl);
    U wsum1=0,wsum2=0;
    U wdsum1=0,wssum1=0,wdsum2=0,wssum2=0;
    int g=stt.A[stt.right[nd]];
    for(int i=0;i<szl;i++){
      int u=buf[t^1][l+i];
      wsum1+=w[u];
      dst[u]=query1(g,u)*w[u];
      wdsum1+=dst[u];
      wssum1+=w[u]*wdep2[u];
      dp[u].set1(dst[u],w[u]);
    }
    for(int i=0;i<szr;i++){
      int v=buf[t^1][l+szl+i];
      wsum2+=w[v];
      dst[v]=query1(g,v)*w[v];
      wdsum2+=dst[v];
      wssum2+=w[v]*wdep2[v];
      dp[v].set2(dst[v],w[v]);
    }
    res+=wdsum1*wssum2+wdsum2*wssum1;
    for(int i=0;i<szl;i++){
      int u=buf[t^1][l+i];
      res+=wsum2*dst[u]*wdep2[u];
    }
    for(int i=0;i<szr;i++){
      int v=buf[t^1][l+szl+i];
      res+=wsum1*dst[v]*wdep2[v];
    }
    int sz=szl+szr;
    std::merge(buf[t^1].begin()+l,buf[t^1].begin()+l+szl,buf[t^1].begin()+l+szl,buf[t^1].begin()+l+szl+szr,buf[t].begin()+l,[&](int lhs,int rhs){
      return in[lhs]<in[rhs];
    });
    st.push_back(buf[t][l]);
    for(int i=1;i<sz;i++){
      int wv=lca2.query(buf[t][l+i-1],buf[t][l+i]);
      int last=-1;
      while(!st.empty()&&dep[st.back()]>dep[wv]){
        int v=st.back();
        if(last!=-1)res-=dp[v].merge(dp[last])*wdep2[v];
        last=v;
        st.pop_back();
      }
      if(st.back()!=wv){
        st.push_back(wv);
        dp[wv].clear();
      }
      if(last!=-1)res-=dp[wv].merge(dp[last])*wdep2[wv];
      st.push_back(buf[t][l+i]);
    }
    for(int i=(int)st.size()-1;i>=1;i--){
      int c=st[i],p=st[i-1];
      res-=dp[p].merge(dp[c])*wdep2[p];
    }
    st.clear();
    return sz;
  };
  dfs(dfs,n*2-2,0,0);
  return res;
}