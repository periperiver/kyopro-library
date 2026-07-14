#pragma once
#include<vector>
#include<cstddef>
#include<utility>
#include<cassert>
#include<concepts>
#include<functional>
#include<memory>
namespace persistent_weight_balanced_tree_impl{
template<typename T>
concept Node=requires(T x){
  requires std::same_as<decltype(x.left),T*>||std::same_as<decltype(x.left),std::shared_ptr<T>>;
  requires std::same_as<decltype(x.left),decltype(x.right)>;
  {x.sz}->std::convertible_to<size_t>;
  {T::push(x.left)}->std::same_as<std::pair<decltype(x.left),decltype(x.left)>>;
  {T::update(x.left,x.left)}->std::same_as<decltype(x.left)>;
  {T::clone(x.left)}->std::same_as<decltype(x.left)>;
};
template<Node node,size_t alpha=4>
struct PersistentWeightBalancedTree{
  private:
  constexpr bool is_balanced(size_t lsz,size_t rsz){return std::min(lsz,rsz)>=(lsz+rsz)/alpha;}
  using np=decltype(std::declval<node>().left);
  np merge_rec(np lnd,np rnd){
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
  std::pair<np,np>split_rec(np nd,size_t k){
    if(k==0)return std::make_pair(nullptr,nd);
    if(k==nd->sz)return std::make_pair(nd,nullptr);
    np x,y;
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
  template<typename T>
  np set_rec(np nd,size_t k,T v){
    if(!nd->left)return node::single(v);
    auto [lnd,rnd]=node::push(nd);
    if(k<lnd->sz)lnd=set_rec(lnd,k,v);
    else rnd=set_rec(rnd,k-lnd->sz,v);
    return node::update(lnd,rnd);
  }
  np root;
public:
  PersistentWeightBalancedTree():root(nullptr){}
  template<typename T>
  PersistentWeightBalancedTree(const std::vector<T>&init){
    static_assert(requires(T x){node::single(x);});
    if(init.empty()){
      root=nullptr;
      return;
    }
    auto dfs=[&](auto self,int l,int r)->np {
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
  PersistentWeightBalancedTree(PersistentWeightBalancedTree&&rhs):root(rhs.root){}
  [[nodiscard]]PersistentWeightBalancedTree merge(PersistentWeightBalancedTree rhs){
    np nd=merge_rec(root,rhs.root);
    PersistentWeightBalancedTree res;
    res.root=nd;
    return res;
  }
  template<int need=3>
  [[nodiscard]]std::pair<PersistentWeightBalancedTree,PersistentWeightBalancedTree>split(size_t k){
    static_assert(1<=need&&need<=3);
    assert(k<=size());
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
  np operator->(){return root;}
  np get_root()const{return root;}
  np get(size_t k)const{
    assert(root&&k<root->sz);
    np nd=root;
    while(nd->left){
      auto [lnd,rnd]=node::push(nd);
      if(k<lnd->sz)nd=lnd;
      else k-=lnd->sz,nd=rnd;
    }
    return nd;
  }
  template<typename T>
  [[nodiscard]]PersistentWeightBalancedTree set(size_t k,T v){
    static_assert(requires(T x){node::single(x);});
    assert(root&&k<root->sz);
    PersistentWeightBalancedTree res;
    res.root=set_rec(root,k,v);
    return res;
  }
  size_t size()const{return root?root->sz:0;}
};
}
using persistent_weight_balanced_tree_impl::PersistentWeightBalancedTree;