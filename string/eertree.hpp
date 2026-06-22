#pragma once
#include<vector>
#include<string>
#include<climits>
template<int mx=CHAR_MAX>
struct eertree{
  struct node{
    node* child[mx];
    node *link,*par;
    int idx,l,r;
    node(node *link_,int idx_,int l_,int r_):link(link_),par(nullptr),idx(idx_),l(l_),r(r_){for(int i=0;i<mx;i++)child[i]=nullptr;}
  };
  std::vector<node>nds;
  std::vector<node*>belong;
  eertree(const std::string&s):belong(s.size()+1){
    nds.reserve(s.size()+2);
    nds.emplace_back(nullptr,0,0,-1);
    nds.emplace_back(&nds[0],1,0,0);
    nds[1].par=&nds[0];
    node *p=&nds[0];
    for(int i=0;i<(int)s.size();i++){
      belong[i]=p;
      while(p!=&nds[0]){
        int j=i-1-p->r+p->l;
        if(j>=0&&s[i]==s[j])break;
        p=p->link;
      }
      if(p->child[s[i]]){
        p=p->child[s[i]];
        continue;
      }
      int to=nds.size();
      int l=i-1-p->r+p->l;
      int r=i+1;
      node*nxt=&nds.emplace_back(nullptr,nds.size(),i-1-p->r+p->l,i+1);
      nxt->par=p;
      p->child[s[i]]=nxt;
      if(p==&nds[0])nxt->link=&nds[1];
      else{
        while(1){
          p=p->link;
          int j=i-1-p->r+p->l;
          if(j>=0&&s[i]==s[j])break;
        }
        nxt->link=p->child[s[i]];
      }
      p=nxt;
    }
    belong[s.size()]=p;
  }
  node* operator[](int i){return belong[i];}
  int distinct_palindrome()const{return nds.size()-2;}
};