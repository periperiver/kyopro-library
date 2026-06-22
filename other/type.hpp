#pragma once
#include<type_traits>
struct is_modint_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.mod(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct is_modint:public decltype(is_modint_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool is_modint_v=is_modint<T>::value;
struct is_dynamic_modint_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.set_mod((typename T::value_type)0),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct is_dynamic_modint:public decltype(is_dynamic_modint_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool is_dynamic_modint_v=is_dynamic_modint<T>::value;
template<typename T>
inline constexpr bool is_static_modint_v=is_modint_v<T>&&!is_dynamic_modint_v<T>;
struct is_uso_modint_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.uso(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct is_uso_modint:public decltype(is_uso_modint_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool is_uso_modint_v=is_uso_modint<T>::value;