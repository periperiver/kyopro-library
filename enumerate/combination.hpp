#pragma once
#include<vector>
#include<numeric>
struct combination{
private:
  int n,k;
  struct Iterator{
  private:
    int n,k;
    std::vector<int>v;
  public:
    Iterator():n(-1),k(0),v(){}
    Iterator(int n,int k):n(n),k(k),v(k){
      std::iota(v.begin(),v.end(),0);
    }
    inline std::vector<int> &operator*(){return v;}
    inline bool operator!=(const Iterator &rhs)const{return this->n!=rhs.n;}
    inline void operator++(){
      for(int i=k;i--;)if(v[i]!=n-k+i){
        std::iota(v.begin()+i,v.end(),v[i]+1);
        return;
      }
      n=-1;
    }
  };
public:
  combination(int n,int k):n(n),k(k){}
  Iterator begin()const{return Iterator(n,k);}
  Iterator end()const{return Iterator();}
};