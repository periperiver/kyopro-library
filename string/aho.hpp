#pragma once
#include<string>
#include<vector>
template<int sigma=26>
struct AhoCorasick{
  struct node{
    int nxt[sigma];
    int failure;
    node(){
      std::fill(nxt,nxt+sigma,-1);
      failure=-1;
    }
    int& operator[](int i){return nxt[i];}
  };
  std::vector<node>dat;
  AhoCorasick(){
    dat.push_back(node());
  }
  int add(const std::string&s){
    int v=0;
    for(int c:s){
      if(dat[v][c]==-1){
        dat[v][c]=dat.size();
        dat.push_back(node());
      }
      v=dat[v][c];
    }
    return v;
  }
  template<bool set_fail=true>
  void build_aho(){
    std::vector<int>que(dat.size());
    int l=0,r=1;
    que[0]=0;
    while(l<r){
      int x=que[l++];
      for(int i=0;i<sigma;i++){
        int s=dat[x][i];
        if(s==-1)continue;
        que[r++]=s;
        int f=dat[x].failure;
        while(f!=-1&&dat[f][i]==-1)f=dat[f].failure;
        dat[s].failure=f==-1?0:dat[f][i];
      }
    }
    if constexpr(!set_fail)return;
    for(int i=0;i<std::ssize(dat);i++){
      int s=que[i];
      for(int j=0;j<sigma;j++)if(dat[s][j]==-1){
        int f=dat[s].failure;
        dat[s][j]=f==-1?0:dat[f][j];
      }
    }
  }
  node& operator[](int i){return dat[i];}
  const node& operator[](int i)const{return dat[i];}
  int size()const{return dat.size();}
};