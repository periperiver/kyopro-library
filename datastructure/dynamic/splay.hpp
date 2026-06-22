#pragma once
#include<type_traits>
struct has_update_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.update(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_update:public decltype(has_update_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_update_v=has_update<T>::value;
struct has_push_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.push(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_push:public decltype(has_push_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_push_v=has_push<T>::value;
struct has_middle_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.middle,std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_middle:public decltype(has_middle_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_middle_v=has_middle<T>::value;

template<typename T,bool no_push=false>
void splay(T*nd){
  if constexpr(has_push_v<T>&&!no_push)nd->push();
  while(nd->par){
    T *p=nd->par;
    T *pp=p->par;
    if constexpr(has_push_v<T>&&!no_push){
      if(pp)pp->push();
      p->push();
      nd->push();
    }
    if(p->left==nd){
      if(pp){
        if(pp->left==p){
          nd->par=pp->par;
          if(pp->par){
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
          }
          pp->left=p->right;
          if(pp->left)pp->left->par=pp;
          p->left=nd->right;
          if(p->left)p->left->par=p;
          nd->right=p;
          p->par=nd;
          p->right=pp;
          pp->par=p;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
        else if(pp->right==p){
          nd->par=pp->par;
          if(pp->par){
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
          }
          p->left=nd->right;
          if(p->left)p->left->par=p;
          pp->right=nd->left;
          if(pp->right)pp->right->par=pp;
          nd->left=pp;
          pp->par=nd;
          nd->right=p;
          p->par=nd;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
      }
      nd->par=pp;
      if(pp){
        if constexpr(has_middle_v<T>){
          if(pp->middle==p)pp->middle=nd;
          else if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
        else{
          if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
      }
      p->left=nd->right;
      if(p->left)p->left->par=p;
      nd->right=p;
      p->par=nd;
      if constexpr(has_update_v<T>)p->update(),nd->update();
      break;
    }
    else if(p->right==nd){
      if(pp){
        if(pp->left==p){
          nd->par=pp->par;
          if(pp->par){
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
          }
          p->right=nd->left;
          if(p->right)p->right->par=p;
          pp->left=nd->right;
          if(pp->left)pp->left->par=pp;
          nd->left=p;
          p->par=nd;
          nd->right=pp;
          pp->par=nd;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
        else if(pp->right==p){
          nd->par=pp->par;
          if(pp->par){
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
          }
          pp->right=p->left;
          if(pp->right)pp->right->par=pp;
          p->right=nd->left;
          if(p->right)p->right->par=p;
          nd->left=p;
          p->par=nd;
          p->left=pp;
          pp->par=p;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
      }
      nd->par=pp;
      if(pp){
        if constexpr(has_middle_v<T>){
          if(pp->middle==p)pp->middle=nd;
          else if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
        else{
          if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
      }
      p->right=nd->left;
      if(p->right)p->right->par=p;
      nd->left=p;
      p->par=nd;
      if constexpr(has_update_v<T>)p->update(),nd->update();
      break;
    }
    else break;
  }
}
template<typename T>
[[nodiscard]]T* near(T *nd,decltype(T::key)k){
  while(true){
    if(k<nd->key){
      if constexpr(has_push_v<T>)nd->push();
      if(nd->left)nd=nd->left;
      else{
        splay<T,true>(nd);
        return nd;
      }
    }
    else if(nd->key<k){
      if constexpr(has_push_v<T>)nd->push();
      if(nd->right)nd=nd->right;
      else{
        splay<T,true>(nd);
        return nd;
      }
    }
    else{
      splay<T,true>(nd);
      return nd;
    }
  }
  return nullptr;
}
template<typename T>
[[nodiscard]]T* get_k(T *nd,decltype(T::sz)k){
  while(true){
    if constexpr(has_push_v<T>)nd->push();
    decltype(T::sz) lsz=nd->left?nd->left->sz:0;
    if(lsz==k)break;
    else if(k<lsz)nd=nd->left;
    else{
      nd=nd->right;
      k-=1+lsz;
    }
  }
  splay<T,true>(nd);
  return nd;
}
template<typename T>
[[nodiscard]]T* merge(T* l,T *r){
  if(!l)return r;
  if(!r)return l;
  while(r->left){
    if constexpr(has_push_v<T>)r->push();
    r=r->left;
  }
  if constexpr(has_push_v<T>)r->push();
  splay<T,true>(r);
  r->left=l;
  l->par=r;
  if constexpr(has_update_v<T>)r->update();
  return r;
}
template<typename T,bool correct_parent=true>
[[nodiscard]]T* left_most(T*nd){
  T nil;
  T *rnd=&nil;
  while(nd->left){
    if constexpr(has_push_v<T>)nd->push();
    T *c=nd->left;
    if(!c->left){
      rnd->left=nd;
      if constexpr(has_update_v<T>||correct_parent)nd->par=rnd;
      rnd=rnd->left;
      nd=c;
    }
    else{
      if constexpr(has_push_v<T>)c->push();
      nd->left=c->right;
      c->right=nd;
      if constexpr(has_update_v<T>||correct_parent){
        if(nd->left)nd->left->par=nd;
        nd->par=c;
      }
      if constexpr(has_update_v<T>)nd->update();
      if constexpr(has_update_v<T>||correct_parent)c->par=rnd;
      rnd->left=c;
      nd=c->left;
      rnd=rnd->left;
    }
  }
  if constexpr(has_push_v<T>)nd->push();
  rnd->left=nd->right;
  nd->right=nil.left;
  nil.left=nil.right=nullptr;
  if constexpr(has_update_v<T>||correct_parent){
    if(rnd->left)rnd->left->par=rnd;
    if(nd->right)nd->right->par=nd;
    nd->par=nullptr;
  }
  if constexpr(has_update_v<T>){
    if(rnd!=&nil){
      while(rnd){
        rnd->update();
        rnd=rnd->par;
      }
    }
  }
  return nd;
}
template<typename T,bool correct_parent=true>
[[nodiscard]]T* right_most(T*nd){
  T nil;
  T *lnd=&nil;
  while(nd->right){
    if constexpr(has_push_v<T>)nd->push();
    T *c=nd->right;
    if(!c->right){
      lnd->right=nd;
      if constexpr(has_update_v<T>||correct_parent)nd->par=lnd;
      lnd=lnd->right;
      nd=c;
    }
    else{
      if constexpr(has_push_v<T>)c->push();
      nd->right=c->left;
      c->left=nd;
      if constexpr(has_update_v<T>||correct_parent){
        if(nd->right)nd->right->par=nd;
        nd->par=c;
      }
      if constexpr(has_update_v<T>){
        nd->update();
      }
      if constexpr(has_update_v<T>||correct_parent)c->par=lnd;
      lnd->right=c;
      nd=c->right;
      lnd=lnd->right;
    }
  }
  if constexpr(has_push_v<T>)nd->push();
  lnd->right=nd->left;
  nd->left=nil.right;
  nil.left=nil.right=nullptr;
  if constexpr(has_update_v<T>||correct_parent){
    if(lnd->right)lnd->right->par=lnd;
    if(nd->left)nd->left->par=nd;
    nd->par=nullptr;
  }
  if constexpr(has_update_v<T>){
    if(lnd!=&nil){
      while(lnd){
        lnd->update();
        lnd=lnd->par;
      }
    }
  }
  return nd;
}