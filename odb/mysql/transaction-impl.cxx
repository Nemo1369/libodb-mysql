// file      : odb/mysql/transaction-impl.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/mysql/mysql.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/connection.hxx>
#include <odb/mysql/error.hxx>
#include <odb/mysql/transaction-impl.hxx>

namespace odb
{
  namespace mysql
  {
    transaction_impl::
    transaction_impl (database_type& db)
        : odb::transaction_impl (db)
    {
    }

    transaction_impl::
    transaction_impl (connection_ptr c)
        : odb::transaction_impl (c->database (), *c), connection_ (c)
    {
    }

    transaction_impl::
    ~transaction_impl ()
    {
    }

    void transaction_impl::
    start ()
    {
      // Grab a connection if we don't already have one.
      //
      if (connection_ == 0)
      {
        connection_ = static_cast<database_type&> (database_).connection ();
        odb::transaction_impl::connection_ = connection_.get ();
      }

      if (mysql_real_query (connection_->handle (), "begin", 5) != 0)
        translate_error (*connection_);
    }

    void transaction_impl::
    commit ()
    {
      connection_->clear ();

      if (mysql_real_query (connection_->handle (), "commit", 6) != 0)
        translate_error (*connection_);

      // Release the connection.
      //
      //connection_.reset ();
    }

    void transaction_impl::
    rollback ()
    {
      connection_->clear ();

      if (mysql_real_query (connection_->handle (), "rollback", 8) != 0)
        translate_error (*connection_);

      // Release the connection.
      //
      //connection_.reset ();
    }
  }
}
