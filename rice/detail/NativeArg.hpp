#ifndef Rice__detail__native_arg__hpp_
#define Rice__detail__native_arg__hpp_

#include "ruby.hpp"
#include "rice_traits.hpp"
#include "../Arg.hpp"

namespace Rice::detail
{
  // Primary Template for NativeArg
  template <typename T, typename Enable = void>
  class NativeArg;

  // NativeArg implementation that works on fundamental types. It keeps a copy of the
  // converted value so that a reference or a pointer to the value can be passed to 
  // the native function.
  template <typename T>
  class NativeArg<T, typename std::enable_if_t<is_builtin_v<intrinsic_type<T>>>>
  {
  public:
    using Intrinsic_T = intrinsic_type<T>;
    NativeArg(Arg& arg);
    T nativeValue(VALUE value);
      
  private:
    Intrinsic_T native_;
    Arg arg_;
  };

  // Special case char which is a native type but if we have a pointer we 
  // want to pass through the underlying Ruby pointer
  template <>
  class NativeArg<char*>
  {
  public:
    NativeArg(Arg& arg);
    char* nativeValue(VALUE value);

  private:
    Arg arg_;
  };

  // Special case VALUE which could be a 64 bit integer or Ruby value.
  template <>
  class NativeArg<VALUE>
  {
  public:
    NativeArg(Arg& arg);
    VALUE nativeValue(VALUE value);

  private:
    Arg arg_;
  };

  // NativeArg implementation that works on all other types. The primary use is for 
  // pointers wrapped by Data_Object where there is no reason to store a local copy.
  template <typename T>
  class NativeArg<T, typename std::enable_if_t<!is_builtin_v<intrinsic_type<T>>>>
  {
  public:
    NativeArg() = default;
    NativeArg(Arg& arg);
    T nativeValue(VALUE value);

  private:
    Arg arg_;
  };
}

#include "NativeArg.ipp"

#endif // Rice__detail__native_arg__hpp_
