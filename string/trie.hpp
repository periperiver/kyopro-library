#pragma once
#include<climits>
#include<vector>
template<int mx=CHAR_MAX>
struct Trie{
  struct node{
    int child[mx],par;
    node():par(-1){
      for(int i=0;i<mx;i++)child[i]=-1;
    }
    int& operator[](int x){return child[x];}
    const int& operator[](int x)const{return child[x];}
  };
  std::vector<node>nds;
  Trie():nds(1){}
  int add(const std::string&s){
    int v=0;
    for(char c:s){
      if(nds[v][c]==-1){
        nds.emplace_back();
        nds[v][c]=nds.size()-1;
        nds.back().par=v;
      }
      v=nds[v][c];
    }
    return v;
  }
  node &operator[](int x){return nds[x];}
  const node &operator[](int x)const{return nds[x];}
};