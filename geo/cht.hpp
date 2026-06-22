#pragma once
#include<set>
#include<numeric>
template<typename T,bool MIN=true>
struct ConvexHullTrick{
  struct Line{
    mutable T a,b,p;
    bool operator<(const Line &rhs)const{return a<rhs.a;}
    bool operator<(T rhs)const{return p<rhs;}
  };
  static constexpr T safe_div(T a,T b){
    if constexpr (std::is_integral_v<T>)return a/b-((a^b)<0&&a%b);
    else return a/b;
  }
  static constexpr T INF=std::numeric_limits<T>::max();
  std::multiset<Line,std::less<>>dat;
  bool insect(typename std::multiset<Line,std::less<>>::iterator x,typename std::multiset<Line,std::less<>>::iterator y){
    if(y==dat.end()){
      x->p=INF;
      return false;
    }
    if(x->a==y->a)x->p=x->b>y->b?INF:-INF;
    else x->p=safe_div(y->b-x->b,x->a-y->a);
    return x->p>=y->p;
  }
  void add(T a,T b){
    if(MIN)a=-a,b=-b;
    auto z=dat.insert({a,b,0}),y=z++,x=y;
    while(insect(y,z))z=dat.erase(z);
    if(x!=dat.begin()&&insect(--x,y))insect(x,y=dat.erase(y));
    while((y=x)!=dat.begin()&&(--x)->p>=y->p)insect(x,dat.erase(y));
  }
  T query(T x)const{
    auto l=*dat.lower_bound(x);
    T ret=l.a*x+l.b;
    return MIN?-ret:ret;
  }
  std::pair<T,T>query_line(T x)const{
    auto l=*dat.lower_bound(x);
    if constexpr(MIN)l.a=-l.a,l.b=-l.b;
    return std::make_pair(l.a,l.b);
  }
  inline void clear(){dat.clear();}
};