#pragma once
#include "../datastructure/static_toptree.hpp"
template<typename T=int>
struct CentroidDecomposition:public Tree<T>{
private:
  using Tree<T>::Tree;
  using Tree<T>::g;
  using Tree<T>::ptr;
  using Tree<T>::remove_parent;
  using Tree<T>::hld;
  int boundaryA1,boundaryB1,boundaryA2,boundaryB2;
  std::vector<Edge<T>>adj1,adj2;
  bool prepared=false;
  StaticTopTree stt;
  void prepare(){
    if(prepared)return;
    remove_parent();
    hld();
    stt=StaticTopTree(*this,0);
    prepared=true;
  }
public:
  template<typename Func>
  void run(const Func&f){
    prepare();
    int n=this->size();
    auto dfs=[&](auto self,int nd)->std::vector<Edge<T>> {
      if(nd<n){
        std::vector<Edge<T>>res(1);
        if(nd==0)res[0]=Edge<T>(-1,0);
        else{
          res[0]=g[ptr[nd+1]-1];
          std::swap(res[0].from,res[0].to);
        }
        return res;
      }
      int lv=stt.left[nd],rv=stt.right[nd];
      std::vector<Edge<T>>lch=self(self,lv);
      std::vector<Edge<T>>rch=self(self,rv);
      boundaryA1=stt.A[lv];
      boundaryB1=stt.B[lv];
      boundaryA2=stt.A[rv];
      boundaryB2=stt.B[rv];
      std::swap(adj1,lch);
      std::swap(adj2,rch);
      f(boundaryA1,boundaryA2);
      std::swap(adj1,lch);
      std::swap(adj2,rch);
      if(boundaryA1==boundaryA2){
        if(std::ssize(lch)<std::ssize(rch))std::swap(lch,rch);
        lch.insert(lch.end(),rch.begin(),rch.end());
      }
      return lch;
    };
    dfs(dfs,n*2-2);
  }
  typename Tree<T>::tree_range get(int c,int x){
    if(c==0){
      if(x==boundaryA1)return typename Tree<T>::tree_range{adj1.begin(),adj1.end()};
      if(x==boundaryB1)return typename Tree<T>::tree_range{g.begin()+ptr[x+1]-(!!x),g.begin()+ptr[x+1]};
      return typename Tree<T>::tree_range{g.begin()+ptr[x],g.begin()+ptr[x+1]};
    }
    else if(c==1){
      if(x==boundaryA2)return typename Tree<T>::tree_range{adj2.begin(),adj2.end()};
      if(x==boundaryB2)return typename Tree<T>::tree_range{g.begin()+ptr[x+1],g.begin()+ptr[x+1]};
      return typename Tree<T>::tree_range{g.begin()+ptr[x],g.begin()+ptr[x+1]};
    }
    else std::abort();
  }
  typename Tree<T>::const_tree_range get(int c,int x)const{
    if(c==0){
      if(x==boundaryA1)return typename Tree<T>::const_tree_range{adj1.begin(),adj1.end()};
      if(x==boundaryB1)return typename Tree<T>::const_tree_range{g.begin()+ptr[x+1]-(!!x),g.begin()+ptr[x+1]};
      return typename Tree<T>::const_tree_range{g.begin()+ptr[x],g.begin()+ptr[x+1]};
    }
    else if(c==1){
      if(x==boundaryA2)return typename Tree<T>::const_tree_range{adj2.begin(),adj2.end()};
      if(x==boundaryB2)return typename Tree<T>::const_tree_range{g.begin()+ptr[x+1],g.begin()+ptr[x+1]};
      return typename Tree<T>::const_tree_range{g.begin()+ptr[x],g.begin()+ptr[x+1]};
    }
    else std::abort();
  }
  CentroidDecomposition(const Tree<T>&t){Tree<T>::operator=(t);}
};