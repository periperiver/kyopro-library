#pragma once
#include<numeric>
#include<type_traits>
template<typename I,typename M>
struct DynamicSegmentTree{
private:
  using S=typename M::S;
  static constexpr int dep=std::numeric_limits<I>::digits;
  static constexpr int log_top_size=16;
  static constexpr int top_size=1<<log_top_size;
  static constexpr I mask=(I(1)<<(dep-log_top_size))-1;
  static_assert(log_top_size<=dep);
  static_assert(std::is_integral_v<I>);
  struct node{
    node *left,*right;
    S v;
    node():left(nullptr),right(nullptr),v(M::e()){}
    node(const S&v_):left(nullptr),right(nullptr),v(v_){}
    inline void update(){
      v=M::op(left?left->v:M::e(),right?right->v:M::e());
    }
  };
  node top[top_size*2];
  node *buf[dep-log_top_size];
public:
  DynamicSegmentTree():top{}{}
  void set(I k,const S&x){
    node *nd=&top[top_size+(k>>(dep-log_top_size))];
    for(int i=dep-log_top_size-1;i>=0;i--){
      buf[i]=nd;
      if(k>>i&1){
        if(!nd->right){
          nd->right=new node();
        }
        nd=nd->right;
      }
      else{
        if(!nd->left){
          nd->left=new node();
        }
        nd=nd->left;
      }
    }
    nd->v=x;
    for(int i=0;i<dep-log_top_size;i++)buf[i]->update();
    k=(top_size+(k>>(dep-log_top_size)))>>1;
    while(k){
      top[k].v=M::op(top[k*2].v,top[k*2+1].v);
      k>>=1;
    }
  }
  S prod(I l,I r){
    if(l==r)return M::e();
    S res=M::e();
    if(((l^r)>>(dep-log_top_size))==0){
      node *nd=&top[top_size+(l>>(dep-log_top_size))];
      I nl=0,nr=I(1)<<(dep-log_top_size);
      l&=mask,r&=mask;
      while(nr-nl>1&&nd){
        I mid=(nl+nr)>>1;
        if(r<=mid)nd=nd->left,nr=mid;
        else if(mid<=l)nd=nd->right,nl=mid;
        else break;
      }
      if(!nd)return M::e();
      if(l==nl&&r==nr)return nd->v;
      I nlr,nrl;
      nlr=nrl=(nl+nr)>>1;
      node *ndl=nd->left,*ndr=nd->right;
      while(nl<l&&ndl){
        I mid=(nl+nlr)>>1;
        if(l<mid){
          if(ndl->right)res=M::op(ndl->right->v,res);
          ndl=ndl->left;
          nlr=mid;
        }
        else ndl=ndl->right,nl=mid;
      }
      while(r<nr&&ndr){
        I mid=(nrl+nr)>>1;
        if(mid<r){
          if(ndr->left)res=M::op(res,ndr->left->v);
          ndr=ndr->right;
          nrl=mid;
        }
        else ndr=ndr->left,nr=mid;
      }
      if(ndl)res=M::op(ndl->v,res);
      if(ndr)res=M::op(res,ndr->v);
      return res;
    }
    int top_l=top_size+(l>>(dep-log_top_size))+1,top_r=top_size+(r>>(dep-log_top_size));
    S rsum=M::e();
    while(top_l<top_r){
      if(top_l&1)res=M::op(res,top[top_l++].v);
      if(top_r&1)rsum=M::op(top[--top_r].v,rsum);
      top_l>>=1,top_r>>=1;
    }
    res=M::op(res,rsum);
    node *nd=&top[top_size+(l>>(dep-log_top_size))];
    l&=mask;
    I nl=0,nr=I(1)<<(dep-log_top_size);
    while(nl<l&&nd){
      I mid=(nl+nr)>>1;
      if(l<mid){
        if(nd->right)res=M::op(nd->right->v,res);
        nd=nd->left;
        nr=mid;
      }
      else nd=nd->right,nl=mid;
    }
    if(nd)res=M::op(nd->v,res);
    nd=&top[top_size+(r>>(dep-log_top_size))];
    r&=mask;
    if(r){
      nl=0,nr=I(1)<<(dep-log_top_size);
      while(r<nr&&nd){
        I mid=(nl+nr)>>1;
        if(mid<r){
          if(nd->left)res=M::op(res,nd->left->v);
          nd=nd->right;
          nl=mid;
        }
        else nd=nd->left,nr=mid;
      }
      if(nd)res=M::op(res,nd->v);
    }
    return res;
  }
};