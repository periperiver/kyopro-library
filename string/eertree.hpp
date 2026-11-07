#pragma once
#include<vector>
#include<array>
#include<string>
#include<climits>
#include<cassert>
template<int mx>
struct eertree{
private:
  struct node{
    std::array<int,mx>child;
    int suffix,series,par;
    int len;
    node():suffix(-1),series(-1),par(-1),len(-1){child.fill(-1);}
    int& operator[](int x){return child[x];}
    const int& operator[](int x)const{return child[x];}
  };
  std::vector<int>s;
  std::vector<node>nds;
  int p;
public:
  eertree():nds(2),p(1){
    nds[0].len=-1;
    nds[1].suffix=0;
    nds[1].series=1;
    nds[1].len=0;
  }
  int add(int c){
    assert(0<=c&&c<mx);
    s.push_back(c);
    while(p>0){
      int j=s.size()-nds[p].len-2;
      if(j>=0&&s[j]==c)break;
      p=nds[p].suffix;
    }
    if(nds[p][c]!=-1)p=nds[p][c];
    else{
      node&nd=nds.emplace_back();
      nd.len=nds[p].len+2;
      nds[p][c]=nds.size()-1;
      nd.par=p;
      if(p==0)nd.suffix=1;
      else{
        while(true){
          p=nds[p].suffix;
          int j=s.size()-nds[p].len-2;
          if(j>=0&&s[j]==c)break;
        }
        nd.suffix=nds[p][c];
      }
      p=nds.size()-1;
    }
    return p;
  }
  int size()const{return nds.size();}
  node& operator[](int x){return nds[x];}
};