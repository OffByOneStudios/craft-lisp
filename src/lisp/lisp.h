#pragma once
#include "lisp/common.h"

// #include "language/parser.h"
// adapted https://gist.github.com/ofan/721464
namespace craft {
namespace lisp
{
  // return given mumber as a string
  inline std::string str(long n) { std::ostringstream os; os << n; return os.str(); }

  // return true iff given character is '0'..'9'
  inline bool isdig(char c) { return isdigit(static_cast<unsigned char>(c)) != 0; }


  enum cell_type { Symbol, Number, List, Proc, Lambda, Object };

  class Cell; // forward declaration; cell and environment reference each other
  
  typedef std::vector<Cell> cells;
  typedef cells::const_iterator cellit;
  typedef std::map<std::string, Cell> map;
  
  class Environment
    : public virtual craft::types::Object
  {
      CRAFT_OBJECT_DECLARE(craft::lisp::Environment)
  private:
      // map a variable name onto a cell
    

      map env_; // inner symbol->cell mapping
      Environment* outer_; // next adjacent outer env, or 0 if there are no further environments

  public:
      CRAFT_LISP_EXPORTED Environment();
      CRAFT_LISP_EXPORTED Environment(Environment* outer);
      CRAFT_LISP_EXPORTED Environment(cells const& parms, cells const& args, Environment* env);

      CRAFT_LISP_EXPORTED map& find( std::string const& var);
      CRAFT_LISP_EXPORTED Cell& operator[] (std::string const& var);
  };
  

  // a variant that can hold any kind of lisp value
  class Cell
    : public virtual craft::types::Object
  {
    CRAFT_OBJECT_DECLARE(craft::lisp::Cell)
  public:
      typedef Cell (*proc_type)(std::vector<Cell> const&);
      typedef std::vector<Cell>::const_iterator iter;
      typedef std::map<std::string, Cell> map;

      cell_type type;
      std::string val;
      std::vector<Cell> list;
      proc_type proc;
      instance<Environment> env;

      CRAFT_LISP_EXPORTED Cell();

      CRAFT_LISP_EXPORTED Cell(cell_type type);
      CRAFT_LISP_EXPORTED Cell(cell_type type, const std::string & val);
      CRAFT_LISP_EXPORTED Cell(proc_type proc);
  };

  const Cell false_sym(Symbol, "#f");
  const Cell true_sym(Symbol, "#t"); // anything that isn't false_sym is true
  const Cell nil(Symbol, "nil");

  // Builtins
  Cell proc_add(const cells & c);
  Cell proc_sub(const cells & c);
  Cell proc_mul(const cells & c);
  Cell proc_div(const cells & c);

  Cell proc_greater(const cells & c);
  Cell proc_less(const cells & c);
  Cell proc_less_equal(const cells & c);

  Cell proc_length(const cells & c);
  Cell proc_nullp(const cells & c);
  Cell proc_car(const cells & c);
  Cell proc_cdr(const cells & c);
  Cell proc_append(const cells & c);
  Cell proc_cons(const cells & c);
  Cell proc_list(const cells & c);

  Cell eval(Cell x, instance<Environment> env);
  std::list<std::string> tokenize(const std::string & str);
  Cell atom(const std::string & token);
  Cell read_from(std::list<std::string> & tokens);
  Cell read(const std::string & s);
  std::string to_string(const Cell & exp);
  void add_globals(instance<Environment> env);
}}
