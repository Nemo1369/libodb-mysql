// file      : odb/mysql/mysql.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2005-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_MYSQL_HXX
#define ODB_MYSQL_MYSQL_HXX

#include <odb/mysql/details/config.hxx>

#include <odb/mysql/mysql-types.hxx>

#ifdef _WIN32
#  include <winsock2.h>
#endif

#ifdef LIBODB_MYSQL_INCLUDE_SHORT
#  include <mysql.h>
#  include <errmsg.h>
#  include <mysqld_error.h>
#else
#  include <mysql/mysql.h>
#  include <mysql/errmsg.h>
#  include <mysql/mysqld_error.h>
#endif

#endif // ODB_MYSQL_MYSQL_HXX
