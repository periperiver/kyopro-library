#pragma once
#include<iostream>
#include<optional>
#include<vector>
#include<limits>
#include<queue>
#include<algorithm>
template<typename Cap,typename Cost>
struct MinCostFlow{
private:
  struct Edge{
    int to;
    Cap cap,flow;
    Cost cost;
    int idx;
  };
  struct Edge2{
    int from,to;
    Cap lower,upper;
    Cost cost;
  };
  static constexpr Cap infcap=std::numeric_limits<Cap>::max();
  static constexpr Cost infcost=std::numeric_limits<Cost>::max();
  int n;
  std::vector<Edge>g;
  std::vector<int>cnt,ptr;
  std::vector<Cap>b;
  std::vector<std::pair<int,int>>edges;
  std::vector<Edge2>pre_csr;
  std::vector<int>pre;
  std::vector<Cost>potential,dst;
  std::vector<int>pos,neg;
  int pos_ptr,neg_ptr;
  bool dual(Cap delta){
    for(int i=0;i<n;i++)pre[i]=-1,dst[i]=infcost;
    pos_ptr=std::remove_if(pos.begin(),pos.begin()+pos_ptr,[&](int i){return b[i]<delta;})-pos.begin();
    neg_ptr=std::remove_if(neg.begin(),neg.begin()+neg_ptr,[&](int i){return b[i]>-delta;})-neg.begin();
    std::priority_queue<std::pair<Cost,int>,std::vector<std::pair<Cost,int>>,std::greater<std::pair<Cost,int>>>que;
    for(int i=0;i<pos_ptr;i++){
      dst[pos[i]]=0;
      que.push({0,pos[i]});
    }
    int calced=0;
    Cost mx=0;
    while(!que.empty()){
      auto [d,x]=que.top();
      que.pop();
      if(dst[x]!=d)continue;
      mx=d;
      if(b[x]<=-delta&&++calced==neg_ptr)break;
      for(int i=ptr[x];i<ptr[x+1];i++){
        Edge&e=g[i];
        if(e.cap-e.flow>=delta){
          if(dst[e.to]>d+potential[x]-potential[e.to]+e.cost){
            dst[e.to]=d+potential[x]-potential[e.to]+e.cost;
            que.push({dst[e.to],e.to});
            pre[e.to]=e.idx;
          }
        }
      }
    }
    for(int i=0;i<n;i++)potential[i]+=std::min(dst[i],mx);
    return calced>0;
  }
  void primal(Cap delta){
    for(int i=0;i<neg_ptr;i++){
      if(dst[neg[i]]==infcost)continue;
      Cap f=-b[neg[i]];
      int now=neg[i];
      while(pre[now]!=-1){
        Edge&e=g[ptr[now]+pre[now]];
        Edge&inv=g[ptr[e.to]+e.idx];
        if(f>inv.cap-inv.flow)f=inv.cap-inv.flow;
        now=e.to;
      }
      if(f>b[now])f=b[now];
      if(f<delta)continue;
      now=neg[i];
      while(pre[now]!=-1){
        Edge&e=g[ptr[now]+pre[now]];
        Edge&inv=g[ptr[e.to]+e.idx];
        e.flow-=f;
        inv.flow+=f;
        pre[now]=-1;
        now=e.to;
      }
      b[now]-=f;
      b[neg[i]]+=f;
    }
  }
public:
  MinCostFlow(int n_):n(n_),cnt(n,0),b(n,0),pre(n),potential(n,0),dst(n),pos(n),neg(n),pos_ptr(0),neg_ptr(0){}
  void add_source(int u,Cap c){b[u]+=c;}
  void add_edge(int from,int to,Cap lower,Cap upper,Cost cost){
    pre_csr.push_back({from,to,lower,upper,cost});
    edges.emplace_back(from,cnt[from]++);
    cnt[to]++;
  }
  std::optional<Cost>mincost_flow(){
    ptr=cnt;
    ptr.insert(ptr.begin(),0);
    for(int i=0;i<n;i++)ptr[i+1]+=ptr[i];
    std::vector<int>ptr2(n,0);
    g.resize(ptr[n]);
    for(auto&&[from,to,lower,upper,cost]:pre_csr){
      int gfrom=ptr2[from],gto=ptr2[to];
      if(from==to)gto++;
      g[ptr[from]+(ptr2[from]++)]={to,upper,0,cost,gto};
      g[ptr[to]+(ptr2[to]++)]={from,-lower,0,-cost,gfrom};
    }
    Cap delta=1;
    for(int i=0;i<n;i++)for(int j=ptr[i];j<ptr[i+1];j++){
      Edge&e=g[j];
      if(e.cap-e.flow<0){
        Cap c=e.cap-e.flow;
        e.flow+=c;
        Edge&inv=g[ptr[e.to]+e.idx];
        inv.flow-=c;
        b[i]-=c;
        b[e.to]+=c;
      }
      else while(delta<=e.cap-e.flow)delta<<=1;
    }
    while(delta){
      pos_ptr=neg_ptr=0;
      for(int i=0;i<n;i++)for(int j=ptr[i];j<ptr[i+1];j++){
        Edge&e=g[j];
        Cap c=e.cap-e.flow;
        if(c>=delta&&e.cost+potential[i]-potential[e.to]<0){
          e.flow+=c;
          g[ptr[e.to]+e.idx].flow-=c;
          b[i]-=c;
          b[e.to]+=c;
        }
      }
      for(int i=0;i<n;i++){
        if(b[i]>0)pos[pos_ptr++]=i;
        else if(b[i]<0)neg[neg_ptr++]=i;
      }
      while(dual(delta))primal(delta);
      delta>>=1;
    }
    if(pos_ptr+neg_ptr>0)return std::nullopt;
    Cost ret=0;
    for(int i=0;i<ptr[n];i++)ret+=g[i].flow*g[i].cost;
    return std::make_optional(ret/2);
  }
  struct Result{
    int from,to;
    Cap lower,upper,flow;
    Cost cost;
    friend std::ostream &operator<<(std::ostream &os,const Result&res){
      os<<'[';
      os<<"from:"<<res.from<<' ';
      os<<"to:"<<res.to<<' ';
      os<<"lower:"<<res.lower<<' ';
      os<<"upper:"<<res.upper<<' ';
      os<<"cost:"<<res.cost<<' ';
      os<<"flow:"<<res.flow;
      os<<']';
      return os;
    }
  };
  std::vector<Cost>get_potential(){
    std::vector<Cost>ret(n,0);
    for(int v=0;v<n;v++)for(int i=0;i<n;i++){
      for(int j=ptr[i];j<ptr[i+1];j++){
        Edge&e=g[j];
        if(e.cap-e.flow>0&&ret[e.to]>ret[i]+e.cost)ret[e.to]=ret[i]+e.cost;
      }
    }
    return ret;
  }
  std::vector<Result>get_edge(){
    std::vector<Result>ret(edges.size());
    for(int i=0;i<(int)ret.size();i++){
      Edge&e=g[ptr[edges[i].first]+edges[i].second];
      Edge&inv=g[ptr[e.to]+e.idx];
      ret[i]={edges[i].first,e.to,-inv.cap,e.cap,e.flow,e.cost};
    }
    return ret;
  }
};