#ifndef Rice__detail__cpp_protect__hpp_
#define Rice__detail__cpp_protect__hpp_

#include <regex>
#include <stdexcept>

// Check which filesystem to include yet
#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
  #if defined(__cpp_lib_filesystem) // Check for feature test macro for <filesystem>
    #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
  #elif defined(__cpp_lib_experimental_filesystem) // Check for feature test macro for <experimental/filesystem>
    #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
  #elif !defined(__has_include) // We can't check if headers exist...Let's assume experimental to be safe
    #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
  #elif __has_include(<filesystem>) // Check if the header "<filesystem>" exists
    #ifdef _MSC_VER // If we're compiling on Visual Studio and are not compiling with C++17, we need to use experimental
      #if __has_include(<yvals_core.h>) // Check and include header that defines "_HAS_CXX17"
        #include <yvals_core.h>
        #if defined(_HAS_CXX17) && _HAS_CXX17 // Check for enabled C++17 support
          #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0 // We're using C++17, so let's use the normal version
        #endif
      #endif
      #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL // If the marco isn't defined yet, that means any of the other VS specific checks failed, so we need to use experimental
        #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
      #endif
    #else // #ifdef _MSC_VER - Not on Visual Studio. Let's use the normal version
      #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
    #endif
  #elif __has_include(<experimental/filesystem>) // Check if the header "<filesystem>" exists
    #define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
  #else // Fail if neither header is available with a nice error message
    #error Could not find system header "<filesystem>" or "<experimental/filesystem>"
  #endif

  // We previously determined that we need the exprimental version
  #if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
  #else // We have a decent compiler and can use the normal version
    #include <filesystem>
    namespace fs = std::filesystem;
  #endif
#endif // #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

#include "Jump_Tag.hpp"
#include "../Exception_defn.hpp"

namespace Rice::detail
{
  template <typename Callable_T>
  auto cpp_protect(Callable_T&& func)
  {
    try
    {
      return func();
    }
    catch (...)
    {
      try
      {
        detail::Registries::instance.handlers.handler()->handle();
      }
      catch (::Rice::Exception const& ex)
      {
        rb_exc_raise(ex.value());
      }
      catch (::Rice::Jump_Tag const& ex)
      {
        rb_jump_tag(ex.tag);
      }
      catch (std::bad_alloc const& ex)
      {
        /* This won't work quite right if the rb_exc_new2 fails; not
           much we can do about that, since Ruby doesn't give us access
           to a pre-allocated NoMemoryError object */
        rb_exc_raise(rb_exc_new2(rb_eNoMemError, ex.what()));
      }
      catch (std::domain_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eFloatDomainError, ex.what()));
      }
      catch (std::invalid_argument const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eArgError, ex.what()));
      }
      catch (fs::filesystem_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eIOError, ex.what()));
      }
      catch (std::length_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRuntimeError, ex.what()));
      }
      catch (std::out_of_range const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRangeError, ex.what()));
      }
      catch (std::overflow_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRangeError, ex.what()));
      }
      catch (std::range_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRangeError, ex.what()));
      }
      catch (std::regex_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRegexpError, ex.what()));
      }
      catch (std::system_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eSystemCallError, ex.what()));
      }
      catch (std::underflow_error const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRangeError, ex.what()));
      }
      catch (std::exception const& ex)
      {
        rb_exc_raise(rb_exc_new2(rb_eRuntimeError, ex.what()));
      }
      catch (...)
      {
        rb_exc_raise(rb_exc_new2(rb_eRuntimeError, "Unknown C++ exception thrown"));
      }
      throw std::runtime_error("Should never get here - just making compilers happy");
    }
  }
}
#endif // Rice__detail__cpp_protect__hpp_
