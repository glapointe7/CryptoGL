
#ifndef FUNCTIONCOMPOSITION_HPP
#define FUNCTIONCOMPOSITION_HPP

/*
 * Compose a function F on itself. 
 * Usage for non-members class : Type X = compose<n>(function)(arg1);
 * Usage for class members : Type X = compose<n>(std::bind(classname::function, this, std::placeholders::_1))(arg1);
 */
template <typename F>
class Composer 
{
private:
   uint32_t n_;
   F f_;
   
public:
   Composer(uint32_t n, F f) : n_(n), f_(f) {}

   template <typename T>
   T operator()(T x) const 
   {
      uint32_t n = n_;
      while (n--) 
      {
         x = f_(x);
      }

      return x;
   }
};

template <uint32_t N, typename F>
Composer<F> compose(F f) 
{
  return Composer<F>(N, f);
}


/*
 * Usage : Compose(f,g,h,...)(value).
 */
template <typename F0, typename... F>
class Composer2 
{
private:
   F0 f0_;
   Composer2<F...> tail_;

public:
   Composer2(F0 f0, F... f) : f0_(f0), tail_(f...) {}

   template <typename T>
   T operator() (const T& x) const 
   {
      return f0_(tail_(x));
   }
};
 
template <typename F>
class Composer2<F> 
{
private:
   F f_;
   
public:
   Composer2(F f) : f_(f) {}

   template <typename T>
   T operator() (const T& x) const 
   {
      return f_(x);
   }
};
 
template <typename... F>
Composer2<F...> composer(F... f) 
{
   return Composer2<F...>(f...);
}

#endif