#pragma once
#include<vector>
struct increasing_sequence{
private:
  int n,u;
  struct Iterator{
    private:
      int n,u;
      std::vector<int>a;
    public:
      Iterator():n(-1){}
      Iterator(int n,int u):n(n),u(u),a(n,0){}
      const std::vector<int> &operator*()const{return a;}
      bool operator!=(const Iterator&rhs)const{return n!=rhs.n;}
      void operator++(){
        for(int i=n-1;i>=0;i--)if(a[i]<u){
          std::fill(a.begin()+i,a.end(),a[i]+1);
          return;
        }
        n=-1;
      }
  };
public:
  increasing_sequence(int n,int u):n(n),u(u){}
  Iterator begin()const{return Iterator(n,u);}
  Iterator end()const{return Iterator();}
};