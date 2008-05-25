#include "method_data.hpp"

#include <ruby.h>


extern "C"
{
#ifdef RUBY_VM
/* YARV */
#include <ruby/node.h>
#else
/* pre-YARV */
#include <node.h>
#include <env.h>
#endif
}

namespace
{

#ifdef RUBY_VM

/* YARV */

struct rb_thread_struct
{   
    VALUE self;
    void *vm;
    VALUE *stack;
    unsigned long stack_size;
    VALUE *cfp;
    /* ... */
};

typedef struct rb_thread_struct rb_thread_t;

extern "C"
{
  extern rb_thread_t * ruby_current_thread;
}

#define CFP_DATA_MEMO_NODE_AND_PC cfp[0]
#define CFP_METHOD_CLASS cfp[11]

/* On YARV, we store the method data on the stack.  We don't have to pop
 * it off the stack, because the stack pointer will be reset to the
 * previous frame's stack pointer when the function returns.
 */
#define FIX_FRAME() \
  do { \
    VALUE * cfp = ruby_current_thread->cfp; \
    CFP_DATA_MEMO_NODE_AND_PC = RBASIC(CFP_METHOD_CLASS)->klass; \
    CFP_METHOD_CLASS = RCLASS_SUPER(CFP_METHOD_CLASS); \
    if(rb_type(CFP_DATA_MEMO_NODE_AND_PC) != T_NODE) \
    { \
      /* This can happen for module functions */ \
      NODE * n = rb_method_node( \
          CFP_METHOD_CLASS, \
          rb_frame_this_func()); \
      CFP_DATA_MEMO_NODE_AND_PC = RBASIC(n->nd_clss)->klass; \
      \
      if(rb_type(CFP_DATA_MEMO_NODE_AND_PC) != T_NODE) \
      { \
        rb_raise(rb_eRuntimeError, "Could not get method data"); \
      } \
    } \
  } while(0)

NODE * data_memo_node()
{
  VALUE * cfp = ruby_current_thread->cfp;
  return (NODE *)CFP_DATA_MEMO_NODE_AND_PC;
}

#else

/* pre-YARV */

/* Okay to not pop this temporary frame, since it will be popped by the
 * caller
 */
#define FIX_FRAME() \
  struct FRAME _frame = *ruby_frame; \
  _frame.last_class = RCLASS(ruby_frame->last_class)->super; \
  _frame.prev = ruby_frame; \
  ruby_frame = &_frame; \

NODE * data_memo_node()
{
  return (NODE *)(RBASIC(ruby_frame->prev->last_class)->klass);
}

#endif

typedef VALUE (*Method_Func)(ANYARGS);

Method_Func actual_cfunc()
{
  return data_memo_node()->nd_cfnc;
}

VALUE data_wrapper_m1(int argc, VALUE * argv, VALUE self)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(argc, argv, self);
  return result;
}

VALUE data_wrapper_0(VALUE self)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self);
  return result;
}

VALUE data_wrapper_1(VALUE self, VALUE arg1)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1);
  return result;
}
VALUE data_wrapper_2(VALUE self, VALUE arg1, VALUE arg2)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2);
  return result;
}
VALUE data_wrapper_3(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3);
  return result;
}
VALUE data_wrapper_4(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4);
  return result;
}
VALUE data_wrapper_5(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5);
  return result;
}
VALUE data_wrapper_6(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6);
  return result;
}
VALUE data_wrapper_7(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return result;
}
VALUE data_wrapper_8(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
  return result;
}
VALUE data_wrapper_9(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
  return result;
}
VALUE data_wrapper_10(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
  return result;
}
VALUE data_wrapper_11(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10, VALUE arg11)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
  return result;
}
VALUE data_wrapper_12(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10, VALUE arg11, VALUE arg12)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
  return result;
}
VALUE data_wrapper_13(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10, VALUE arg11, VALUE arg12, VALUE arg13)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
  return result;
}
VALUE data_wrapper_14(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10, VALUE arg11, VALUE arg12, VALUE arg13, VALUE arg14)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
  return result;
}
VALUE data_wrapper_15(VALUE self, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9, VALUE arg10, VALUE arg11, VALUE arg12, VALUE arg13, VALUE arg14, VALUE arg15)
{
  VALUE result;
  FIX_FRAME();
  result = (*actual_cfunc())(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
  return result;
}

} // namespace

/* Define a method and attach data to it.
 *
 * The method looks to ruby like a normal aliased CFUNC, with a modified
 * origin class:
 *
 * NODE_FBODY
 *   |- (u1) orig - origin class
 *   |  |- basic
 *   |  |  |- flags - origin class flags + FL_SINGLETON
 *   |  |  +- klass - NODE_MEMO
 *   |  |     |- (u1) cfnc - actual C function to call
 *   |  |     |- (u2) rval - stored data
 *   |  |     +- (u3) 0
 *   |  |- iv_tbl - 0
 *   |  |- m_tbl - 0
 *   |  +- super - actual origin class
 *   |- (u2) mid - name of the method
 *   +- (u3) head - NODE_CFUNC
 *      |- (u1) cfnc - wrapper function to call
 *      +- (u2) argc - function arity
 *
 * Or, on YARV:
 *
 * NODE_FBODY
 *   |- (u1) oid - name of the method
 *   +- (u2) body - NODE_METHOD
 *      |- (u1) clss - origin class
 *      |  |- basic
 *      |  |  |- flags - origin class flags + FL_SINGLETON
 *      |  |  +- klass - NODE_MEMO
 *      |  |     |- (u1) cfnc - actual C function to call
 *      |  |     |- (u2) rval - stored data
 *      |  |     +- (u3) 0
 *      |  |- ptr - rb_classext_t
 *      |  |  |- super - actual origin class
 *      |  |  +- iv_tbl - 0
 *      |  |- m_tbl - 0
 *      |  +- iv_index_tbl - 0?
 *      |- (u2) body - NODE_CFUNC
 *      |  |- (u1) cfnc - wrapper function to call
 *      |  +- (u2) argc - function arity
 *      +- (u3) noex - NOEX_PUBLIC
 *
 * When the wrapper function is called, last_class is set to the origin
 * class found in the FBODY node.  So that the method data will be
 * accessible, and so last_class will point to klass and not to our MEMO
 * node, it is necessary to "fix" the current frame.
 *
 * Pre-YARV, this means we duplicate the current frame and set last_class:
 *
 * ruby_frame
 *   |- last_class - klass
 *   |- prev
 *   |  |- last_class - NODE_MEMO
 *   |  |  |- (u1) cfnc - actual C function to call
 *   |  |  |- (u2) rval - stored data
 *   |  |  +- (u3) 0
 *   |  |- prev - the real previous frame
 *   |  +- ...
 *   +- ...
 *
 * The method data is then accessible via
 * ruby_frame->prev->last_class->rval.
 *
 * On YARV, the current frame is not duplicated; rather, the method data
 * is placed on the stack and is referenced by one of the unused members
 * of the control frame (the program counter):
 *
 * ruby_current_thread->cfp
 *   |- pc - NODE_MEMO
 *   |  |- (u1) cfnc - actual C function to call
 *   |  |- (u2) rval - stored data
 *   |  +- (u3) 0
 *   |- method_class - klass
 *   +- ...
 *
 */
VALUE Rice::detail::
define_method_with_data(
    VALUE klass, ID id, VALUE (*cfunc)(ANYARGS), int arity, VALUE data)
{
  /* TODO: origin should have #to_s and #inspect methods defined */
#if RUBY_VERSION_CODE >= 170
  VALUE origin = rb_class_boot(klass);
#else
  VALUE origin = rb_class_new(klass);
#endif
  NODE * node;

  VALUE (*data_wrapper)(ANYARGS);
  switch(arity)
  {
    case 0: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_0); break;
    case 1: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_1); break;
    case 2: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_2); break;
    case 3: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_3); break;
    case 4: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_4); break;
    case 5: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_5); break;
    case 6: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_6); break;
    case 7: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_7); break;
    case 8: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_8); break;
    case 9: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_9); break;
    case 10: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_10); break;
    case 11: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_11); break;
    case 12: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_12); break;
    case 13: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_13); break;
    case 14: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_14); break;
    case 15: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_15); break;
    case -1: data_wrapper = RUBY_METHOD_FUNC(data_wrapper_m1); break;
    default: rb_raise(rb_eArgError, "unsupported arity %d", arity);
  }

  FL_SET(origin, FL_SINGLETON);
  rb_singleton_class_attached(origin, klass);
  rb_name_class(origin, SYM2ID(rb_class_name(klass)));

  RBASIC(origin)->klass = (VALUE)NEW_NODE(NODE_MEMO, cfunc, data, 0);

#ifdef RUBY_VM
  /* YARV */
  node = NEW_FBODY(
      NEW_METHOD(
          NEW_CFUNC(data_wrapper, arity),
          origin,
          NOEX_PUBLIC),
      id);
  st_insert(RCLASS_M_TBL(klass), id, (st_data_t)node);
#else
  /* pre-YARV */
  node = NEW_FBODY(
      NEW_CFUNC(data_wrapper, arity),
      id,
      origin);
  rb_add_method(klass, id, node, NOEX_PUBLIC);
#endif
}

VALUE Rice::detail::
method_data()
{
  return data_memo_node()->nd_rval;
}

