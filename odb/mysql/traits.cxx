// file      : odb/mysql/traits.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2005-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::memcpy, std::strlen

#include <odb/mysql/traits.hxx>

using namespace std;

namespace odb
{
  namespace mysql
  {
    using details::buffer;

    //
    // string_value_traits
    //

    void string_value_traits::
    set_image (buffer& b,
               size_t& n,
               bool& is_null,
               const string& v)
    {
      is_null = false;
      n = v.size ();

      if (n > b.capacity ())
        b.capacity (n);

      if (n != 0)
        memcpy (b.data (), v.c_str (), n);
    }

    //
    // c_string_value_traits
    //

    void c_string_value_traits::
    set_image (buffer& b,
               size_t& n,
               bool& is_null,
               const char* v)
    {
      is_null = false;
      n = strlen (v);

      if (n > b.capacity ())
        b.capacity (n);

      if (n != 0)
        memcpy (b.data (), v, n);
    }
  }
}
