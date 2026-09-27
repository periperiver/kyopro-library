#pragma once
#include<vector>
#include<cassert>
template<typename T>
struct SquareRootDecomposition{
private:
  std::vector<T>block,dlock;
  int n,b;
public:
  explicit SquareRootDecomposition(int n_):n(n_),b(sqrt(n_)){
    block.resize((n+b-1)/b,0);
    dlock.resize(n,0);
  }
  inline void add(int i,T x){
    dlock[i]+=x;
    block[i/b]+=x;
  }
  inline void set(int i,T x){
    add(i,x-dlock[i]);
  }
  T sum(int l,int r)const{
    T res=0;
    if(r-l<b){
      for(int i=l;i<r;i++)ret+=dlock[i];
    }
    else{
      int l2=(l+b-1)/b*b;
      int r2=r/b*b;
      for(int i=l;i<l2;i++)res+=dlock[i];
      for(int i=r2;i<r;i++)res+=dlock[i];
      l=l2/b,r=r2/b;
      for(int i=l;i<r;i++)res+=block[i];
    }
    return ret;
  }
  int lower_bound(int id,T k=1)const{
    assert(k);
    if(k>0){
      int id2=(id+b-1)/b*b;
      for(int i=id;i<id2;i++){
        if((k-=dlock[i])<=0)return i;
      }
      id=id2/b;
      while(id<block.size()){
        if(k-block[id]<=0)break;
        k-=block[id++];
      }
      if(id==block.size())return n;
      id*=b;
      for(int i=id;i<id+b;i++){
        if((k-=dlock[i])<=0)return i;
      }
    }
    else{
      k=-k;
      int id2=id/b*b;
      for(int i=id-1;i>=id2;i--){
        if((k-=dlock[i])<=0)return i+1;
      }
      id=id2/b;
      while(id){
        if(k-block[id-1]<=0)break;
        k-=block[--id];
      }
      if(id==0)return 0;
      id*=b;
      for(int i=id-1;i>=id-b;i--){
        if((k-=dlock[i])<=0)return i+1;
      }
    }
    return -1;
  }
  friend std::ostream &operator<<(std::ostream &os,const SquareRootDecomposition&srd){
    os<<srd.dlock;
    return os;
  }
};