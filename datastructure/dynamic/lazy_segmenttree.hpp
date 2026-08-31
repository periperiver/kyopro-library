#pragma once
template<typename I,typename M>
struct DynamicLazySegmentTree{
private:
  using S=typename M::M1::S;
  using F=typename M::M2::S;
  struct node{
    I l,r;
    node *left,*right,*par;
    S val;
    F lazy;
    node(I l,I r,S val):l(l),r(r),val(val),lazy(M::M2::e()),left(nullptr),right(nullptr),par(nullptr){}
    void propagate(F f){
      val=M::act(val,f);
      lazy=M::M2::op(lazy,f);
    }
    void push(){
      I mid=(l+r)/2;
      if(!left){
        left=new node(l,mid,M::M1::e());
        left->par=this;
      }
      if(!right){
        right=new node(mid,r,M::M1::e());
        right->par=this;
      }
      left->propagate(lazy);
      right->propagate(lazy);
      lazy=M::M2::e();
    }
    inline void update(){
      val=M::M1::op(left->val,right->val);
    }
  };
  node *root;
public:
  DynamicLazySegmentTree():root(nullptr){}
  DynamicLazySegmentTree(I l,I r){
    root=new node(l,r,M::M1::e());
  }
  DynamicLazySegmentTree(const DynamicLazySegmentTree<I,M>&seg){
    auto dfs=[&](auto self,node *nd)->node* {
      if(!nd)return nullptr;
      node *ret=new node(*nd);
      ret->left=self(self,nd->left);
      ret->right=self(self,nd->right);
      ret->left->par=ret;
      ret->right->par=ret;
      return ret;
    };
    root=dfs(dfs,seg.root);
  }
  void set(I k,S x){
    node *nd=root;
    while(nd->r-nd->l>1){
      I mid=(nd->l+nd->r)/2;
      nd->push();
      if(k<mid)nd=nd->left;
      else nd=nd->right;
    }
    nd->val=x;
    nd=nd->par;
    while(nd){
      nd->update();
      nd=nd->par;
    }
  }
  S get(I k){
    node *nd=root;
    while(nd->r-nd->l>1){
      I mid=(nd->l+nd->r)/2;
      nd->push();
      if(k<mid)nd=nd->left;
      else nd=nd->right;
    }
    return nd->val;
  }
  void apply(I l,I r,const F&f){
    if(l==r)return;
    node *nd=root;
    if(l<=nd->l&&nd->r<=r){
      nd->propagate(f);
      return;
    }
    while(nd->r-nd->l>1){
      I mid=(nd->l+nd->r)/2;
      nd->push();
      if(r<=mid)nd=nd->left;
      else if(mid<=l)nd=nd->right;
      else break;
    }
    if(nd->l==l&&nd->r==r){
      nd->propagate(f);
      nd=nd->par;
      while(nd){
        nd->update();
        nd=nd->par;
      }
    }
    else{
      node *le=nd->left,*ri=nd->right;
      while(le->l<l){
        I mid=(le->l+le->r)/2;
        le->push();
        if(l<mid){
          le->right->propagate(f);
          le=le->left;
        }
        else le=le->right;
      }
      le->propagate(f);
      le=le->par;
      while(le!=nd){
        le->update();
        le=le->par;
      }
      while(ri->r>r){
        I mid=(ri->l+ri->r)/2;
        ri->push();
        if(r<=mid)ri=ri->left;
        else{
          ri->left->propagate(f);
          ri=ri->right;
        }
      }
      ri->propagate(f);
      ri=ri->par;
      while(ri!=nd){
        ri->update();
        ri=ri->par;
      }
      while(nd){
        nd->update();
        nd=nd->par;
      }
    }
  }
  S prod(I l,I r){
    if(l==r)return M::M1::e();
    node *nd=root;
    S sum=M::M1::e();
    while(nd->r-nd->l>1){
      I mid=(nd->l+nd->r)/2;
      nd->push();
      if(r<=mid)nd=nd->left;
      else if(mid<=l)nd=nd->right;
      else break;
    }
    if(nd->l==l&&nd->r==r)return nd->val;
    node *le=nd->left,*ri=nd->right;
    while(le->l<l){
      I mid=(le->l+le->r)/2;
      le->push();
      if(l<mid){
        sum=M::M1::op(le->right->val,sum);
        le=le->left;
      }
      else le=le->right;
    }
    sum=M::M1::op(le->val,sum);
    while(ri->r>r){
      I mid=(ri->l+ri->r)/2;
      ri->push();
      if(r<=mid)ri=ri->left;
      else{
        sum=M::M1::op(sum,ri->left->val);
        ri=ri->right;
      }
    }
    sum=M::M1::op(sum,ri->val);
    return sum;
  }
  S all_prod()const{
    return root->val;
  }
};