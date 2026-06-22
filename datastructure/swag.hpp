#pragma once
#include<vector>
template<typename M>
struct SWAG{
private:
  using S=typename M::S;
  std::vector<S>a,b;
  std::vector<S>proda,prodb;
  void eval(){
    proda.resize(a.size()+1,M::e());
    prodb.resize(b.size()+1,M::e());
    for(int i=1;i<(int)proda.size();i++)proda[i]=M::op(a[i-1],proda[i-1]);
    for(int i=1;i<(int)prodb.size();i++)prodb[i]=M::op(prodb[i-1],b[i-1]);
  }
public:
  SWAG():proda{M::e()},prodb{M::e()}{}
  void push_front(S v){
    a.emplace_back(move(v));
    proda.emplace_back(M::op(a.back(),proda.back()));
  }
  void push_back(S v){
    b.emplace_back(std::move(v));
    prodb.emplace_back(M::op(prodb.back(),b.back()));
  }
  inline S front()const{return a.empty()?b[0]:a.back();}
  inline S back()const{return b.empty()?a[0]:b.back();}
  void pop_front(){
    if(a.empty()){
      int mid=b.size()/2;
      a=std::vector<S>(b.rbegin()+mid,b.rend());
      b=std::vector<S>(b.end()-mid,b.end());
      eval();
    }
    a.pop_back();
    proda.pop_back();
  }
  void pop_back(){
    if(b.empty()){
      int mid=a.size()/2;
      b=vector<S>(a.rbegin()+mid,a.rend());
      a=vector<S>(a.end()-mid,a.end());
      eval();
    }
    b.pop_back();
    prodb.pop_back();
  }
  inline S all_prod()const{return M::op(proda.back(),prodb.back());}
  inline int size()const{return a.size()+b.size();}
  inline bool empty()const{return a.empty()&&b.empty();}
};