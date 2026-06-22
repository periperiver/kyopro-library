#pragma ocne
#include<utility>
template<typename T>
struct PersistentMeldableHeap{
  struct node{
    node *left,*right;
    T val;
    int s;
    node():left(nullptr),right(nullptr),val(),s(0){}
    node(T x):left(nullptr),right(nullptr),val(x),s(0){}
    node(node *nd):left(nd->left),right(nd->right),val(nd->val),s(nd->s){}
    static node* meld(node *lhs,node *rhs){
      if(!lhs)return rhs?new node(rhs):nullptr;
      if(!rhs)return new node(lhs);
      lhs=new node(lhs);
      rhs=new node(rhs);
      if(lhs->val>rhs->val)std::swap(lhs,rhs);
      lhs->right=meld(lhs->right,rhs);
      if(!lhs->left||lhs->left->s<lhs->right->s)std::swap(lhs->left,lhs->right);
      lhs->s=lhs->right?lhs->right->s+1:0;
      return lhs;
    }
  };
  node *root;
  PersistentMeldableHeap():root(nullptr){}
  PersistentMeldableHeap(node *nd):root(nd){}
  [[nodiscard]]PersistentMeldableHeap push(T x){
    return PersistentMeldableHeap(node::meld(this->root,new node(x)));
  }
  [[nodiscard]]PersistentMeldableHeap pop(){
    return PersistentMeldableHeap(node::meld(this->root->left,this->root->right));
  }
  [[nodiscard]]PersistentMeldableHeap meld(PersistentMeldableHeap rhs){
    return PersistentMeldableHeap(node::meld(this->root,rhs.root));
  }
  const T& top()const{
    return root->val;
  }
  bool empty()const{return !root;}
};