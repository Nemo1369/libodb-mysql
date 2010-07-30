// file      : odb/mysql/transaction.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_TRANSACTION_HXX
#define ODB_MYSQL_TRANSACTION_HXX

#include <odb/transaction.hxx>

#include <odb/mysql/forward.hxx>

namespace odb
{
  namespace mysql
  {
    class transaction_impl;

    class transaction: public odb::transaction
    {
    public:
      typedef mysql::database database_type;
      typedef mysql::connection connection_type;

      explicit
      transaction (transaction_impl*);

      // Return the database this transaction is on.
      //
      database_type&
      database ();

      // Return the underlying database connection for this transaction.
      //
      connection_type&
      connection ();

      // Return current transaction or throw if there is no transaction
      // in effect.
      //
      static transaction&
      current ();

    public:
      transaction_impl&
      implementation ();
    };
  }
}

#include <odb/mysql/transaction.ixx>

#endif // ODB_MYSQL_TRANSACTION_HXX