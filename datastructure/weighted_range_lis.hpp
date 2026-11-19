#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include "persistent/segmenttree.hpp"
template<typename T>
struct WeightedRangeLIS{
private:
  struct M{
    struct S{
      T lval,rval;
      S():lval(0),rval(0){}
      S(T x):lval(x),rval(x){}
      S(T x,T y):lval(x),rval(y){}
    };
    static S op(S x,S y){
      return S(x.lval,y.rval);
    }
    static S e(){return S();}
  };
  struct M2{
    using S=PersistentSegmentTree<int,M>;
    static S op(S x,S y){
    }
    static S e(){return S();}
  };
  int n;
  std::vector<PersistentSegmentTree<int,M2>>seg;
public:
  WeightedRangeLIS(){}
  template<typename U>
  WeightedRangeLIS(std::vector<U>a,std::vector<T>w):n(a.size()),seg(n+1){
    assert(a.size()==w.size());
    std::vector<U>z(a);
    std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
    seg[0]=PersistentSegmentTree<int,M2>(0,z.size());
    for(int i=0;i<n;i++){
      int key=std::lower_bound(z.begin(),z.end(),a[i])-z.begin();
      PersistentSegmentTree<int,M>now=seg[i].prod(0,key);
    }
  }
  T query(int l,int r)const{
    assert(0<=l&&l<=r&&r<=n);
    return seg[r].all_prod().prod(l,n).lval;
  }
};
//後でやる