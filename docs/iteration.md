Iteration is mostly based off of C#'s LINQ, but uses multi-methods in a julia style for optimization, and uses clojure style threading for readability.

## Iteration

The iteration protocol roughly follows C#.

Has the following abstract hierarchy:

* `(Iterable I)`
  * `(Iterator I)`

All `Iterator`s are `Iterable`s. An `Iterator` stores the current iteration state and can be use to iterator through a collection. An `Iterable` can create an `Iterator`.

* [Julia](https://docs.julialang.org/en/latest/manual/interfaces/#man-interface-iteration-1)

### Iterable

Must have:

* `iterate (container Iterable,) Iterator`.

### Iterator

Must have

* `current (iterator Iterator,) Iterator`.
* `next (iterator Iterator,) Bool`.

## Methods

### `map`

Rejected Alternate Names: `select`, `transform`.

Builds an iterable by transforming each element of the source element.

`map (source (Iterable I), transform (Func (I) R)) (Iterable R)`

### `fan`

Rejected Alternate Names: `spread`, `furl`.

Builds an iterable by transforming each element of the source element and concatenating them.

`fan (source (Iterable I), expander (Func (I) (Iter R))) (Iterable R)`

### `filter`

Rejected Alternate Names: `where`, `cond` (overload), `if`, `sift`.

Builds an iterable by filtering elements of another iterable that are fulfill a predicate.

`filter (source (Iterable I), predicate (Func (I) Boolean)) (Iterable I)`

Note: Would prefer to have a filter-not that inverts the logic. Would need a better name to pair it with, like `sift`/`block` or something.

### `fold`

Rejected Alternate Names: `furl`, `reduce`, `aggregate`

Builds a single element from an iterable by folding them into an initial piece of state (or the first element if no state is given).

This function behaves as a `foldl`, that is the reducer first calculates the state and first value, and then the state and second value. In a loop structure, rather than a recursive structure.

 * `fold (source (Iterable I), reducer (Func (I I) I)) I`
   If the source is empty, an error occurs.
 * `fold (source (Iterable I), state S, reducer (Func (S I) S))`
   If the source is empty, the original state is returned.
