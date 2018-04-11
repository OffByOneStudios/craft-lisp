Here we describe the core semantics of the cult language.

# Cult Semantics

The below are all part of `cult.system`.

## Special Forms

Here we describe the special forms.

### `define`

This primitive binds a value to a name in the current scope. The default requirement is simply that the name is bound and will be resolved as part of the scope. The mechanics of the definition depends on the scope.

#### Notes

* `SBindable`: With simple analysis the compiler can improve the performance of things around define by exploiting the mechanics of the interactions of scopes and the things being bound. For example, in a `do` scope a `define` creates a name for a value that will be accessible within that `do` block; if that value is analyzed to be a `Variable` then a compiler might push it along with all the other variables in a single stack frame.
* The name of a special form must be known at read time (e.g. can only be computed by macros) for the time being.

### `proclaim`

A form for talking to the semantics/compiler/etc.

### `do`

Executes it's sub-forms in sequence. Creates a scope for variables, and provides RAII gaurntees.

#### Notes

Using `proclaim` can prevent the creation of a scope, and instead provide just the sequence semantics.

### `quote`

Returns the **unread** contents of it's subforms.

### `cond`

Pairs of conditions and branches, with an optional else branch. Executes conditions in sequence until one is truthy, and then executes that branch, returning it's results.

### `while`

A condition and an implicit `do` block. Executes the condition, if it is truthy, executes the block, then starts over.

### `variable`

Creates a slot requirement. Can be placed in a define to represent a mutable slot. Can be followed by optional Type and other information (state hints).

### `function`

Creates a function. A scope that takes arguments. An object that is an executable.

The first form is a binding form, the rest is an implicit do block. There are two scopes in a function (the argument scope, and the execution scope).

#### Notes

Functions can be *internally* named by defines. If the scope is eventually a module, then the name is just as defined. If the scope is eventually a function then the name should perpend that function's name. Actually resolved anonymous functions should use a line number, or at least a sequence number, for their own name. Functions must always know the module they are from.

Functions might also acquire their name through macros and the structures they are added to. For example, by default, a function for a multi-method would typically be an anonymous method-level function (e.g. `(attach ~multimethod (function ...))`)

### `type`



## Concepts

### Helper Macros

#### `var`

`(var <name> [<value>] [<Type>])

Uses `variable`.

Roughly equal to:

`(define <name> (variable <value> <Type>))`

#### `fn`

`(fn [<name>] [<...>])

Uses `function`.

(If the name is provided) roughly equal to:

`(define <name> (fn [<...>]))`

#### `struct`

Uses `type`

#### `feature`

Uses `type`

### Eqaulity / Match

Equality comes in three varities. Strict reference equality, logical equality, and fuzzy equality.

* Strict reference equality is only true if two instances are the same object.
* Logical equality is true if two instances are logically equal according to the user. It should be associative (e.g. A equals B should be the same as B equals A).
* Fuzzy equality returns a score of how much an object equals an ideal object.

#### `is`

System function checks for reference equality between two instances.

#### `equal`

**Overrideable** System method checks for logical equality between two instances.

#### `match`

**Overrideable**  System method checks for fuzzy equality between two instances. Can return anything which can be coerced to a numerical score or coerced to a true/false (the return type might, for instance, assist in binding the case statement variables).

#### `case`

```
(case value
  [([label] match-form case-branch)]*)
```

System macro for performing switch/case statements. `label` is an optional keyword. In the case macro `break`

### Methods

The primary way to wrap up any sort of dispatching functionality is with a method (which are full multi-methods).

### Type Graph



#### `is-a`

`(is-a value type)`

System function checks for the `is-a` relationship in the type graph.

#### `has-a`

`(has-a value type)`

System function checks for the `has-a` relationship in the type graph. Note that this can be used to lookup multi-methods / multi-features by providing a tuple of values. For example:

```
(has-a (tuple foo bar baz) some-multi-method)
(has-a (tuple foo bar) SomeMultiAspect)
```

### Coercing / Casting

Casting is the operation of taking an instance of a type and turning it into an instance of a different type but of *the same unique object*. This primarily happens between any two types that respond true to the `is` or `has` concept. This operation (ignoring some orthogonal issues like reference counting) should be bidirectional, that is to say casting from A to B and then back to A should return the original instance.

Coercing is the operation of taking an instance of a type and turning it into an instance of a different type that is (probably) a different object. The types don't need to share any type relationships. This operation can create new objects, and does not have to be bidirectional, though it should avoid destroying or corrupting the original object.

Casting is a complete subset of coercian. Below are the places where these transformations are perfommed automatically:

* **Coercian** Assignment (e.g. `set`) performs coercian from the value, to the destination type of the slot (if it has any).
* **Casting** Function call arguments are only cast to the destination types (this simplifies dispatch).
* **Casting** Constructors as functions also only use casing (this simplifies dispatch).
* **Coercian** Initializers perform coercian (as the destination type is known).

#### `as`

`(as value type)`

System macro performs coercion/casting of a value to a specific type. Will automatically fall back to casting (and can optimize *to* casting if necessary). Prefer this function.

#### `cast`

`(cast value type)`

System function performs casting of a value to a specific type.

#### `coerce`

`(coerce value type)`

System method performs coercion of a value to a specific type, will automatically perform casting if necessary).

## Implementation

Here we describe the semantic tree of objects.

### CultSemantics

The root object of a semantic tree. Provides a module of semantics. Can **append** (for live modules evaluating new forms in sequence) or **merge** (for live editors re-evaluating code) another module of semantics. Is a scope.