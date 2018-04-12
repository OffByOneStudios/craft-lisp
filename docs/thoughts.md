## Explicit Set

No good way to use `=` as the set symbol. Use an explicit `set`, with `get`s being implicit in many places (e.g. like function calls).

## Match Equality

The function `?=` is similar to `!=` and `<=` in that it provides a complex match (e.g. between a regex and a string).

## No Tagged Names

Don't use `!` or `?` as suffixes for names, leave that to the syntax layer.

## No Keyword Functions

`(:foo thing)` will interfere with the keyword extension features I have planned.

## Compiler Optimization in Multimethods

For a function signature (e.g. `+`) `((a Int64) (b Int64) Int64)` the compiler function could be listed beside it as say `((state CodeGenState) (a (CodeGenVariable Int64)) (b (CodeGenVariable Int64)) (CodeGenVariable Int64))`

## Function Signatures

(foo bar baz)
((foo Int) bar (baz (Union String Int)))
(foo Int, bar, baz (Union String Int))
TODO:
* optional
  ? ((foo Int) (bar Int 8))
  ? ((foo Int) (bar Int :optional))
* keyword arugments
  ? ((foo Int) (:bar Int) (:baz 8))
  ? ((foo Int) (:keyword bar Int) (:keyword baz 8))
* return type
  ? ((foo Int) (bar Int 8) (:baz 8) Int)
  ? ((foo Int) (bar Int 8) (:baz 8) (:returns Int))
* collections
  ? ((foo Int) (args :args))
  ? ((foo Int) (args :args) (kwargs :keywords))

## Data Structure Library

Powerful library for mixing in various data structure features and choosing the best implementation of those availble.

### Evented

A way to easily hook an object up to a data structure and get events when certain things happen. 

Also, have a way to *replay* the events (removing add/remove pairs so only the adds get replayed) so that an object can be hooked up correctly automatically.

Also, have a way to batch and time updates (e.g. so 200 adds in a row will be batched to a single (or couple) add-collection events... perhaps on a timer or with hints (e.g. to support UI)).