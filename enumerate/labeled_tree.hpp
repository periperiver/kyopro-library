#pragma once
#include "../graph/prufer.hpp"
struct labeled_tree{
private:
  int n;
  struct Iterator{
    int n;
    std::vector<int>v;
    Iterator():n(-1){}
    Iterator(int n_):n(n_),v(std::max(0,n_-2)){}
    Tree<> operator*(){
      if(n==1){
        Tree res(1);
        res.build();
        return res;
      }
      return prufer(v);
    }
    inline bool operator!=(const Iterator&rhs)const{return this->v!=rhs.v;}
    inline void operator++(){
      for(int i=n-3;i>=0;i--){
        if(v[i]!=n-1){
          v[i]++;
          return;
        }
        v[i]=0;
      }
      v={-1};
    }
  };
public:
  labeled_tree(int n_):n(n_){}
  Iterator begin()const{return Iterator(n);}
  Iterator end()const{
    Iterator res;
    res.v={-1};
    return res;
  }
};