Here we describe the core semantics of the cult language.

# Special Forms

Here we describe the special forms.

## `define`

This primitive binds a value to a name in the current scope. The default requirement is simply that the name is bound and will be resolved as part of the scope. The mechanics of the definition depends on the scope.

### Notes

With simple analysis the compiler can improve the performance of things around define by exploiting the mechanics of the interactions of scopes and the things being bound. For example, in a `do` scope a `define` creates a name for a value that will be accessible within that `do` block; if that value is analyzed to be a `Variable` then a compiler might push it along with all the other variables in a single stack frame.

## `do`

Creates a new scope, executes it's sub-forms in sequence.

## `quote`

Returns the **unread** contents of it's subforms.

## `cond`

Pairs of conditions and branches, with an optional else branch. Executes conditions in sequence until one is truthy, and then executes that branch, returning it's results.

## `while`

A condition and an implicit `do` block. Executes the condition, if it is truthy, executes the block, then starts over.

## `fn`

Creates an anonymous lambda. A scope that takes arguments. An object that is an executable.

The first form is a binding form, the rest is an implicit do block. There are two scopes in a function

### Notes

Functions can be *internally* named by defines. If the scope is eventually a module, then the name is just as defined. If the scope is eventually a function then the name should perpend that function's name. Actually resolved anonymous functions should use a line number, or at least a sequence number, for their own name. Functions must always know the module they are from.

Functions might also acquire their name through macros and the structures they are added to. For example, by default, a function for a multi-method would typically be an anonymous method-level function (e.g. `(attach ~multimethod (fn ...))`)

# Implementation

Here we describe the semantic tree of objects.

