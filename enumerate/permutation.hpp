#pragma once
#include<vector>
#include<numeric>
#include<algorithm>
struct permutation{
private:
  int n;
  struct Iterator{
  private:
    int n;
    std::vector<int>v;
  public:
    Iterator():n(-1),v(){}
    Iterator(int n):n(n),v(n){
      std::iota(v.begin(),v.end(),0);
    }
    inline std::vector<int> &operator*(){return v;}
    inline bool operator!=(const Iterator&rhs)const{return this->n!=rhs.n;}
    inline void operator++(){
      for(int i=n-2;i>=0;i--)if(v[i]<v[i+1]){
        int j=i+1;
        while(j+1<n&&v[j+1]>v[i])j++;
        std::swap(v[i],v[j]);
        std::reverse(v.begin()+i+1,v.end());
        return;
      }
      n=-1;
    }
  };
public:
  permutation(int n):n(n){}
  Iterator begin()const{return Iterator(n);}
  Iterator end()const{return Iterator();}
};