#pragma once
#include<iostream>
#include<cassert>
#include<algorithm>
struct Parentheses{
  struct S{
    int l,r;
    int ans;
    S():l(0),r(0),ans(0){}
    S(char c){
      if(c=='('||c=='+'){
        l=0;
        r=1;
        ans=0;
      }
      else if(c==')'||c=='-'){
        l=1;
        r=0;
        ans=0;
      }
      else assert(false);
    }
    friend std::ostream &operator<<(std::ostream &os,const S&x){
      os<<"[l:"<<x.l<<" r:"<<x.r<<" ans:"<<x.ans<<"]";
      return os;
    }
  };
  static inline S op(const S&x,const S&y){
    S res;
    int s=std::min(x.r,y.l);
    res.l=x.l+y.l-s;
    res.r=x.r+y.r-s;
    res.ans=x.ans+y.ans+s*2;
    return res;
  }
  static inline S e(){return S();}
  using F=int;
  static inline S mapping(const F&f,const S&x,long long sz){
    if(f==0)return x;
    S res;
    if(f==1){
      res.l=0;
      res.r=sz;
      res.ans=0;
    }
    else{
      res.l=1;
      res.r=0;
      res.ans=0;
    }
    return res;
  }
  static inline F composition(const F&f,const F&g){return f==0?g:f;}
  static inline F id(){return 0;}
};