// file      : odb/mysql/statement.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2005-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <mysql/mysqld_error.h> // ER_DUP_ENTRY

#include <new> // std::bad_alloc

#include <odb/mysql/statement.hxx>
#include <odb/mysql/connection.hxx>
#include <odb/mysql/exceptions.hxx>

using namespace std;

namespace odb
{
  namespace mysql
  {
    // statement
    //

    statement::
    statement (connection& conn)
        : conn_ (conn)
    {
      stmt_ = mysql_stmt_init (conn_.handle ());

      if (stmt_ == 0)
        throw bad_alloc ();
    }

    statement::
    ~statement ()
    {
      mysql_stmt_close (stmt_);
    }

    // insert_statement
    //

    insert_statement::
    ~insert_statement ()
    {
    }

    insert_statement::
    insert_statement (connection& conn,
                      const string& query,
                      binding& image)
        : statement (conn), image_ (image), version_ (0)
    {
      if (mysql_stmt_prepare (stmt_, query.c_str (), query.size ()) != 0)
        throw database_exception (stmt_);
    }

    void insert_statement::
    execute ()
    {
      if (mysql_stmt_reset (stmt_))
        throw database_exception (stmt_);

      if (version_ != image_.version)
      {
        if (mysql_stmt_bind_param (stmt_, image_.bind))
          throw database_exception (stmt_);

        version_ = image_.version;
      }

      if (mysql_stmt_execute (stmt_))
      {
        unsigned int e (mysql_stmt_errno (stmt_));

        if (e == ER_DUP_ENTRY)
          throw object_already_persistent ();
        else
          throw database_exception (stmt_);
      }

      /*
        @@ Should I throw unknown error or some such?

      if (mysql_stmt_affected_rows (stmt_) != 1)
        throw object_already_persistent ();
      */
    }

    // select_statement
    //

    select_statement::
    ~select_statement ()
    {
    }

    select_statement::
    select_statement (connection& conn,
                      const string& query,
                      binding& id,
                      binding& image)
        : statement (conn),
          id_ (id),
          id_version_ (0),
          image_ (image),
          image_version_ (0)
    {
      if (mysql_stmt_prepare (stmt_, query.c_str (), query.size ()) != 0)
        throw database_exception (stmt_);
    }

    select_statement::result select_statement::
    execute ()
    {
      if (mysql_stmt_reset (stmt_))
        throw database_exception (stmt_);

      if (id_version_ != id_.version)
      {
        if (mysql_stmt_bind_param (stmt_, id_.bind))
          throw database_exception (stmt_);

        id_version_ = id_.version;
      }

      if (image_version_ != image_.version)
      {
        if (mysql_stmt_bind_result (stmt_, image_.bind))
          throw database_exception (stmt_);

        image_version_ = image_.version;
      }

      if (mysql_stmt_execute (stmt_))
        throw database_exception (stmt_);

      int r (mysql_stmt_fetch (stmt_));

      switch (r)
      {
      case 0:
        {
          return success;
        }
      case 1:
        {
          throw database_exception (stmt_);
        }
      case MYSQL_NO_DATA:
        {
          free_result ();
          return no_data;
        }
      case MYSQL_DATA_TRUNCATED:
        {
          return truncated;
        }
      }
    }

    void select_statement::
    refetch ()
    {
      // Re-fetch columns that were truncated.
      //
      for (size_t i (0); i < image_.count; ++i)
      {
        if (*image_.bind[i].error)
        {
          *image_.bind[i].error = 0;

          if (mysql_stmt_fetch_column (
                stmt_, image_.bind + i, static_cast<unsigned int> (i), 0))
            throw database_exception (stmt_);
        }
      }
    }

    void select_statement::
    free_result ()
    {
      if (mysql_stmt_free_result (stmt_))
        throw database_exception (stmt_);
    }

    // update_statement
    //

    update_statement::
    ~update_statement ()
    {
    }

    update_statement::
    update_statement (connection& conn,
                      const string& query,
                      binding& id,
                      binding& image)
        : statement (conn),
          id_ (id),
          id_version_ (0),
          image_ (image),
          image_version_ (0)
    {
      if (mysql_stmt_prepare (stmt_, query.c_str (), query.size ()) != 0)
        throw database_exception (stmt_);
    }

    void update_statement::
    execute ()
    {
      if (mysql_stmt_reset (stmt_))
        throw database_exception (stmt_);

      if (image_version_ != image_.version || id_version_ != id_.version)
      {
        // Here we assume that the last element in image_.bind is the
        // id parameter.
        //
        if (mysql_stmt_bind_param (stmt_, image_.bind))
          throw database_exception (stmt_);

        id_version_ = id_.version;
        image_version_ = image_.version;
      }

      if (mysql_stmt_execute (stmt_))
        throw database_exception (stmt_);

      my_ulonglong r (mysql_stmt_affected_rows (stmt_));

      if (r > 0)
        return;

      if (r == 0)
        throw object_not_persistent ();
      else
        throw database_exception (stmt_);
    }

    // delete_statement
    //

    delete_statement::
    ~delete_statement ()
    {
    }

    delete_statement::
    delete_statement (connection& conn,
                      const string& query,
                      binding& id)
        : statement (conn), id_ (id), version_ (0)
    {
      if (mysql_stmt_prepare (stmt_, query.c_str (), query.size ()) != 0)
        throw database_exception (stmt_);
    }

    void delete_statement::
    execute ()
    {
      if (mysql_stmt_reset (stmt_))
        throw database_exception (stmt_);

      if (version_ != id_.version)
      {
        if (mysql_stmt_bind_param (stmt_, id_.bind))
          throw database_exception (stmt_);

        version_ = id_.version;
      }

      if (mysql_stmt_execute (stmt_))
        throw database_exception (stmt_);

      my_ulonglong r (mysql_stmt_affected_rows (stmt_));

      if (r > 0)
        return;

      if (r == 0)
        throw object_not_persistent ();
      else
        throw database_exception (stmt_);
    }

    // object_statements_base
    //

    object_statements_base::
    ~object_statements_base ()
    {
    }
  }
}