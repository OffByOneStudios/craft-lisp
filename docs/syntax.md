# Syntax

## Design

Eventually we plan to have multiple syntax transformations. For now the semantics of the language are available through a flexible lisp syntax with a "batteries included" configuration. The point of which is to provide a minimalist yet powerful syntax to map to the semantic tree of the system.

### Specification

#### Comments

The comment character is the `;`. Everything after it on a given line is ignored. Entire forms can be commented by placing the comment directly infront of the start of the node. For example `;(` or `;foo:[` in which case the entire form will be commented out (as long as the forms match.

Commented forms can allow for out of line documentation, however it is not the recommended use case. For example `;doc:(` could be used. However it would not provide a documentations string that is queryable, and so is not recommended.

#### Readers

This is the primary power user feature. Most of the following features can be prefaced with an `reader:` to change their reader behavior. Readers are entirely part of the lisp syntax. They can be manipulated by using special forms specific to the lisp syntax (which would not show up in the final semantics).

The lisp syntax uses a custom namespace and a stack of reader bindings as a scope. This allows it to change the evaluator behaviour as it reads. Readers are orthogonal to macros. Macros are semantic objects of the system which are represented in authoritative trees, readers are specific to the lisp syntax and are only represented in the AST.

This distinction is important. Readers are meant for helping with syntactic suger *only*. Other syntax projections will not be able to see readers, they will be projecting to their prefered syntax representation. Hence readers must be very straight forward in what they represent. Prefer macros in all cases where possible.

#### Forms

The following pairings create Forms: `()`, `[]`, `{}`, and `<>`. Forms are effectively nodes in the semantic tree (or some sort of complex value representation). All forms share the same reader namelayer (though it is distinct from atom readers).

The default reader for each of these differs.

* `()` generally has the `expr` reader by default (the top level may be a little special). This is a lisp expression. Generally speaking this will map the form to a function, macro, or special-form which will then be turned into the semantic tree. Functions by being turned into a `CallSite` if proper. Macros and special forms being evaluated by the reader itself.
* `[]` has the `vec` reader by default. This is a sequence of expressions which will be placed into a vector value.
* `{}` has the `dict` reader by default. This is a sequence of key-value expression pairs that will be placed into a map value.
* `<>` has the `type` reader by default. `type` provides a way to describe complex type descriptions. Recursive.

Also avaliable are:

* ` ` (empty) which is the quoting syntax. Recursive and replaces symbol readers with keyword readers.
* `set` which is a sequence of values to place in a set.
* `inst` which is an instance Initializer like so: `inst:{FooBar baz bang :boo 5}
* `ast` provides a more direct syntax for generating syntax nodes. Recursive.

#### Atoms

Atoms are a sequence of characters that represent a value, or sometimes symbol, in an expression. Atoms can have explicitly defined readers using the same reader syntax as above.

Symbols represent a named reference to some other part of the system and must start with a non-numeric alphanumeric character. One variety of symbols are the operator symbols (which contain no alphanumeric characters).

Strings can be formed using pairs of `"` and `'` characters, strings can contain arbitrary characters (with some escaping).

Then there are a variety of other value formats for numbers (integer, floating point, dates).

The default reader tries a specific sequence of readers for each atom it is given until one of them matches:

* `symbol`
- `:keyword` A keyword of somekind (e.g. rather than a symbol). This uses the empty parser.
- `"string"` or `'string'` A string literal.
- `0.34LL` A number literal, text at the end determines representation.

### Ideas

Simple syntax sugar ideas for the lisp:

- Allow some or all of `<>` `{}` `[]` for meaning various things. Maybe even just allow them interchangeably.

  - `<>` are both used for things like `< :less-than`, `> :greater-than`, `-> :thread-forms` and others.
  - Variants of `{}` and `[]` could be useful for implicit data structures. Although the usefulness at this layer is questionable.
  - Some motivations include:
    - Type expressions.

- Allow some or all of `/` `@` `:` `.` as pathing operators... or maybe allow for arbitrary pathing operators. Limitation on usage, there must be an alpha character before each one.

  * Example of alpha limitation: `/` is the symbol `/`, `foo/bar` is the symbol sequence `(/ foo bar)` and `\|/` is the symbol `\|/`. So `foo:/bar` is `(: foo /bar)` and `foo/:bar` is `(/ foo :bar)` (bar is a keyword here).

- `,` to group forms in forms. For example the following would be equivalent:

  ```
    '((:key "value") (:foo 5))
    '(:key "value", :foo 5)
  ```

  - `.` to join a form. For example the following would be equivelent:

    ```
    '((:key "value") (:foo 5) testing 18)
    '(:key "value", :foo 5 . testing 18)
    ```

- Some motivations include:

  - Argument binding forms.
  - Cond.
  - Dictionary pairs (and the ramifications on various macros).

- Macro helpers:

  - `:()` for `quote`d code.
    - [-] Hard to express symbols and other literals in a quoted way.
  - `'()` for `quote`d code.
  - ``()` for `quasiquote`d code.
    - [-] Do we actually need a separate quasiquote?
  - `~` for splicing.
    - [-] Wanted to use this for matching (e.g. `~=` but `?=` might work as well)
  - `,` for splicing.
    - [-] Wanted to use this for reducing parens.

### Conventions

- `UpperCase` for types.
- `dashed-names` for functions and macros.
- `use/for-pathing` this is meant as a refinement system. Though it can also be used for namespacing.
- Avoid using `.` even though they are not currently reserved.

### Aside: Extensible Keyword Macro Pattern

This is a way for macros to extend their "syntax", in general of the form:

```
(ex-macro (shared-forms)
  :foo "foo"
  :bar (bar 2)
  :baz baz
)
```

Which often builds into a dictionary (or list). The keywords can come from any library in the current namespace, and can be given in short form or specific form (e.g. `:foo` or `:module/foo`).

## Implementation

### CultLispSyntax

The syntax object that provides `PParse`.

### Sexpr

The core data structure of cult syntax. An S-Expression form.

### Symbol

An object representing a symbol in the final system. Can be interned.

### Keyword

An object representing a keyword (an unbound symbol) in the final system. Can be interned.
