#pragma once
#include<vector>
#include<algorithm>
template<typename M>
struct WeightedUnionFind{
private:
  using S=typename M::S;
  std::vector<int>par;
  std::vector<S>dat;
  int cs;
public:
  WeightedUnionFind(int n):par(n,-1),dat(n,M::e()),cs(n){}
  WeightedUnionFind(const std::vector<S>&init):par(init.size(),-1),dat(init),cs(init.size()){}
  int root(int u){
    if(par[u]<0)return u;
    return par[u]=root(par[u]);
  }
  bool merge(int u,int v){
    int ru=root(u),rv=root(v);
    if(ru==rv)return false;
    if(par[ru]<par[rv])std::swap(ru,rv);
    dat[rv]=M::op(dat[ru],dat[rv]);
    par[rv]+=par[ru];
    par[ru]=rv;
    cs--;
    return true;
  }
  bool same(int u,int v){return root(u)==root(v);}
  int size(int u=-1){return u==-1?cs:-par[root(u)];}
  S& operator[](int i){return dat[root(i)];}
  std::vector<std::pair<std::vector<int>,S>>get_all(){
    std::vector<std::pair<std::vector<int>,S>>res(par.size());
    for(int i=0;i<par.size();i++)res[root(i)].first.push_back(i);
    int p=0;
    for(int i=0;i<(int)res.size();i++){
      if(!res[i].first.empty()){
        res[i].second=dat[i];
        if(i==p)p++;
        else res[p++]=std::move(res[i]);
      }
    }
    res.resize(p);
    return res;
  }
};