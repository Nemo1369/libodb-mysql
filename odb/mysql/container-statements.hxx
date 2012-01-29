// file      : odb/mysql/container-statements.hxx
// copyright : Copyright (c) 2005-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_CONTAINER_STATEMENTS_HXX
#define ODB_MYSQL_CONTAINER_STATEMENTS_HXX

#include <odb/pre.hxx>

#include <cstddef> // std::size_t

#include <odb/forward.hxx>
#include <odb/traits.hxx>

#include <odb/mysql/mysql.hxx>
#include <odb/mysql/version.hxx>
#include <odb/mysql/statement.hxx>

#include <odb/mysql/details/export.hxx>

namespace odb
{
  namespace mysql
  {
    class connection;

    // Template argument is the generated abstract container traits type.
    // That is, it doesn't need to provide column counts and statements.
    //
    template <typename T>
    class container_statements
    {
    public:
      typedef T traits;

      typedef typename traits::data_image_type data_image_type;
      typedef typename traits::cond_image_type cond_image_type;

      typedef typename traits::functions_type functions_type;

      typedef mysql::insert_statement insert_statement_type;
      typedef mysql::select_statement select_statement_type;
      typedef mysql::delete_statement delete_statement_type;

      typedef mysql::connection connection_type;

      container_statements (connection_type&);

      connection_type&
      connection ()
      {
        return conn_;
      }

      // Functions.
      //
      functions_type&
      functions ()
      {
        return functions_;
      }

      // Id image binding (external).
      //
      const binding&
      id_binding ()
      {
        return *id_binding_;
      }

      void
      id_binding (const binding& b)
      {
        id_binding_ = &b;
      }

      // Condition image.
      //
      cond_image_type&
      cond_image ()
      {
        return cond_image_;
      }

      std::size_t
      cond_image_version () const
      {
        return cond_image_version_;
      }

      void
      cond_image_version (std::size_t v)
      {
        cond_image_version_ = v;
      }

      std::size_t
      cond_id_binding_version () const
      {
        return cond_id_binding_version_;
      }

      void
      cond_id_binding_version (std::size_t v)
      {
        cond_id_binding_version_ = v;
      }

      binding&
      cond_image_binding ()
      {
        return cond_image_binding_;
      }

      // Data image.
      //
      data_image_type&
      data_image ()
      {
        return data_image_;
      }

      std::size_t
      data_image_version () const
      {
        return data_image_version_;
      }

      void
      data_image_version (std::size_t v)
      {
        data_image_version_ = v;
      }

      std::size_t
      data_id_binding_version () const
      {
        return data_id_binding_version_;
      }

      void
      data_id_binding_version (std::size_t v)
      {
        data_id_binding_version_ = v;
      }

      binding&
      data_image_binding ()
      {
        return data_image_binding_;
      }

      my_bool*
      data_image_truncated ()
      {
        return data_image_truncated_;
      }

      binding&
      select_image_binding ()
      {
        return select_image_binding_;
      }

      my_bool*
      select_image_truncated ()
      {
        return select_image_truncated_;
      }

      //
      // Statements.
      //

      insert_statement_type&
      insert_one_statement ()
      {
        if (insert_one_ == 0)
          insert_one_.reset (
            new (details::shared) insert_statement_type (
              conn_, insert_one_text_, data_image_binding_, false));

        return *insert_one_;
      }

      select_statement_type&
      select_all_statement ()
      {
        if (select_all_ == 0)
          select_all_.reset (
            new (details::shared) select_statement_type (
              conn_,
              select_all_text_,
              cond_image_binding_,
              select_image_binding_,
              false));

        return *select_all_;
      }

      delete_statement_type&
      delete_all_statement ()
      {
        if (delete_all_ == 0)
          delete_all_.reset (
            new (details::shared) delete_statement_type (
              conn_, delete_all_text_, cond_image_binding_, false));

        return *delete_all_;
      }

    private:
      container_statements (const container_statements&);
      container_statements& operator= (const container_statements&);

    protected:
      connection_type& conn_;
      functions_type functions_;

      const binding* id_binding_;

      cond_image_type cond_image_;
      std::size_t cond_image_version_;
      std::size_t cond_id_binding_version_;
      binding cond_image_binding_;

      data_image_type data_image_;
      std::size_t data_image_version_;
      std::size_t data_id_binding_version_;

      binding data_image_binding_;
      my_bool* data_image_truncated_;

      // Skips the id from data_image_binding.
      //
      binding select_image_binding_;
      my_bool* select_image_truncated_;

      const char* insert_one_text_;
      const char* select_all_text_;
      const char* delete_all_text_;

      details::shared_ptr<insert_statement_type> insert_one_;
      details::shared_ptr<select_statement_type> select_all_;
      details::shared_ptr<delete_statement_type> delete_all_;
    };

    // Template argument is the generated concrete container traits type.
    //
    template <typename T>
    class container_statements_impl: public T::statements_type
    {
    public:
      typedef T traits;
      typedef typename T::statements_type base;
      typedef mysql::connection connection_type;

      container_statements_impl (connection_type&);

    private:
      container_statements_impl (const container_statements_impl&);
      container_statements_impl& operator= (const container_statements_impl&);

    private:
      MYSQL_BIND cond_image_bind_[traits::cond_column_count];
      MYSQL_BIND data_image_bind_[traits::data_column_count];
      my_bool data_image_truncated_array_[traits::data_column_count];
    };
  }
}

#include <odb/mysql/container-statements.txx>

#include <odb/post.hxx>

#endif // ODB_MYSQL_CONTAINER_STATEMENTS_HXX
