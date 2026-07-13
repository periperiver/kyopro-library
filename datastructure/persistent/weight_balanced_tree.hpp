#pragma once
#include<vector>
#include<cstddef>
#include<utility>
#include<cassert>
#include<concepts>
#include<functional>
namespace persistent_weight_balanced_tree_impl{
template<typename T>
concept Node=requires(T x){
  {x.left}->std::same_as<T*&>;
  {x.right}->std::same_as<T*&>;
  {x.sz}->std::convertible_to<size_t>;
  {T::push(&x)}->std::same_as<std::pair<T*,T*>>;
  {T::update(&x,&x)}->std::same_as<T*>;
  {T::clone(&x)}->std::same_as<T*>;
};
constexpr bool is_balanced(size_t lsz,size_t rsz){return std::min(lsz,rsz)>=(lsz+rsz)/4;}
template<Node node>
struct PersistentWeightBalancedTree{
private:
  node* merge_rec(node *lnd,node *rnd){
    if(!lnd)return rnd;
    if(!rnd)return lnd;
    if(is_balanced(lnd->sz,rnd->sz))return node::update(lnd,rnd);
    if(lnd->sz>rnd->sz){
      auto [x,y]=node::push(lnd);
      y=merge_rec(y,rnd);
      if(is_balanced(x->sz,y->sz))return node::update(x,y);
      if(is_balanced(x->sz,y->left->sz)&&is_balanced(x->sz+y->left->sz,y->right->sz)){
        auto [y1,y2]=node::push(y);
        return node::update(node::update(x,y1),y2);
      }
      auto [y1,y2]=node::push(y);
      auto [y11,y12]=node::push(y1);
      return node::update(node::update(x,y11),node::update(y12,y2));
    }
    else{
      auto [x,y]=node::push(rnd);
      x=merge_rec(lnd,x);
      if(is_balanced(x->sz,y->sz))return node::update(x,y);
      if(is_balanced(x->right->sz,y->sz)&&is_balanced(x->left->sz,x->right->sz+y->sz)){
        auto [x1,x2]=node::push(x);
        return node::update(x1,node::update(x2,y));
      }
      auto [x1,x2]=node::push(x);
      auto [x21,x22]=node::push(x2);
      return node::update(node::update(x1,x21),node::update(x22,y));
    }
  }
  template<int need=3>
  std::pair<node*,node*>split_rec(node *nd,size_t k){
    if(k==0)return std::make_pair(nullptr,nd);
    if(k==nd->sz)return std::make_pair(nd,nullptr);
    node *x,*y;
    if(k<nd->left->sz){
      if constexpr(need==3)std::tie(x,y)=node::push(nd);
      else std::tie(x,y)=node::template push<need|1>(nd);
      auto [lnd,rnd]=split_rec<need>(x,k);
      if constexpr(need==1)return std::make_pair(lnd,nullptr);
      if constexpr(need==2)return std::make_pair(nullptr,merge_rec(rnd,y));
      return std::make_pair(lnd,merge_rec(rnd,y));
    }
    else{
      if constexpr(need==3)std::tie(x,y)=node::push(nd);
      else std::tie(x,y)=node::template push<need|2>(nd);
      auto [lnd,rnd]=split_rec<need>(y,k-nd->left->sz);
      if constexpr(need==1)return std::make_pair(merge_rec(x,lnd),nullptr);
      if constexpr(need==2)return std::make_pair(nullptr,rnd);
      return std::make_pair(merge_rec(x,lnd),rnd);
    }
  }
  public:
  node *root;
  PersistentWeightBalancedTree():root(nullptr){}
  template<typename T>
  PersistentWeightBalancedTree(const std::vector<T>&init){
    static_assert(requires(T x){node::single(x);});
    auto dfs=[&](auto self,int l,int r)->node* {
      if(l+1==r)return node::single(init[l]);
      int mid=(l+r)/2;
      return node::update(self(self,l,mid),self(self,mid,r));
    };
    root=dfs(dfs,0,init.size());
  }
  template<typename T>
  PersistentWeightBalancedTree(T x){
    static_assert(requires(T x){node::single(x);});
    root=node::single(x);
  }
  PersistentWeightBalancedTree(const PersistentWeightBalancedTree&rhs){
    *this=rhs;
  }
  [[nodiscard]]PersistentWeightBalancedTree merge(PersistentWeightBalancedTree rhs){
    node *nd=merge_rec(root,rhs.root);
    PersistentWeightBalancedTree res;
    res.root=nd;
    return res;
  }
  template<int need=3>
  [[nodiscard]]std::pair<PersistentWeightBalancedTree,PersistentWeightBalancedTree>split(size_t k){
    static_assert(1<=need&&need<=3);
    auto [lnd,rnd]=split_rec<need>(root,k);
    PersistentWeightBalancedTree res1,res2;
    res1.root=lnd;
    res2.root=rnd;
    return std::make_pair(res1,res2);
  }
  PersistentWeightBalancedTree &operator=(const PersistentWeightBalancedTree&rhs){
    if(rhs.root)root=node::clone(rhs.root);
    else root=nullptr;
    return *this;
  }
};
}
using persistent_weight_balanced_tree_impl::PersistentWeightBalancedTree;