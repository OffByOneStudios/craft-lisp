#pragma once

// #include "language/parser.h"
// adapted https://gist.github.com/ofan/721464
namespace craft {
namespace lisp
{
  // return given mumber as a string
  inline std::string str(long n) { std::ostringstream os; os << n; return os.str(); }

  // return true iff given character is '0'..'9'
  inline bool isdig(char c) { return isdigit(static_cast<unsigned char>(c)) != 0; }


  ////////////////////// cell

  enum cell_type { Symbol, Number, List, Proc, Lambda };

  struct environment; // forward declaration; cell and environment reference each other

  // a variant that can hold any kind of lisp value
  struct cell {
      typedef cell (*proc_type)(const std::vector<cell> &);
      typedef std::vector<cell>::const_iterator iter;
      typedef std::map<std::string, cell> map;
      cell_type type; std::string val; std::vector<cell> list; proc_type proc; environment * env;
      cell(cell_type type = Symbol) : type(type), env(0) {}
      cell(cell_type type, const std::string & val) : type(type), val(val), env(0) {}
      cell(proc_type proc) : type(Proc), proc(proc), env(0) {}
  };

  typedef std::vector<cell> cells;
  typedef cells::const_iterator cellit;

  const cell false_sym(Symbol, "#f");
  const cell true_sym(Symbol, "#t"); // anything that isn't false_sym is true
  const cell nil(Symbol, "nil");


  ////////////////////// environment

  // a dictionary that (a) associates symbols with cells, and
  // (b) can chain to an "outer" dictionary
  struct environment {
      environment(environment * outer = 0) : outer_(outer) {}

      environment(const cells & parms, const cells & args, environment * outer)
      : outer_(outer)
      {
          cellit a = args.begin();
          for (cellit p = parms.begin(); p != parms.end(); ++p)
              env_[p->val] = *a++;
      }

      // map a variable name onto a cell
      typedef std::map<std::string, cell> map;

      // return a reference to the innermost environment where 'var' appears
      map & find(const std::string & var)
      {
          if (env_.find(var) != env_.end())
              return env_; // the symbol exists in this environment
          if (outer_)
              return outer_->find(var); // attempt to find the symbol in some "outer" env
          std::cout << "unbound symbol '" << var << "'\n";
          exit(1);
      }

      // return a reference to the cell associated with the given symbol 'var'
      cell & operator[] (const std::string & var)
      {
          return env_[var];
      }

  private:
      map env_; // inner symbol->cell mapping
      environment * outer_; // next adjacent outer env, or 0 if there are no further environments
  };
  
  // Scheme
  cell proc_add(const cells & c);
  cell proc_sub(const cells & c);
  cell proc_mul(const cells & c);
  cell proc_div(const cells & c);
  
  cell proc_greater(const cells & c);
  cell proc_less(const cells & c);
  cell proc_less_equal(const cells & c);

  cell proc_length(const cells & c);
  cell proc_nullp(const cells & c);
  cell proc_car(const cells & c);
  cell proc_cdr(const cells & c);
  cell proc_append(const cells & c);
  cell proc_cons(const cells & c);
  cell proc_list(const cells & c);
  
  cell eval(cell x, environment * env);
  std::list<std::string> tokenize(const std::string & str);
  cell atom(const std::string & token);
  cell read_from(std::list<std::string> & tokens);
  cell read(const std::string & s);
  std::string to_string(const cell & exp);
  void add_globals(environment & env);
}}
