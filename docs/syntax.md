# Syntax

## Design

Eventually we plan to have multiple syntax transformations. For now the semantics of the language are available through a lisp style AST.

- `()` An AST expression of some kind.
- `symbol` Must start with a non-numeric character (e.g. punctuation is fine).
- `:keyword` A keyword of somekind (e.g. rather than a symbol).
- `"String"` A string literal.
- `0.34LL` A number literal, text at the end determines representation.
- `;` For comments. `;()` to comment a form.

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
- `use/for-pathing` this is primarily meant to be used as a module prefix. Though this isn't necessary. And the names are hence local to each module.
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