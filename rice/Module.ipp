#ifndef Rice__Module__ipp_
#define Rice__Module__ipp_

#include "detail/Native_Function.hpp"
#include "Exception.hpp"

namespace Rice
{
  inline Module::Module() : Object(rb_cObject)
  {
  }

  inline Module::Module(VALUE v) : Object(v)
  {
    if (::rb_type(v) != T_CLASS && ::rb_type(v) != T_MODULE)
    {
      throw Exception(
        rb_eTypeError,
        "Expected a Module but got a %s",
        rb_class2name(CLASS_OF(v))); // TODO: might raise an exception
    }
  }

  template<typename Exception_T, typename Functor_T>
  inline Module& Module::add_handler(Functor_T functor)
  {
    // Create a new exception handler and pass ownership of the current handler to it (they
    // get chained together). Then take ownership of the new handler.
    this->handler_ = std::make_shared<detail::Functor_Exception_Handler<Exception_T, Functor_T>>(
      functor, std::move(this->handler_));

    return *this;
  }

  inline std::shared_ptr<detail::Exception_Handler> Module::handler() const
  {
    return this->handler_;
  }

  inline Module& Module::include_module(Module const& inc)
  {
    detail::protect(rb_include_module, *this, inc);
    return *this;
  }

  template<typename Func_T>
  inline Module& Module::define_method(Identifier name, Func_T&& func, Arguments* arguments)
  {
    this->wrap_native_method(*this, name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T, typename...Arg_Ts>
  inline Module& Module::define_method(Identifier name, Func_T&& func, Arg_Ts const& ...args)
  {
    Arguments* arguments = new Arguments(args...);
    this->wrap_native_method(*this, name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T, typename...Arg_Ts>
  inline Module& Module::define_function(Identifier name, Func_T&& func, Arg_Ts const& ...args)
  {
    Arguments* arguments = new Arguments(args...);
    this->wrap_native_function(*this, name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T>
  inline Module& Module::define_singleton_method(Identifier name, Func_T&& func, Arguments* arguments)
  {
    this->wrap_native_method(rb_singleton_class(*this), name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T, typename...Arg_Ts>
  inline Module& Module::define_singleton_method(Identifier name, Func_T&& func, Arg_Ts const& ...args)
  {
    Arguments* arguments = new Arguments(args...);
    this->wrap_native_method(rb_singleton_class(*this), name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T, typename...Arg_Ts>
  inline Module& Module::define_singleton_function(Identifier name, Func_T&& func, Arg_Ts const& ...args)
  {
    Arguments* arguments = new Arguments(args...);
    this->wrap_native_function(rb_singleton_class(*this), name, std::forward<Func_T>(func), this->handler(), arguments);
    return *this;
  }

  template<typename Func_T>
  inline Module& Module::define_module_function(Identifier name, Func_T&& func, Arguments* arguments)
  {
    if (this->rb_type() != T_MODULE)
    {
      throw std::runtime_error("can only define module functions for modules");
    }

    define_function(name, func, arguments);
    define_singleton_function(name, std::forward<Func_T>(func), arguments);
    return *this;
  }

  template<typename Func_T, typename...Arg_Ts>
  inline Module& Module::define_module_function(Identifier name, Func_T&& func, Arg_Ts const& ...args)
  {
    if (this->rb_type() != T_MODULE)
    {
      throw std::runtime_error("can only define module functions for modules");
    }

    define_function(name, std::forward<Func_T>(func), args...);
    define_singleton_function(name, std::forward<Func_T>(func), args...);
    return *this;
  }

  inline Module& Module::const_set(Identifier name,Object value)
  {
    detail::protect(rb_const_set, *this, name, value);
    return *this;
  }

  inline Object Module::const_get(Identifier name) const
  {
    return detail::protect(rb_const_get, *this, name);
  }

  inline bool Module::const_defined(Identifier name) const
  {
    size_t result = detail::protect(rb_const_defined, *this, name);
    return bool(result);
  }

  inline void Module::remove_const(Identifier name)
  {
    detail::protect(rb_mod_remove_const, *this, name.to_sym());
  }

  template<typename T>
  Data_Type<T> Module::define_class(char const* name)
  {
    return this->define_class_with_object_as_base<T>(name);
  }

  inline Module define_module_under(Object module, char const* name)
  {
    VALUE v = rb_define_module_under(module, name);
    return Module(v);
  }

  inline Module define_module(char const* name)
  {
    VALUE v = rb_define_module(name);
    return Module(v);
  }

  inline Module anonymous_module()
  {
    return Module(detail::protect(rb_module_new));
  }

  template<>
  struct detail::From_Ruby<Module>
  {
    static Module convert(VALUE value)
    {
      return Module(value);
    }
  };
}
#endif // Rice__Module__ipp_
