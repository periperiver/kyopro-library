#pragma once
#include<vector>
#include<array>
#include<memory>
template<typename T>
struct MemoryPool{
  union node{
    node *next;
    alignas(T) char storage[sizeof(T)];
  };
  static constexpr int N=1<<15;
  std::vector<std::unique_ptr<std::array<node,N>>>pool;
  node *head;
  int i,j;
  void expand(){
    pool.emplace_back(std::make_unique<std::array<node,N>>());
  }
  MemoryPool():head(nullptr),i(0),j(0){expand();}
  template<typename...Args>
  T* allocate(Args&&...args){
    node* ptr;
    if(head){
      ptr=head;
      head=head->next;
    }
    else{
      ptr=&((*pool[i])[j++]);
      if(j==N){
        i++,j=0;
        if(i==(int)pool.size())expand();
      }
    }
    return new (ptr->storage) T(std::forward<Args>(args)...);
  }
  void deallocate(T*ptr){
    node *p=reinterpret_cast<node*>(ptr);
    head=p->next=head;
  }
  void clear(){
    head=nullptr;
    i=j=0;
  }
};