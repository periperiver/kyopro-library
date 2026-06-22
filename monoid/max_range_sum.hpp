#pragma once
#include<algorithm>
#include<limits>
template<typename T,bool allow_empty=true>
struct MaxRangeSum{
  static constexpr T minf=std::numeric_limits<T>::min()/2;
  struct S{
    T sum,ans,left,right;
    S():sum(0),ans(allow_empty?0:minf),left(allow_empty?0:minf),right(allow_empty?0:minf){}
    S(T v_):sum(v_),ans(allow_empty?std::max<T>(v_,0):v_),left(allow_empty?std::max<T>(v_,0):v_),right(allow_empty?std::max<T>(v_,0):v_){}
  };
  static inline S op(const S&x,const S&y){
    S res;
    res.sum=x.sum+y.sum;
    res.ans=std::max(x.ans,y.ans);
    if(res.ans<x.right+y.left)res.ans=x.right+y.left;
    res.left=std::max(x.left,x.sum+y.left);
    res.right=std::max(x.right+y.sum,y.right);
    return res;
  }
  static inline S e(){return S();}
  static inline void revS(S&x){std::swap(x.left,x.right);}
};