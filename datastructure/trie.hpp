#pragma once
#include "template.hpp"
template<typename K,typename V,int N>
struct trie{
private:
  struct node{
    node** child;
    node *par;
    V *val;
    node():val(nullptr),child(new node*[N]),par(nullptr){
      for(int i=0;i<N;i++)child[i]=nullptr;
    }
  };
  struct trie_iterator{
    node *nd;
    vector<K>key;
    trie_iterator(node *nd2):nd(nd2),key(){}
    bool operator!=(const trie_iterator&itr)const{
      return nd!=itr.nd;
    }
    void step(){
      for(int i=0;i<N;i++){
        if(nd->child[i]){
          key.push_back(i);
          nd=nd->child[i];
          return;
        }
      }
      while(true){
        nd=nd->par;
        if(!nd)return;
        int nx=key.back();
        key.pop_back();
        for(int i=nx+1;i<N;i++){
          if(nd->child[i]){
            key.push_back(i);
            nd=nd->child[i];
            return;
          }
        }
      }
    }
    trie_iterator &operator++(){
      do{
        step();
      }while(nd&&!nd->val);
      return *this;
    }
    pair<const vector<K>&,V&> operator*()const{
      return {key,*nd->val};
    }
  };
  node *root;
public:
  trie(){
    root=new node();
  }
  V &operator[](const vector<K>&key){
    int sz=key.size();
    node *now=root;
    for(int i=0;i<sz;i++){
      if(!now->child[key[i]]){
        now->child[key[i]]=new node();
        now->child[key[i]]->par=now;
      }
      now=now->child[key[i]];
    }
    if(!now->val)now->val=new V();
    return *now->val;
  }
  bool contains(const vector<K>&key)const{
    int sz=key.size();
    node *now=root;
    for(int i=0;i<sz;i++){
      if(!now->child[key[i]])return false;
      now=now->child[key[i]];
    }
    return now->val;
  }
  trie_iterator begin(){
    trie_iterator ret(root);
    while(ret.nd&&!ret.nd->val)ret.step();
    return ret;
  }
  trie_iterator end(){
    return trie_iterator(nullptr);
  }
};