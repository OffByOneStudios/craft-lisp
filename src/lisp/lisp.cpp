// Scheme Interpreter in 90 lines of C++ (not counting lines after the first 90).
// Inspired by Peter Norvig's Lis.py.
// Copyright (c) 2010 Anthony C. Hay. This program leaks memory.
#include "common.h"
#include "lisp.h"

using namespace craft;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Cell)
{
  _.defaults();
}

CRAFT_OBJECT_DEFINE(Environment)
{
  _.defaults();
}

Cell::Cell() :
  type(Symbol)
  , env(instance<Environment>())
{

}

Cell::Cell(cell_type type):
  type(type)
  , env(instance<Environment>())
{

}

Cell::Cell(cell_type type, std::string const&  val):
  type(type)
  , val(val)
  , env(nullptr)
{

}

Cell::Cell(proc_type proc):
  type(Proc)
  , proc(proc)
  , env(nullptr)
{

}

Environment::Environment():
  outer_(nullptr)
{

}

Environment::Environment(Environment* outer):
  outer_(outer)
{

}

Environment::Environment(cells const& parms, cells const& args, Environment* outer):
  outer_(outer)
{
  cellit a = args.begin();
  for (cellit p = parms.begin(); p != parms.end(); ++p)
      env_[p->val] = *a++;
}

map& Environment::find(std::string const& var)
{
    if (env_.find(var) != env_.end())
        return env_; // the symbol exists in this environment
    if (outer_)
        return outer_->find(var); // attempt to find the symbol in some "outer" env
    throw stdext::exception("Undefined Symbol {0}", var);
}

// return a reference to the cell associated with the given symbol 'var'
Cell& Environment::operator[] (std::string const& var)
{
    return env_[var];
}

////////////////////// built-in primitive procedures

Cell lisp::proc_add(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i) n += atol(i->val.c_str());
    return Cell(Number, str(n));
}

Cell lisp::proc_sub(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i) n -= atol(i->val.c_str());
    return Cell(Number, str(n));
}

Cell lisp::proc_mul(const cells & c)
{
    long n(1);
    for (cellit i = c.begin(); i != c.end(); ++i) n *= atol(i->val.c_str());
    return Cell(Number, str(n));
}

Cell lisp::proc_div(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i) n /= atol(i->val.c_str());
    return Cell(Number, str(n));
}

Cell lisp::proc_greater(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i)
        if (n <= atol(i->val.c_str()))
            return false_sym;
    return true_sym;
}

Cell lisp::proc_less(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i)
        if (n >= atol(i->val.c_str()))
            return false_sym;
    return true_sym;
}

Cell lisp::proc_less_equal(const cells & c)
{
    long n(atol(c[0].val.c_str()));
    for (cellit i = c.begin()+1; i != c.end(); ++i)
        if (n > atol(i->val.c_str()))
            return false_sym;
    return true_sym;
}

Cell lisp::proc_length(const cells & c) { return Cell(Number, str(c[0].list.size())); }
Cell lisp::proc_nullp(const cells & c)  { return c[0].list.empty() ? true_sym : false_sym; }
Cell lisp::proc_car(const cells & c)    { return c[0].list[0]; }

Cell lisp::proc_cdr(const cells & c)
{
    if (c[0].list.size() < 2)
        return nil;
    Cell result(c[0]);
    result.list.erase(result.list.begin());
    return result;
}

Cell lisp::proc_append(const cells & c)
{
    Cell result(List);
    result.list = c[0].list;
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i) result.list.push_back(*i);
    return result;
}

Cell lisp::proc_cons(const cells & c)
{
    Cell result(List);
    result.list.push_back(c[0]);
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i) result.list.push_back(*i);
    return result;
}

Cell lisp::proc_list(const cells & c)
{
    Cell result(List); result.list = c;
    return result;
}

// define the bare minimum set of primintives necessary to pass the unit tests
void lisp::add_globals(instance<Environment> env)
{
    (*env)["nil"] = nil;   (*env)["#f"] = false_sym;  (*env)["#t"] = true_sym;
    (*env)["append"] = Cell(&proc_append);   (*env)["car"]  = Cell(&proc_car);
    (*env)["cdr"]    = Cell(&proc_cdr);      (*env)["cons"] = Cell(&proc_cons);
    (*env)["length"] = Cell(&proc_length);   (*env)["list"] = Cell(&proc_list);
    (*env)["null?"]  = Cell(&proc_nullp);    (*env)["+"]    = Cell(&proc_add);
    (*env)["-"]      = Cell(&proc_sub);      (*env)["*"]    = Cell(&proc_mul);
    (*env)["/"]      = Cell(&proc_div);      (*env)[">"]    = Cell(&proc_greater);
    (*env)["<"]      = Cell(&proc_less);     (*env)["<="]   = Cell(&proc_less_equal);
}


////////////////////// eval

Cell lisp::eval(Cell x, instance<Environment> env)
{
    if (x.type == Symbol)
        return env->find(x.val)[x.val];
    if (x.type == Number)
        return x;
    if (x.list.empty())
        return nil;
    if (x.list[0].type == Symbol) {
        if (x.list[0].val == "quote")       // (quote exp)
            return x.list[1];
        if (x.list[0].val == "if")          // (if test conseq [alt])
            return eval(eval(x.list[1], env).val == "#f" ? (x.list.size() < 4 ? nil : x.list[3]) : x.list[2], env);
        if (x.list[0].val == "set!")        // (set! var exp)
            return env->find(x.list[1].val)[x.list[1].val] = eval(x.list[2], env);
        if (x.list[0].val == "define")      // (define var exp)
            return (*env)[x.list[1].val] = eval(x.list[2], env);
        if (x.list[0].val == "lambda") {    // (lambda (var*) exp)
            x.type = Lambda;
            // keep a reference to the environment that exists now (when the
            // lambda is being defined) because that's the outer environment
            // we'll need to use when the lambda is executed
            x.env = env;
            return x;
        }
        if (x.list[0].val == "begin") {     // (begin exp*)
            for (size_t i = 1; i < x.list.size() - 1; ++i)
                eval(x.list[i], env);
            return eval(x.list[x.list.size() - 1], env);
        }
    }
                                            // (proc exp*)
    Cell proc(eval(x.list[0], env));
    cells exps;
    for (Cell::iter exp = x.list.begin() + 1; exp != x.list.end(); ++exp)
        exps.push_back(eval(*exp, env));
    if (proc.type == Lambda) {
        // Create an environment for the execution of this lambda function
        // where the outer environment is the one that existed* at the time
        // the lambda was defined and the new inner associations are the
        // parameter names with the given arguments.
        // *Although the environmet existed at the time the lambda was defined
        // it wasn't necessarily complete - it may have subsequently had
        // more symbols defined in that environment.
        instance<Environment> nenv = instance<Environment>::make(proc.list[1].list, exps, proc.env.get());
        return eval(/*body*/proc.list[2], nenv.get());
    }
    else if (proc.type == Proc)
        return proc.proc(exps);

    throw stdext::exception("Not A Function");
}


////////////////////// parse, read and user interaction

// convert given string to list of tokens
std::list<std::string> lisp::tokenize(const std::string & str)
{
    std::list<std::string> tokens;
    const char * s = str.c_str();
    while (*s) {
        while (*s == ' ')
            ++s;
        if (*s == '(' || *s == ')')
            tokens.push_back(*s++ == '(' ? "(" : ")");
        else {
            const char * t = s;
            while (*t && *t != ' ' && *t != '(' && *t != ')')
                ++t;
            tokens.push_back(std::string(s, t));
            s = t;
        }
    }
    return tokens;
}

// numbers become Numbers; every other token is a Symbol
Cell lisp::atom(const std::string & token)
{
    if (isdig(token[0]) || (token[0] == '-' && isdig(token[1])))
        return Cell(Number, token);
    return Cell(Symbol, token);
}

// return the Lisp expression in the given tokens
Cell lisp::read_from(std::list<std::string> & tokens)
{
    const std::string token(tokens.front());
    tokens.pop_front();
    if (token == "(") {
        Cell c(List);
        while (tokens.front() != ")")
            c.list.push_back(read_from(tokens));
        tokens.pop_front();
        return c;
    }
    else
        return atom(token);
}

// return the Lisp expression represented by the given string
Cell lisp::read(const std::string & s)
{
    std::list<std::string> tokens(tokenize(s));
    return read_from(tokens);
}

// convert given cell to a Lisp-readable string
std::string lisp::to_string(const Cell & exp)
{
    if (exp.type == List) {
        std::string s("(");
        for (Cell::iter e = exp.list.begin(); e != exp.list.end(); ++e)
            s += to_string(*e) + ' ';
        if (s[s.size() - 1] == ' ')
            s.erase(s.size() - 1);
        return s + ')';
    }
    else if (exp.type == Lambda)
        return "<Lambda>";
    else if (exp.type == Proc)
        return "<Proc>";
    return exp.val;
}
