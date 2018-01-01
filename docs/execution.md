# Execution

Here we describe the ideal execution model of CULT for maximum power. For optimization purposes some of these structures might be reified into complete stack frames and static frames.

## Features

* Dynamical Bound Variables
* Lexically Bound Variables
* Scopes
* Dynamic Call Frames
* Reentrant Call Frames

```
(method foo (a b c)               ; new call frame with new scope (for a b c)
  (lambda (x y) (+ a b c x y)))   ; has a reference to the call frame's scope
```

## Design

The high level design of the interpreter uses a loop to interpret the top of the call stack. The primary reason for this is to allow a heap based stack. However it has the side effect of allowing an infinite stack. Of course JITed optimized code and native code may eschew this. Effectively it looks likes this:

* Determine next function to call.
* Call the function.
* Interpret the results.

Everything that can be reached by an execution thread can be reached through the stack. At the root of the stack is the initial environment, interpreter, and type system. Some of these are mutable, some are not. Ideally the type system would be immutable...

### Scopes

The `Scope` object describes a lexical environment.

1. Each scope holds a reference to it's parent.
2. All variables are default constructed when the scope is entered.
3. All variables are default destructed when the scope is exited.

Ideally scopes would be a single direction linked list. However as a consequence of reentrance, multiple scopes may reference the same parent.

If we can guarantee a scope contains no other scopes, as a consequence of **1**, we can reify that scope into a fixed size structure and push it onto the stack. This should be easy to guarantee, since macros can be expanded at compile time, and scopes are lexical.

### Frames

The `Frame` objects describes a dynamic(? ; or do the really just describe execution) environment; often created via executing a function.