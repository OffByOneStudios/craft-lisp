Mix lispier julia with more c++.

* Allow C++ memory management idioms.
  * RAII
  * Copy/Move construct and assign
  * Optional reference counting
  * Optional garbage collecting
* Power first.
  * Build features focused on power first.
  * Then work on optimizing them.
* A dialog with the compiler.
  * Julia's introspection of parse tree states
  * Julia's introspection of llvm and assembly
  * Overriding special compiler multi-methods (e.g. construct and assign) to provide behaviour (garbage collect, RAII).
  * Overriding compiler optimization and code-gen multi-methods.
* Multi methods.
  * Maintain dependency graph to allow generation of
* Optional typing.
  * Improved compiled performance when types are known (like julia).

## Syntax

Eventually we plan to have multiple syntax transformations. For now the semantics of the language are available through a lisp style AST.

* `()` An AST expression of some kind.
* `symbol` Must start with a non-numeric character (e.g. punctuation is fine).
* `:keyword` A keyword of somekind (e.g. rather than a symbol).
* `"String"` A string literal.
* `0.34LL` A number literal, text at the end determines representation.
* `;` For comments. `;()` to comment a form.

### Ideas

Simple syntax sugar ideas for the lisp:

* Allow some or all of `<>` `{}` `[]` for meaning various things. Maybe even just allow them interchangeably.
  * `<>` are both used for things like `< :less-than`, `> :greater-than`, `-> :thread-forms` and others.
  * Variants of `{}` and `[]` could be useful for implicit data structures. Although the usefulness at this layer is questionable.
  * Some motivations include:
    * Type expressions.
* `,` to group forms in forms. For example the following would be equivalent:
   ```
   '((:key "value") (:foo 5))
   '(:key "value", :foo 5)
   ```
  * Some motivations include:
    * Argument binding forms.
    * Cond.
    * Dictionary pairs (and the ramifications on various macros).

### Conventions

* `UpperCase` for types.
* `dashed-names` for functions and macros.
* `use/for-pathing` e.g. for a family of functions or variants.

## Defines

Generally speaking defines map a data structure into a name. For this usage is the general purpose extensible `def` macro. It provides the primary dispatch point for compile time variables. It uses a keyword macro pattern.

* Function Definition Form
  ```
  (def/fn name
    (TypeArgs)
    (args) ReturnType
    "doc"
    (body...))
  ```
  * `name` is the symbol to bind to. By default all functions can be turned into multi-methods via a merge strategy provided by most function objects; hence `name` can name an existing function to merge with.
  * `:explicit form` keyword arguments may be placed after the first, and before the first body form.
  * `(TypeArgs)` is the type argument binding form, it must resolve to a Type containing free variables, must be the found before `(args)`. The typevars will be bound in any following form.
  * `(args)` is the argument binding form, it is destructed in many placed in to many keywords, of the form:
    ```
    (symbol Type (default-form-or "literal"), )
    ```
  * `ReturnType` must be the form immediately after the `(args)` form, which it assists in building the `:type` keyword.
  * `"doc"` is the only string found, and is placed in `:doc`.
  * `(body...)` from the first non-type form onwards is assumed to be the body in an explicit `(block)` form (itself an extensible keyword macro) which is placed in `:code`.
* MultiMethod Definition Form
  ```
  (def/multi name
    "doc")
  ```
  A helper form to place documentation on a multi-method. Might also be used for other disambiguation, type-enforcement, and documentation purposes.

### Aside: Extensible Keyword Macro Pattern

In general of the form:

```
(ex-macro (shared-forms)
  :foo "foo"
  :bar (bar 2)
  :baz baz
)
```

Which often builds into a dictionary (or list). The keywords can come from any library in the current namespace, and can be given in short form or specific form (e.g. `:foo` or `:library/foo`).

Often, pre-templated macros will exist, for example:

```
(ex-macro/fbb (shared-forms)
  "foo"
  (bar 2)
  baz
)
```

### Namespaces:

Namespaces are simply a way to control the path to a name. They do not provide modular features. Modules will have many namespaces, and may enter an existing namespace to modify it. Namespaces are named differently for each module (e.g. `(A foo::bar)` (module A's namespace bar in namespace foo) could actually be `(B bar)`).

### Types:

Generally speaking a type is a struct composed of names for types that compose it.

There is no inheritance, but there is composition, and support for automatically forwarding certain types (effectively providing multiple inheritance). No functions are declared inside a type.

```
(def-type NewType
  (slots
    (variable_name Type ...<specifiers>)
    (...)
  )
  ...(<other sections>)
)
```

```
type NewType
{
    variable_name Type ...<specifiers>;
    
    
}
```

#### Special Functions:

There are a number of special multi-methods which can be overriden to provide types with special behaviour. And macros can provide groups of these automatically.

`core::construct(instantiated NewType, ...args)`
`core::assign(existing NewType, assigning NewType)`

