A **module** is a collection of code. A module is also a kind of scope. Modules are generally tied to a single source (like a file, or database), though a source may have many modules. A module can require other modules. Modules are strongly named. Modules can be imported multiple times; they can be imported with different parameters or imported to replace their old version. Modules don't have to interact with the namespace system.

A **namespace** is a collection of scopes. A namespace is itself a kind of scope. A namespace can apply rules to the scopes it collects to curate the names. A namespace keeps track of the scopes it uses (so that it can update itself). Namespaces are weakly named.

### Example: Core

The `CULT` namespace is a built in namespace representing all of the cult system features. It is built by the cult system's init module.

The `USER` namespace is a built in default namespace representing what the user wants to be in by default. It is built by the user profile's init module. This can still be accessed from other namespaces, so a user can inject arbitrary stuff in here.

Generally projects, like libraries and executable, will create their own namespace and init module (primarily through tools). A namespace can be packaged into a module that builds it. This allows modules to require that namespace module, so they can be packed as "namespaceless".

### Init

On init of the language, init modules are searched for and ran. The purpose of these are to build the initial namespaces, and configure the resolution features for the modules.

### Multimethods

Multimethods are effectively rebuilt (or at least give that appearance) in each namespace, this means a module can add implementations that are private to itself.

### Relationships

#### Import

A namespace can import from another scope (namespace or module). As part of the import it can do a lot of things with the incoming names:

* Filter them
* Path them (e.g. "foo/bar/baz")

#### Require

A module can require another module.

#### Use

### Example: Plugins

A plugin, a dynamically loaded module, will likely capture a parent namespace, and modify it with it's own features.