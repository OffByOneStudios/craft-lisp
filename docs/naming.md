How do we resolves names (mostly symbols) into objects?

## Problem

When we reference a name, a string of symbols, how is that turned into an object. Most of the properties we want:

1. Short names. We should not have to be extremely specific in referencing something
   a. A corollary is we want the fewest possible s-exprs.
2. Object oriented names (for accessing structure variables for example).
3. The package manager/require system should have the power to relocate modules (regardless of what they claim to want).
4. We should be able to reference symbols specific to the module they came from.
5. We should be able to reference symbols globally.
6. We should be able to "override" (often layer in the case of multimethods and implementations) the names of symbols in other locations.
   a. Including features to improve the usage of this.
7. Writing a set of files with similar name settings should be easy.
8. We should be able to reflect on how/where objects got their names.



## Solution

### Externally Accessible Scopes

Allow using an operator to access a symbol's object as a scope (in the lisp, and probably most languages, this would be the `.`). This operator would be part of the `Resolve` semantic node.

This becomes complicated when foo's type must be looked up dynamically at runtime, but not overly so. The nodes that will be externally accessible scopes include: `Semantic`s (as modules), `Multimethod`s (for specializations), `Type`s (for members); where as likely non-accessible scopes include: `Function`s, `Closure`s, and `Block`s. In addition a special `Namespace` object for global namespaces.

This will allow for object oriented syntax (2.) while allowing for short "names" by reducing use of parens (1a.) to access named slots. This syntax will also be used to provide module and global symbols (4., 5.).

#### Examples

```lisp
(+ foo.bar 2) ; access the bar symbol in the foo symbol
```

### Global Namespaces

Provide a way to reference global symbols. Regardless of where they were defined, as long as they were required at some point. This is primarily used by the name mangling features, and for being specific about which multimethod to overload, and should be avoided otherwise.

Namespaces can also be used as synthetic modules for the purposes of import (requiring them won't do anything).

#### Examples

The primary way to manipulate this will be through a `ns/using` directive:

```lisp
(ns/using com.my-domain.foo)
```

A way to reference symbols globally regardless of namespace manipulation should probably be included with the syntax:

```lisp
(+ foo ns:some.ns.foo)
```

Some package managers/systems may prefer to provide a Java style namespace solution:

```lisp
(+ foo ns:com.my-domain.foo)
```

### Define Keyword Attributes

We could use complicated defines to give us control over how names were set in the first place. This would allow us to enable many features:

* Modules could have public/private symbols.
* A single define can set multiple symbols.
* A define can override symbols.
* Defines represent a point of manipulation by the system.

This primarily makes it easy to define a symbol in multiple places for use with local and global symbols (4., 5.) as well as overloading features (6a.) without unnecessary forms (1a.). They also provide a point of resolution for reflection (8.).

#### Examples

```lisp
(define
    :local local-name ; by default specifics a local name
    :global my-ns.example-global ; the full global name
    :overload-local other-local-name ; if this define overloads, use this as the overload local name
    :private ; Means it's not exported from local scope
    :public ; Means it is exported from local scope
    (some thing 5))
```

Macros could help with this:

```lisp
(def-method cult.foo ; global name of multimethod
	core-overload-for-TheType ; optional overload name 
	[arg-a arg-b]
	body...)
; expands to:
(define
	:global cult.foo
	:overload-local core-overload-for-TheType
	(function (arg-a arg-b)
		body...)
)
```

### Environment Manipulation

A set of utilities for managing the environment's behavior with respect to certain operations (like require). They are:

* `ns` A special form / macro which can be used as a header to do all of the below.
* `ns/namespace` A `NamespaceManipulation` node to set the default namespace of of defines in the current module
* `ns/require` A `NamespaceManipulation` node to require a module be loaded. This does not effect the symbol resolution for the current module. This means the required module does not have to actually be loaded for this module to be loaded.
* `ns/import ` A `NamespaceManipulation` node to import a module into the current one. This requires the module. It also, by default, places the symbols in the imported module into the current one (but with private marked on all of them). This has additional arguments to place the symbols in a different name (e.g. `module/symbol`) or to place the module in a symbol (e.g. `module.symbol`). This has additional arguments to white list or black list symbols. This has additional arguments to manage the visibility of the imported symbols (e.g. to forward them through this module).
* `ns/using` A `NamespaceManipulation` node which adds the given module or namespace to the resolution order for the current module. This has additional arguments to place the symbols in a different name (e.g. `module/symbol`).
* `ns/include` A `NamespaceManipulation` node which includes another module as if it's a direct part of the current one. It is a warning (unless overridden with an additional argument) for the included module to have `ns` family features. This always causes the module to be reimported as it's effectively attempting to pretend the source code was inserted directly.
* `ns/load` A `NamespaceManipulation` node which calls for the runtime inclusion of another module. Unlike other `NamespaceManipulation` operations, this one causes side effects at runtime of the module's root level (notably invoking load machinery) where as the other nodes don't. This node can only place the module in a symbol (e.g. for `module.symbol` style resolution).

The above should be instrumented through the `CultSemantics` object providing a resolution system, post bind step, which looks up the symbol it is given.

The default environment is effectively:

```
(ns/require "builtin:types")
(ns/require "builtin:lisp")

(ns/using "namespace:cult.system")
(ns/using "namespace:cult.core" :as cult)
```

This will allow us to provide for package managers (3.) and sets of files (7.) while improving resolution of global manipulation of short names (1., 5.).

#### A Note on `/`

The slash `/` symbol is not an operator/special-syntax like `.` is. It is a convention as a way to provide a "path" within a namespace - sometimes called a refinement - and has no special parsing rules associated with it (outside of tooling relying on convention and default arguments to some methods).

This is often used by modules to place symbols they imported into their own area for the purposes of development. Cult in specific uses this to separate concepts, even though they are all part of the same namespace, package, and final export list, they are from different modules. This is especially important for cult as it is often automatically `ns/using` and may use some common names which are better prefixed with their purpose (e.g. incase they are in the problem domain). For example while `+` may be safely assumed to be the mathematical operation by default, assumming that `compile` always means "compile a cult/current-runtime system module" is more of a stretch and is hence named with `cult\compile` by default.

Notably, if something which provides a `/` name changes, the module will have to be marked to be rebuilt/imported (unless it resulted in no significant changes). This is because the root level of the object is ran at load time and is allowed to introspect the state of the system, additionally macros may have been changed (or the multi-methods they rely on). However if something that provides a `.` name changes it will not have to reimport the module (though things like a Jit may have made assumptions based off of the state *at the time they were invoked* (e.g. post-load) and may need to update their function cache).

### Semantic Objects

Objects in the system, especially those at the root level of `CultSemantics` (like multimethods and types), should be thought of as semantic objects. For example that a multimethod defined in `ns:cult.system` by the name of `+` represents the concept of addition as defined by cult, where as a multimethod defined in `ns:com.foobar.maze` by the name of `+` could mean something completely different as defined by that namespace.

This distinction has some consequences. For example when defining a new object `+` in a module: whether we want to overload (e.g. extend) `+`, resolve to a different object for `+` (e.g. create a new name / shadow the name), or perhaps merge two distinct definitions into one.
