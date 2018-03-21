# Execution

Here we describe the ideal execution model of CULT for maximum power. For optimization purposes some of these structures might be reified into complete stack frames and static frames.

## Design

All executions allocate an execution object. Primarily this holds the stack, but also a reference to the root namespace of the system (e.g. type system, thread state, loaded modules).

### Interpreter

The interpreter is designed to execute the AST as given, with only the read time optimizations. It is designed as a fallback/bootstrap implementation. While code can extend and improve the compiler it *must* not do so without providing an interpreter implementation (no matter how slow it may be).

The high level design of the interpreter uses a loop to interpret the top of the call stack. The primary reason for this is to allow a heap based stack. However it has the side effect of allowing an infinite stack. Of course JITed optimized code and native code may eschew this. Effectively it looks likes this:

* Determine next function to call.
* Call the function.
* Interpret the results.



### Scopes

(Move this, this is a read time object/concept)

The `Scope` object describes a lexical environment.

1. Each scope holds a reference to it's parent.
2. All variables are default constructed when the scope is entered.
3. All variables are default destructed when the scope is exited.

Ideally scopes would be a single direction linked list. However as a consequence of reentrance, multiple scopes may reference the same parent.

If we can guarantee a scope contains no other scopes, as a consequence of **1**, we can reify that scope into a fixed size structure and push it onto the stack. This should be easy to guarantee, since macros can be expanded at compile time, and scopes are lexical.



# [In Progress]

#### Code about closures

```
(define foo
  (fn (a b c)                   ; new call frame with new scope (for a b c)
    (fn (x y)
      (+ a b c x y))))          ; has a reference to the call frame's scope
```

## 