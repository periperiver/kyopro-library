#pragma once
#include<vector>
#include<algorithm>
struct separate{
private:
  int n,k;
  struct Iterator{
  private:
    int n,k;
    std::vector<int>v;
  public:
    Iterator():n(-1),k(),v(){}
    Iterator(int n,int k):n(n),k(k),v(n*k){
      for(int i=0;i<n;i++)for(int j=0;j<k;j++)v[i*k+j]=i;
    }
    inline std::vector<int> &operator*(){return v;}
    inline bool operator!=(const Iterator&rhs)const{return this->n!=rhs.n;}
    inline void operator++(){
      for(int i=n*k-2;i>=0;i--)if(v[i]<v[i+1]){
        int j=i+1;
        while(j+1<n*k&&v[i]<v[j+1])j++;
        std::swap(v[i],v[j]);
        std::reverse(v.begin()+i+1,v.end());
        int mx=-1,id=-1;
        for(int l=0;l<=i;l++){
          if(mx+1<v[l]){
            std::sort(v.begin()+id+1,v.end(),std::greater());
            j=id;
            while(j>=0&&v[j]>=v[id+1])j--;
            if(j==-1)n=-1;
            else{
              for(l=j+1;l+1<n*k&&v[j]<v[l+1];l++);
              std::swap(v[j],v[l]);
              std::reverse(v.begin()+j+1,v.end());
            }
            return;
          }
          if(v[l]>mx)mx=v[l];
          else id=l;
        }
        return;
      }
      n=-1;
    }
  };
public:
  separate(int n,int k):n(n),k(k){}
  Iterator begin()const{return Iterator(n,k);}
  Iterator end()const{return Iterator();}
};