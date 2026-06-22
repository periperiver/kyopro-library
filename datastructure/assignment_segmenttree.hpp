#pragma once
#include<vector>
#include "../math/util.hpp"
template<typename M>
struct AssignmentSegmentTree{
private:
  using S=typename M::S;
  int n,log2n,s;
  std::vector<S>dat,memo;
  std::vector<S*>lazy;
  inline void propagate(int i,S*f){
    lazy[i]=f;
    dat[i]=*f;
  }
  inline void push(int i){
    if(lazy[i]){
      propagate(i*2,lazy[i]-1);
      propagate(i*2+1,lazy[i]-1);
      lazy[i]=nullptr;
    }
  }
  inline void update(int i){dat[i]=M::op(dat[i*2],dat[i*2+1]);}
  inline void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
  inline void path_update(int i){
    for(i>>=lsb(i);i>>=1;)update(i);
  }
public:
  AssignmentSegmentTree():n(0),log2n(0),s(0){}
  explicit AssignmentSegmentTree(int n_):n(ceil_pow2(n_)),log2n(msb(n)),s(n_),dat(n*2,M::e()),lazy(n*2,nullptr){
    memo.reserve(n*2);
  }
  explicit AssignmentSegmentTree(const std::vector<S>&init):n(ceil_pow2(init.size())),log2n(msb(n)),s(init.size()),dat(n*2,M::e()),lazy(n*2,nullptr){
    memo.reserve(n*2);
    std::copy(init.begin(),init.end(),dat.begin()+n);
    for(int i=n;--i;)update(i);
  }
  void assign(int l,int r,S f){
    l+=n,r+=n;
    path_push(l),path_push(r);
    int l2=l,r2=r;
    S* ptr=&memo.emplace_back(f);
    for(int i=1;i<=log2n;i++)memo.push_back(M::op(memo.back(),memo.back()));
    while(l<r){
      if(l&1)propagate(l++,ptr);
      if(r&1)propagate(--r,ptr);
      l>>=1,r>>=1,ptr++;
    }
    path_update(l2),path_update(r2);
    if((int)memo.capacity()-(int)memo.size()<log2n+1){
      for(int i=1;i<n;i++)push(i);
      std::fill(lazy.begin()+n,lazy.end(),nullptr);
      memo.clear();
    }
  }
  S prod(int l,int r){
    l+=n,r+=n;
    path_push(l),path_push(r);
    S resl=M::e(),resr=M::e();
    while(l<r){
      if(l&1)resl=M::op(resl,dat[l++]);
      if(r&1)resr=M::op(dat[--r],resr);
      l>>=1,r>>=1;
    }
    return M::op(resl,resr);
  }
  S get(int i){
    i+=n;
    for(int j=log2n;j>0;j--)push(i>>j);
    return dat[i];
  }
  inline S all_prod()const{return dat[1];}
  std::vector<S>get_all(){
    for(int i=1;i<n;i++)push(i);
    return std::vector<S>(dat.begin()+n,dat.begin()+n+s);
  }
};