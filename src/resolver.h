#ifndef RESOLVER
#define RESOLVER

struct _add_exception {
  template <class T, class UB,class UB2,class UB3,class ...A>
  struct resolver {
    static void exc(...);
  };

  template <class T, class Ret,class Error,class Exception>
  struct resolver <T, Ret(T::*)(Error&),Error,Exception> {
    typedef Ret(T::*F)(Error&);

    static Ret exc(T *obj, F f) throw(Exception){
      Error err;
      auto result = (obj->*f)(err);
      if(err)
          throw(Exception(err));
      return result;
    }
  };

  template <class T, class Ret,class Error,class Exception, class ...A>
  struct resolver <T, Ret(T::*)(Error&, A...),Error,Exception,A...> {
    typedef Ret(T::*F)(Error&, A...);
    static Ret exc(T *obj, F f, A... arg) throw(Exception){
      Error err;
      auto result = (obj->*f)(err, arg...);
      if(err)
          throw(Exception(err));
      return result;
    }
  };

  template<class M,class T,class Error,class Exception, class ...A>
  static auto exc(T *obj, M m, A ...a) throw(Exception) -> decltype(resolver<T, M,Error,Exception,A...>::exc(obj, m, a...))  {
    return resolver<T, M,Error,Exception,A...>::exc(obj, m, a...);
  }
};

#endif // RESOLVER

