// file      : odb/mysql/transaction-impl.ixx
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  namespace mysql
  {
    inline transaction_impl::connection_type& transaction_impl::
    connection ()
    {
      return *connection_;
    }
  }
}
