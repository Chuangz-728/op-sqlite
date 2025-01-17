#pragma once

#include <ReactCommon/CallInvoker.h>
#include <jsi/jsi.h>
#include <memory>
#ifdef OP_SQLITE_USE_LIBSQL
#include "libsql.h"
#else
#include <sqlite3.h>
#endif
#include "ThreadPool.h"
#include <string>

namespace opsqlite {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

class PreparedStatementHostObject : public jsi::HostObject {
public:
#ifdef OP_SQLITE_USE_LIBSQL
  PreparedStatementHostObject(
      std::string name, libsql_stmt_t stmt,
      std::shared_ptr<react::CallInvoker> js_call_invoker,
      std::shared_ptr<ThreadPool> thread_pool)
      : _name(name), _stmt(stmt), _js_call_invoker(js_call_invoker),
        _thread_pool(thread_pool){};
#else
  PreparedStatementHostObject(
      std::string name, sqlite3_stmt *stmt,
      std::shared_ptr<react::CallInvoker> js_call_invoker,
      std::shared_ptr<ThreadPool> thread_pool)
      : _name(name), _stmt(stmt), _js_call_invoker(js_call_invoker),
        _thread_pool(thread_pool){};
#endif
  virtual ~PreparedStatementHostObject();

  std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt);

  jsi::Value get(jsi::Runtime &rt, const jsi::PropNameID &propNameID);

private:
  std::shared_ptr<react::CallInvoker> _js_call_invoker;
  std::shared_ptr<ThreadPool> _thread_pool;
  std::string _name;
#ifdef OP_SQLITE_USE_LIBSQL
  libsql_stmt_t _stmt;
#else
  // This shouldn't be de-allocated until sqlite3_finalize is called on it
  sqlite3_stmt *_stmt;
#endif
};

} // namespace opsqlite
