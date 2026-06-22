#pragma once
#include "../datastructure/static_toptree.hpp"
#include<type_traits>
#include<functional>
struct MultiContourQuery{
private:
  int n;
  std::vector<std::pair<int,int>>ptr;
  std::vector<std::vector<int>>dstA,dstB;
  std::vector<int>dep;
  std::vector<std::vector<int>>query;
  StaticTopTree stt;
  int get_len(int u)const{return dstA[dep[u]][stt.B[u]];}
  static void check(int&nl,int&nr,int l,int r,int len){
    if(nl<l-len)nl=l-len;
    if(nr>r-len)nr=r-len;
  }
public:
  MultiContourQuery(){}
  template<typename T>
  explicit MultiContourQuery(Tree<T>t){
    assert(!t.is_directed());
    n=t.size();
    t.remove_parent();
    stt=StaticTopTree(t);
    dep.resize(n*2-1);
    for(int i=n*2-1;i-->n;)dep[stt.left[i]]=dep[stt.right[i]]=dep[i]+1;
    int dmax=*std::max_element(dep.begin(),dep.end());
    dstA.resize(dmax+1,std::vector<int>(n,-1));
    dstB.resize(dmax+1,std::vector<int>(n,-1));
    ptr.resize(n*2-1);
    query.resize(n*2-1);
    int vsc=0;
    std::vector<int>que(n+dmax+1);
    int p=0,q=0;
    auto dfs=[&](auto self,int v)->std::vector<int> {
      int d=dep[v];
      int lch=stt.left[v],rch=stt.right[v];
      std::vector<int>adj=v<n?std::vector<int>{v}:self(self,lch);
      std::vector<int>adj2=v<n?std::vector<int>{}:self(self,rch);
      if(v>=n&&stt.A[lch]==stt.A[rch])adj.insert(adj.end(),adj2.begin(),adj2.end());
      adj2.clear(),adj2.shrink_to_fit();
      p=0,q=0;
      for(int u:adj){
        dstA[d][u]=1;
        que[q++]=u;
      }
      while(p<q){
        int u=que[p++];
        for(const Edge<T>&e:t[u]){
          dstA[d][e.to]=dstA[d][u]+1;
          que[q++]=e.to;
        }
      }
      int len=dstA[d][que[q-1]]+1;
      ptr[v]=std::make_pair(vsc,len);
      vsc+=len;
      p=0,q=0;
      dstB[d][stt.B[v]]=0;
      que[q++]=stt.B[v];
      bool boundaryA=false;
      while(p<q){
        int u=que[p++];
        if(u<0){
          u=~u;
          for(int w:adj)if(w!=u){
            dstB[d][w]=dstB[d][u]+2;
            que[q++]=w;
          }
        }
        else{
          if(u!=stt.B[v]){
            for(const Edge<T>&e:t[u])if(dstB[d][e.to]==-1){
              dstB[d][e.to]=dstB[d][u]+1;
              que[q++]=e.to;
            }
          }
          int par=t.parent(u);
          if(par==stt.A[v]){
            if(!boundaryA){
              que[q++]=~u;
              boundaryA=true;
            }
          }
          else if(dstB[d][par]==-1){
            dstB[d][par]=dstB[d][u]+1;
            que[q++]=par;
          }
        }
      }
      return std::move(adj);
    };
    dfs(dfs,n*2-2);
  }
  template<typename Func>
  void get_vs(int v,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int,int)>>);
    int d=dep[v];
    int x=v;
    while(d>=0){
      f(ptr[v].first+dstA[d][x],dstB[d][x]);
      v=stt.par[v];
      d--;
    }
  }
  template<typename Func>
  void get_range(std::vector<std::tuple<int,int,int>>vlr,const Func&f){
    static_assert(std::is_convertible_v<Func,std::function<void(int,int,int,int)>>);
    auto dfs=[&](auto self,int u,int la,int ra,int lb,int rb)->void {
      if(la<0)la=0;
      if(lb<0)lb=0;
      if(ra>ptr[u].second)ra=ptr[u].second;
      if(la>=ra||lb>=rb)return;
      if(query[u].empty()){
        f(ptr[u].first+la,ptr[u].first+ra,lb,rb);
        return;
      }
      int d=dep[u];
      if(u<n){
        bool ok=la<=1&&1<ra&&lb<=0&&0<rb;
        for(int id:query[u]){
          const auto&[v,l,r]=vlr[id];
          ok&=l<=0&&0<r;
        }
        if(ok)f(ptr[u].first+1,ptr[u].first+2,0,1);
      }
      else{
        int lch=stt.left[u],rch=stt.right[u];
        int len1=get_len(lch),len2=get_len(rch);
        if(stt.B[lch]==stt.A[rch]){
          int nlb=lb-len2,nrb=rb-len2;
          for(int id:query[rch]){
            const auto&[v,l,r]=vlr[id];
            check(nlb,nrb,l,r,dstA[d+1][v]);
          }
          self(self,lch,la,ra,nlb,nrb);
          int nla=la-len1,nra=ra-len1;
          for(int id:query[lch]){
            const auto&[v,l,r]=vlr[id];
            check(nla,nra,l,r,dstB[d+1][v]);
          }
          self(self,rch,nla,nra,lb,rb);
        }
        else{
          int nla=la,nra=ra;
          for(int id:query[rch]){
            const auto&[v,l,r]=vlr[id];
            check(nla,nra,l,r,dstA[d+1][v]);
          }
          self(self,lch,nla,nra,lb,rb);
          nla=la,nra=ra;
          check(nla,nra,lb,rb,dstA[d+1][stt.B[lch]]);
          for(int id:query[lch]){
            const auto&[v,l,r]=vlr[id];
            check(nla,nra,l,r,dstA[d+1][v]);
          }
          self(self,rch,nla,nra,0,n);
        }
      }
    };
    for(int i=0;const auto&[v,l,r]:vlr){
      for(int x=v;x!=-1;x=stt.par[x])query[x].push_back(i);
      i++;
    }
    dfs(dfs,dep.size()-1,0,n+1,0,n+1);
    for(int i=0;const auto&[v,l,r]:vlr){
      for(int x=v;x!=-1;x=stt.par[x])query[x].clear();
      i++;
    }
  }
};