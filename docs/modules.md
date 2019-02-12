# Modules

All code lives in a module. The current module is always the top-most lexical scope (though it may query other modules). Modules can import other modules. Doing so runs their initialization routine.

The author hopes that modules may eventually be immutable, this has some difficulties. Notably in adding multimethods and types and of course in the general problem of state. For now, be aware that requiring a module has side effects in the current process, and that modules store significant amounts of state (for the dispatch system, type system, and the application logic). Additionally, the primary purpose of having immutable modules, is so that the system can reload portions of the modules and only update the relevant parts, for now be aware that the system does this to the module objects in a mutable way.

Modules have strong names using a URI scheme. Custom handlers can be added to the URI scheme (including, for example, module systems and package managers). Below are some examples:

* `"builtin:system.cult"` Would resolve to the system's cult implementation.
* `"file:D:\\Data\\hello.cult"` Would resolve to an absolute file.
* `"file:hello.cult"` Would resolve to a file local to the current file.
* `"file:./test/hello.cult"` Would resolve to a file relative to the current file's directory.
* `"packman:Organization/Package >15.1.3"` Would use a package manager to resolve to a specific package, with optional version matcher.
  * Optionally the loader is wrapped in a multimethod so it can be invoked with an arbitrary number of arguments of arbitrary type. For example the above devloves to:
    ``` (cult/load "packman" "Organization/Package" (packman/Match :greater-than v:15.1.3))
    ```
* `"namespace:foo/bar"` Would resolve to the synthetic module containing all the symbols in the given namespace.

## Lifecycle

When a new module is requested it will be returned by the namespace. If the module already exists it may have already been loaded. Modules have the following important steps to their state:

* Constructed
  * The module has a URI.
  * The module does not yet have content.
* Loaded
  * The module has a URI.
  * The module has the required content.
* Initialized
  * The module has a URI.
  * The module has the required content.
  * The module has been initialized into the namespace.
  * The module is ready to be used (and interpreted)

Other things may add more states (like a backend with multiple steps to the compilation of a module), these are the portable guaranteed ones.
