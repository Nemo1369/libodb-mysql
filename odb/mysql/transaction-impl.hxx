// file      : odb/mysql/transaction-impl.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_TRANSACTION_IMPL_HXX
#define ODB_MYSQL_TRANSACTION_IMPL_HXX

#include <odb/pre.hxx>

#include <odb/transaction.hxx>

#include <odb/mysql/version.hxx>
#include <odb/mysql/forward.hxx>

#include <odb/details/shared-ptr.hxx>

#include <odb/mysql/details/export.hxx>

namespace odb
{
  namespace mysql
  {
    class LIBODB_MYSQL_EXPORT transaction_impl: public odb::transaction_impl
    {
    protected:
      friend class database;
      friend class transaction;

      typedef mysql::database database_type;
      typedef mysql::connection connection_type;

      transaction_impl (database_type&);

      virtual
      ~transaction_impl ();

      virtual void
      commit ();

      virtual void
      rollback ();

      connection_type&
      connection ();

    private:
      details::shared_ptr<connection_type> connection_;
    };
  }
}

#include <odb/mysql/transaction-impl.ixx>

#include <odb/post.hxx>

#endif // ODB_MYSQL_TRANSACTION_IMPL_HXX
