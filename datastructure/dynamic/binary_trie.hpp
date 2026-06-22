#pragma once
#include<numeric>
#include<type_traits>
template<typename I,typename M,int dep=std::numeric_limits<I>::digits>
struct BinaryTrie{
  static_assert(std::is_integral_v<I>);
  using S=typename M::S;
  struct node{
    node *ch[2]={nullptr,nullptr};
    node *par=nullptr;
    int sz=0;
    S v=M::e();
    node(){}
    inline void update(){
      v=M::op((ch[0]?ch[0]->v:M::e()),(ch[1]?ch[1]->v:M::e()));
      sz=(ch[0]?ch[0]->sz:0)+(ch[1]?ch[1]->sz:0);
    }
    static node* meld(node* left,node* right){
      if(!left)return right;
      if(!right)return left;
      left->ch[0]=meld(left->ch[0],right->ch[0]);
      left->ch[1]=meld(left->ch[1],right->ch[1]);
      if(left->ch[0])left->ch[0]->par=left;
      if(left->ch[1])left->ch[1]->par=left;
      left->update();
      right->ch[0]=right->ch[1]=nullptr;
      delete(right);
      return left;
    }
    ~node(){
      if(ch[0])delete(ch[0]);
      if(ch[1])delete(ch[1]);
    }
  };
  static constexpr I inf=std::numeric_limits<I>::max();
  node *root;
  BinaryTrie():root(new node()){}
  BinaryTrie(node *nd):root(nd){}
  void set(I key,const S&val){
    node *nd=root;
    for(int i=dep-1;i>=0;i--){
      int nxt=key>>i&1;
      if(!nd->ch[nxt]){
        nd->ch[nxt]=new node();
        nd->ch[nxt]->par=nd;
      }
      nd=nd->ch[nxt];
    }
    nd->v=val;
    nd->sz=1;
    nd=nd->par;
    while(nd){
      nd->update();
      nd=nd->par;
    }
  }
  node* get(I key){
    node *nd=root;
    for(int i=dep-1;i>=0;i--){
      int v=key>>i&1;
      if(!nd->ch[v])return nullptr;
      nd=nd->ch[v];
    }
    return nd;
  }
  void erase(node *nd){
    for(int i=0;i<dep;i++){
      node *p=nd->par;
      nd->ch[0]=nd->ch[1]=nullptr;
      if(p->ch[0]==nd){
        p->ch[0]=nullptr;
        delete nd;
        if(p->ch[1])break;
      }
      else if(p->ch[1]==nd){
        p->ch[1]=nullptr;
        delete nd;
        if(p->ch[0])break;
      }
      else std::abort();
      nd=p;
    }
  }
  // I xor_min(I key)const{
  //   node *nd=root;
  //   I res=0;
  //   for(int i=dep-1;i>=0;i--){
  //     if(nd->ch[key>>i&1])nd=nd->ch[key>>i&1];
  //     else nd=nd->ch[!(key>>i&1)],res|=I(1)<<i;
  //   }
  //   return res;
  // }
  // I xor_max(I key)const{
  //   node *nd=root;
  //   I res=0;
  //   for(int i=dep-1;i>=0;i--){
  //     int v=!(key>>i&1);
  //     if(nd->ch[v])nd=nd->ch[v],res|=I(1)<<i;
  //     else nd=nd->ch[!v];
  //   }
  //   return res;
  // }
  std::pair<BinaryTrie<I,M>,BinaryTrie<I,M>>split(int k){
    if(k==0)return std::make_pair(BinaryTrie<I,M>(),*this);
    if(k==root->sz)return std::make_pair(*this,BinaryTrie<I,M>());
    I mask=0;
    node *sl=root,*sr=new node();
    for(int i=dep-1;i>=0;i--){
      if((sl->ch[0]?sl->ch[0]->sz:0)==k){
        sr->ch[1]=sl->ch[1];
        sl->ch[1]=nullptr;
        if(sr->ch[1])sr->ch[1]->par=sr;
        break;
      }
      else if((sl->ch[0]?sl->ch[0]->sz:0)>k){
        sr->ch[1]=sl->ch[1];
        sl->ch[1]=nullptr;
        if(sr->ch[1])sr->ch[1]->par=sr;
        sr->ch[0]=new node();
        sr->ch[0]->par=sr;
        sl=sl->ch[0],sr=sr->ch[0];
      }
      else{
        k-=sl->ch[0]?sl->ch[0]->sz:0;
        sr->ch[1]=new node();
        sr->ch[1]->par=sr;
        sl=sl->ch[1],sr=sr->ch[1];
      }
    }
    while(true){
      sl->update();
      if(sl->par)sl=sl->par;
      else break;
    }
    while(true){
      sr->update();
      if(sr->par)sr=sr->par;
      else break;
    }
    return std::make_pair(BinaryTrie<I,M>(sl),BinaryTrie<I,M>(sr));
  }
  inline void clear(){
    if(root->ch[0])delete(root->ch[0]);
    if(root->ch[1])delete(root->ch[1]);
    root->ch[0]=root->ch[1]=nullptr;
    root->sz=0;
    root->v=M::e();
  }
  inline S all_prod()const{return root->v;}
  inline int size()const{return root->sz;}
  static inline BinaryTrie meld(const BinaryTrie&left,const BinaryTrie&right){
    return BinaryTrie(node::meld(left.root,right.root));
  }
  void kill(){
    delete root;
  }
};