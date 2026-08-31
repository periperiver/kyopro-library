#pragma once
#include<iostream>
#include<cassert>
#include<algorithm>
template<typename T=int>
struct Parentheses{
  struct S{
    T l,r,len;
    T ans;
    S():l(0),r(0),len(0),ans(0){}
    S(char c){
      if(c=='('||c=='+'){
        l=0;
        r=1;
        len=1;
        ans=0;
      }
      else if(c==')'||c=='-'){
        l=1;
        r=0;
        len=1;
        ans=0;
      }
      else assert(false);
    }
    friend std::ostream &operator<<(std::ostream &os,const S&x){
      os<<"[l:"<<x.l<<" r:"<<x.r<<" len:"<<x.len<<" ans:"<<x.ans<<"]";
      return os;
    }
  };
  static inline S op(const S&x,const S&y){
    S res;
    T s=std::min(x.r,y.l);
    res.l=x.l+y.l-s;
    res.r=x.r+y.r-s;
    res.len=x.len+y.len;
    res.ans=x.ans+y.ans+s*2;
    return res;
  }
  static inline S e(){return S();}
};