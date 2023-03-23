#ifndef Rice__detail__Native_Function__hpp_
#define Rice__detail__Native_Function__hpp_

#include "ruby.hpp"
#include "Exception_Handler_defn.hpp"
#include "MethodInfo.hpp"
#include "function_traits.hpp"
#include "from_ruby.hpp"

namespace Rice::detail
{
  //! The NativeFunction class calls C++ functions/methods/lambdas on behalf of Ruby
  /*! The NativeFunction class is an intermediate between Ruby and C++. Every method
   *  defined in Rice is associated with a NativeFuntion instance that is stored in
   *  a unordered_map maintained by the MethodData class. The key is the Ruby class
   *  and method.
   * 
   *  When Ruby calls into C++ it invokes the static NativeFunction.call method. This
   *  method then looks up the NativeFunction instance and calls its ->() operator.
   *
   *  The instance then converts each of the arguments passed from Ruby into their
   *  C++ equivalents. It then retrieves the C++ object (if there is one, Ruby could
   *  be calling a free standing method or lambda). Then it calls the C++ method
   *  and gets back the result. If there is a result (so not void), it is converted
   *  from a C++ object to a Ruby object and returned back to Ruby.
   * 
   *  This class make heavy use of C++ Template metaprogramming to determine
   *  the types and parameters a method takes. It then uses that information
   *  to perform type conversion Ruby to C++.
   *   
   *  @tparam From_Ruby_T - The type of C++ class wrapped by Ruby. Note
   *    this may be different than the Class of Function_T. For example, 
   *    std::map has a size() method but that is actually implemented on 
   *    an ancestor class _Tree. Thus From_Ruby_T is std::map but 
   *    Function_T::Class_T is _Tree. This typename must be specified
   *    by the calling code.
   *  @tparam Function_T - A template that represents the C++ function
   *    to call. This typename is automatically deduced by the compiler.
   *  @tparam IsMethod - A boolean specifying whether the function has
   *    a self parameter or not. Rice differentiates these two cases by
   *    calling them methods (self) or functions (no self).
   */

  template<typename From_Ruby_T, typename Function_T, bool IsMethod>
  class NativeFunction
  {
  public:
    // We remove const to avoid an explosion of To_Ruby specializations and Ruby doesn't
    // have the concept of constants anyways
    using Return_T = remove_cv_recursive_t<typename function_traits<Function_T>::return_type>;
    using Class_T = typename method_traits<Function_T, IsMethod>::Class_T;
    using Arg_Ts = typename method_traits<Function_T, IsMethod>::Arg_Ts;
    using From_Ruby_Args_Ts = typename tuple_map<From_Ruby, Arg_Ts>::type;

    // Static member function that Ruby calls
    static VALUE call(int argc, VALUE* argv, VALUE self);

  public:
    NativeFunction(Function_T func, std::shared_ptr<Exception_Handler> handler, MethodInfo* methodInfo);

    // Invokes the wrapped function
    VALUE operator()(int argc, VALUE* argv, VALUE self);

  private:
    template<typename T, std::size_t I>
    From_Ruby<T> createFromRuby();
      
    // Create NativeArgs which are used to convert values from Ruby to C++
    template<std::size_t...I>
    From_Ruby_Args_Ts createFromRuby(std::index_sequence<I...>& indices);

    To_Ruby<Return_T> createToRuby();
      
    // Convert Ruby argv pointer to Ruby values
    std::vector<VALUE> getRubyValues(int argc, VALUE* argv);

    // Convert Ruby values to C++ values
    template<typename std::size_t...I>
    Arg_Ts getNativeValues(std::vector<VALUE>& values, std::index_sequence<I...>& indices);

    // Figure out what self is
    Class_T getSelf(VALUE self);

    // Do we need to keep alive any arguments?
    void checkKeepAlive(VALUE self, VALUE returnValue, std::vector<VALUE>& rubyValues);

    // Call the underlying C++ function
    VALUE invokeNativeFunction(Arg_Ts& nativeArgs);
    VALUE invokeNativeMethod(VALUE self, Arg_Ts& nativeArgs);

  private:
    Function_T func_;
    From_Ruby_Args_Ts fromRubys_;
    To_Ruby<Return_T> toRuby_;
    std::shared_ptr<Exception_Handler> handler_;
    std::unique_ptr<MethodInfo> methodInfo_;
  };
}
#include "NativeFunction.ipp"

#endif // Rice__detail__Native_Function__hpp_