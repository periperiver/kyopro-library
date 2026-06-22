#pragma once
#include<cstddef>
#include<utility>
#include<cassert>
#include<concepts>
namespace PersistentWeightBalancedTree{
namespace internal{
template<typename T>
concept Node=requires(T x){
  {x.left}->std::same_as<T*&>;
  {x.right}->std::same_as<T*&>;
  {x.sz}->std::convertible_to<size_t>;
};
template<typename T,typename...Args>
concept has_create_node=requires(){
  {T::create_node(std::declval<Args>()...)}->std::same_as<T*>;
};
inline bool is_balanced(size_t lsz,size_t rsz){
  if(lsz>rsz)std::swap(lsz,rsz);
  return lsz>=(lsz+rsz)/4;
}
template<Node node,typename...Args>
inline node* create_node(Args&&...args){
  static_assert(std::is_constructible_v<node,Args...>||has_create_node<node,Args...>);
  if constexpr(has_create_node<node,Args...>)return node::create_node(std::forward<Args>(args)...);
  else return new node(std::forward<Args>(args)...);
}
template<Node node>
node* merge_rec(node*lnd,node *rnd){
  if(!lnd)return rnd;
  if(!rnd)return lnd;
  if(is_balanced(lnd->sz,rnd->sz))return create_node<node>(lnd,rnd);
  if(lnd->sz>rnd->sz){
    node *x=lnd->left;
    node *y=merge_rec(lnd->right,rnd);
    if(is_balanced(x->sz,y->sz))return create_node<node>(x,y);
    if(is_balanced(x->sz,y->left->sz)&&is_balanced(x->sz+y->left->sz,y->right->sz))return create_node<node>(create_node<node>(x,y->left),y->right);
    return create_node<node>(create_node<node>(x,y->left->left),create_node<node>(y->left->right,y->right));
  }
  else{
    node *x=merge_rec(lnd,rnd->left);
    node *y=rnd->right;
    if(is_balanced(x->sz,y->sz))return create_node<node>(x,y);
    if(is_balanced(x->right->sz,y->sz)&&is_balanced(x->left->sz,x->right->sz+y->sz))return create_node<node>(x->left,create_node<node>(x->right,y));
    return create_node<node>(create_node<node>(x->left,x->right->left),create_node<node>(x->right->right,y));
  }
}
template<Node node>
std::pair<node*,node*>split_rec(node *nd,size_t k){
  if(k==0)return std::make_pair(nullptr,nd);
  if(k==nd->sz)return std::make_pair(nd,nullptr);
  if(k<nd->left->sz){
    auto [lnd,rnd]=split_rec(nd->left,k);
    return std::make_pair(lnd,merge_rec(rnd,nd->right));
  }
  else{
    auto [lnd,rnd]=split_rec(nd->right,k-nd->left->sz);
    return std::make_pair(merge_rec(nd->left,lnd),rnd);
  }
}
}
template<internal::Node node>
[[nodiscard]]node* merge(node *lnd,node *rnd){
  return internal::merge_rec(lnd,rnd);
}
template<internal::Node node>
[[nodiscard]]std::pair<node*,node*>split(node *nd,size_t k){
  if(!nd){
    assert(k==0);
    return std::make_pair(nullptr,nullptr);
  }
  assert(k<=nd->sz);
  return internal::split_rec(nd,k);
}
}