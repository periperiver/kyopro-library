#pragma once
#include "../datastructure/static_toptree.hpp"
#include<type_traits>
#include<functional>
struct ContourQuery{
private:
  std::vector<std::pair<int,int>>ptr;
  std::vector<std::vector<int>>dst;
  std::vector<int>stt_par,dep,lr;
  int vsc;
public:
  ContourQuery(){}
  template<typename T>
  ContourQuery(Tree<T>t){
    assert(!t.is_directed());
    int n=t.size();
    t.remove_parent();
    StaticTopTree stt(t);
    dep.resize(n*2-1);
    for(int i=n*2-1;i-->n;){
      dep[stt.left[i]]=dep[stt.right[i]]=dep[i]+1;
    }
    dst.resize(*std::max_element(dep.begin(),dep.begin()+n),std::vector<int>(n,-1));
    ptr.resize(n*2-1);
    std::vector<int>que(n+dst.size());
    int p=0,q=0;
    vsc=n;
    auto dfs=[&](auto self,int v)->std::vector<int> {
      if(v<n){
        return std::vector<int>{v};
      }
      const int d=dep[v];
      int lv=stt.left[v],rv=stt.right[v];
      std::vector<int>lch=self(self,lv);
      p=q=0;
      if(stt.A[lv]==stt.A[rv]){
        for(int x:lch){
          dst[d][x]=1;
          que[q++]=x;
        }
        while(p<q){
          int x=que[p++];
          if(x==stt.B[lv])continue;
          for(const Edge<T>&e:t[x]){
            dst[d][e.to]=dst[d][x]+1;
            que[q++]=e.to;
          }
        }
        int len=dst[d][que[q-1]]+1;
        ptr[lv]=std::make_pair(vsc,len);
        vsc+=len;
      }
      else{
        dst[d][stt.B[lv]]=0;
        bool boundaryA=false;
        if(t.parent(stt.B[lv])==stt.A[lv])que[q++]=~stt.B[lv],boundaryA=true;
        else{
          que[q++]=t.parent(stt.B[lv]);
          dst[d][stt.B[lv]]=0,dst[d][que[0]]=1;
        }
        while(p<q){
          int x=que[p++];
          if(x<0){
            x=~x;
            for(int y:lch)if(x!=y){
              dst[d][y]=dst[d][x]+2;
              que[q++]=y;
            }
          }
          else{
            for(const Edge<T>&e:t[x])if(dst[d][e.to]==-1){
              dst[d][e.to]=dst[d][x]+1;
              que[q++]=e.to;
            }
            int par=t.parent(x);
            if(par==stt.A[lv]){
              if(!boundaryA){
                que[q++]=~x;
                boundaryA=true;
              }
            }
            else if(dst[d][par]==-1){
              dst[d][par]=dst[d][x]+1;
              que[q++]=par;
            }
          }
        }
        int len;
        if(que[q-1]<0)len=q>=2?dst[d][que[q-2]]+1:1;
        else len=dst[d][que[q-1]]+1;
        ptr[lv]=std::make_pair(vsc,len);
        vsc+=len;
      }
      std::vector<int>rch=self(self,rv);
      p=q=0;
      for(int x:rch){
        dst[d][x]=1;
        que[q++]=x;
      }
      while(p<q){
        int x=que[p++];
        if(x==stt.B[rv])continue;
        for(const Edge<T>&e:t[x]){
          dst[d][e.to]=dst[d][x]+1;
          que[q++]=e.to;
        }
      }
      ptr[rv]=std::make_pair(vsc,dst[d][que[q-1]]+1);
      vsc+=dst[d][que[q-1]]+1;
      if(stt.A[lv]==stt.A[rv]){
        if(std::ssize(lch)<std::ssize(rch))std::swap(lch,rch);
        lch.insert(lch.end(),rch.begin(),rch.end());
      }
      return std::move(lch);
    };
    dfs(dfs,n*2-2);
    stt_par=std::move(stt.par);
    lr.resize(n-1);
    for(int i=0;i<n-1;i++)lr[i]=stt.left[i+n]^stt.right[i+n];
  }
  template<typename Func>
  void get_vs(int v,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int)>>);
    f(v);
    int d=dep[v]-1;
    int x=v;
    while(d>=0){
      f(ptr[v].first+dst[d][x]);
      v=stt_par[v];
      d--;
    }
  }
  template<typename Func>
  void get_range(int v,int l,int r,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int,int)>>);
    if(l>=r)return;
    if(l<=0&&1<=r)f(v,v+1);
    int x=v;
    while(true){
      int par=stt_par[v];
      if(par==-1)break;
      int another=lr[par-std::ssize(lr)-1]^v;
      int d=dep[par];
      int rtov=dst[d][x];
      int nl=std::clamp(l-rtov,0,ptr[another].second);
      int nr=std::clamp(r-rtov,0,ptr[another].second);
      if(nl!=nr)f(ptr[another].first+nl,ptr[another].first+nr);
      v=par;
    }
  }
  inline int size()const{return vsc;}
};