Mix lispier julia with more c++.

* Power First
  * Build capabilities first, then work on optimizing them (by using those capabilities)
  * Feature/Type system:
    * Provides an in-memory store of the capabilities of the process.
    * Provides hooks into existing languages
      * C++
    * Powerful feature system
      * Stores graph of code.
      * Multi-methods that can satisfy portions of the relationships.
      * Feature "edges" that can relate many types to a single implementation.
      * Multi-methods are single dispatch virtual methods, and can be called that way.
* A dialog with the compiler.
  * Replace any stage of the language (Parsing, Reading, Compiling, Execution)
    * Multiple-dispatch allows for N-to-N couplings.
  * Introspect any stage (parsing, read-stages, compile-stages).
* Optional "metacode"
  * Types:
    * Options
      * Concrete types (support the entirety of C struct behaviors)
      * Abstract tags.
      * Complex C++ inheritance and structuring.
      * Externally hosted type systems.
    * Benefits:
      * Improved performance when types are known (like julia).
      * Improved safety using type assertions.
      * Improved power in dispatch when using things like diagonal/triangular/arbitrary type expressions.
  * Memory:
    * Options:
      * C++: RAII; Copy/Move construct and assign
      * Rust: Ownership / Borrow
      * Optional-preferred reference counting
      * Optional garbage collecting
    * Benefits:
      * Improved performance when known.
      * Improved safety using assertions.
      * Improved power in dispatch when.
  * Effects:
    * Options:
      * [Koka](https://koka-lang.github.io/koka/doc/kokaspec.html)
    * Benefits:
      * Improved performance when known.
      * Improved safety using assertions.
      * Improved functional analysis.

### Limitations

Here we list some explicit limitations we have no plan to work around:

* Module top levels are special:
  At the moment some things can only be placed in the top-level of modules: macros, methods, types, module-variables, and dependencies (and other scope modifying calls).
  They can't be executed dynamically in a function scope at runtime. We provide sufficient workarounds for this at the moment, between letting top levels be fully executable (able to require code and run it at the top level) and the arbitrary code creating macros, there should be no effective limitation here.

# Implementation Design

This project is an implementation of this lisp. Here we will describe the basic design.

## Objects

These are objects that are part of the system as a whole.

### Environment

This is the root class for managing the host of the interpreter, it manages a list of modules.

### Module

Part of the environment's interface, this represents a collection of code in the system.

### Execution

Part of the environment's interface, this represents an execution. Provides a thread local interface to get the current execution. Environments and Executors must cooperate to ensure this is always correct.

## Features

These are the interfaces that the system is designed around.

### PSyntax

A syntax specific data structure provider. Provides a `parse` function for taking a string and returning the syntax specific data structure this provider is typed on. The parse function takes the module being parsed (for accessing parse time extensions) as well as an options structure (reading in source locations, keeping the string for source lookup, etc.)

While in the future all semantic transformations will be multi-method based, for now the transformation from syntax to semantics is done here with a `read` function.

Finally, provides helpers for accessing optional features (source locations, source text), traversing the data structure (as an acyclic graph, even if it isn't one), and printing the data structure.

### PSemantics

A semantic specific data structure provider. Contains the state of the semantics (e.g. if type inferencing has been ran), and the mechanics for manipulating the overall state. Contains an accessor to the syntax used to create these semantics.

Currently this provider assumes the semantics it describes is mapped to a single module.

### PBackend

A helper provider for classes which provide execution or compilation (or both). Because these sorts of systems tend to be heavy weight, and optional, this provider is used as the point for setting them up. Specifically used for the namespace bound providers below (`PBackend` and `PCompiler`).

### PExecutor

An execution specific data structure provider. Contains the state of the execution, and the mechanics for manipulating and introspecting that state.

Provides execution through an interface which takes a specific module and string on that module to execute. From there the executor will have to use it's understanding of the semantics to choose how to interpret that execution request.

While in the future all executors will be multi-method based, it is likely that an executor implementation will only support a single set of semantics at a time for the moment. This provider assumes it is mapped to a namespace. The implementation is also responsible for managing the state of each of the namespace's modules (and their functions).

Other interfaces may exist to increase cooperation between multiple executors.

### PCompiler

An compilation specific data structure provider. Contains the state of the compilation, and the mechanics for manipulating and introspecting that state.

Provides compilation through an interface which takes a specific module to compile and the output target of that compilation (e.g. a file, or in memory). Provides controls for purging the currently compiled contents (as it is assumed to be aggressive caching everything it is asked to compile for later use).

While in the future all compilers will be multi-method based, it is likely that a compiler implementation will only support a single set of semantics at a time for the moment. This provider assumes it is mapped to a namespace. The implementation is also responsible for managing the state of each of the namespace's modules (and their functions).

## Phases

This describes the phases that code goes through to be executed. Between each phase is a data structure that is communicated from one phase to the other. These phases are provided by the namespace (determined by the features it has loaded, namely backends and ~~frontends~~ (`PSyntax`) with the right providers). Each of these phases may invoke *arbitrary* other code in the system.

External requests for execution of these phases should go through the namespace and the modules it provides.

```c++
instance<lisp::Environment> env;

env->eval("(+ 3 4)"); // Uses the default UserModule: `user:*defualt*`

instance<lisp::Module> module = env->requireModule("file:test.cult");
instance<lisp::OpenModule> user_module = env->makeModule("my-foo-module");

// -- Execution --
instance<> res;

// Evals into the user module (only possible on open modules)
user_module->eval("(define test (fn (x) (concat (str x) \"testing\")))"); 

// Call a function in the module
res = env->exec(module, "test"); 
res = env->exec(user_module, "test", lisp::GenericCall { instance<uint64_t>::make(15) });

// -- Compilation --
instance<> compiler_options = PCompiler::deserialize_options(s);

// Compiles the whole namespace - as described by options - to the given path
env->compile(path, compiler_options); // this is how exectuables are done

// Compiles the module - as described by options - to the given path
env->compile(module, path, compiler_options); // custom user caching/AoT, prefer built-in caching
```

Internal requests should go through namespace as well:

```c++
// -- Parsing --
PSyntax::ParseOptions parse_options;

// Parsing (from string):
instance<CultLispSyntax> source = env->parse<CultLispSyntax>(some_string, &parse_options);
instance<> source = env->parse(some_string, cpptype<CultLispSyntax>::typeDesc(), &parse_options);

// -- Reading --
PSyntax::ReadOptions read_options;

// Reading (from string):
instance<CultSemantics> semantics = env->read<CultSemantics>(some_string, &parse_options, &read_options);
instance<> semantics = env->read(some_string, cpptype<CultSemantics>::typeDesc(), &parse_options, &read_options);

// Reading (from syntax):
instance<CultSemantics> semantics = env->read<CultSemantics>(source, &read_options);
instance<> semantics = env->read(source, cpptype<CultSemantics>::typeDesc(), &read_options);

// -- Execution --
// Execution uses the same interface as external code. This allows the language to know if it
//   is being re-entered from external code it may have called. However there are some extra
//   features meant for internal use only.

// `eval` Evaluates arbitrary code in a one-off module using the current state
Execution::getCurrent()->eval(s);
```

Other requests can use internal helper methods (since they will need full knowledge anyway):

```c++
// Get the semantic function from the module
auto func_sem = module->get<CultSemantics>()->get(function_name);
assert(func_sem./*isType*/hasFeature<SCultSubroutine>());

// create and get
func_sem = module->require<CultSemantics>()->get(function_name);

// Get the handle from the backend
auto func_backend_handle = env->get<LlvmBackend>()->get(func_sem);
auto module_backend_handle = env->get<LlvmBackend>()->get(module);

// -- Execution --
// These will setup the current Execution correctly.
func_backend_handle->exec( lisp::CultCall { ... } );
func_backend_handle->exec( lisp::GenericCall { ... } ); // GenericCall converts implicitly
module_backend_handle->exec("test", lisp::CultCall { ... } );

// -- Compilation --
// Meant to be deserialized
instance<LlvmCompilerOptions> compiler_options = instance<LlvmCompilerOptions>::make();

// We can use the compiler options the front end uses here
compiler_options->setOutputType(LlvmCompilerOptions::OutputType::IrString);
std::vector<uint8_t> out_artifact;
std::string out_messages;
module_backend_handle->compile(compiler_options, &out_artifact, &out_messages);
std::cout << std::string(out_artifact.begin(), out_artifact.end());

// Or we can manipulate it directly
auto func_ptr = func_backend_handle->getGeneric();
(*func_ptr)(...); // Call it directly

/// Specializing
// Get the specialization (nullptr otherwise)
func_ptr = func_backend_handle->getForTypes( lisp::types::Tuple { ... }, other_call_info);
func_ptr = func_backend_handle->getForCall( lisp::CultCall { ... } );

// Forcibly recompile
func_ptr = func_backend_handle->compileForTypes( lisp::types::Tuple { ... }, other_call_info );
func_ptr = func_backend_handle->compileForCall( lisp::CultCall { ... } );

// These try to compile and then get
func_ptr = func_backend_handle->requireForTypes( lisp::types::Tuple { ... }, other_call_info );
func_ptr = func_backend_handle->requireForCall( lisp::CultCall { ... } );
```



### Parsing

This phase parses the code into a syntax data structure (e.g. AST). Each language can have it's own syntax, parser, and data structure. The output of this step is a syntax specific structure.

This phase may execute reader macros, arbitrary code that parses strings and returns valid syntax data structures.

### Reading

This phase takes a syntax specific data structure and an semantics specific data structure and transforms from the given syntax to the given semantics.

This phase may execute macros, arbitrary code that mutates syntax data structures.

### Semantic Phases (Analysis, Type Inferencing, Borrow Checking)

Arbitrary semantic phases may be executed as required, this will typically be done automatically through checks in the semantic manipulation code.

These phases may execute arbitrary code if the analysis features or the semantic data structure are extended by the user.

### Execution

This phase takes a semantics specific data structure and executes it using an executor backend. Some executors may perform additional phases (such as compilation). This phase is used to perform arbitrary execution.

### Compilation

This phase takes a semantics specific data structure and compiles it using a compiler backend. Some compilers may perform additional phases (such as optimization).

These phases may execute arbitrary code if the compilation features are extended by the user.

## Implementations

### CultSyntax

This is the default syntax structure, a simple C++ implemented lisp syntax. See [the syntax document](./syntax) for more details.

The data structure this uses is a tree of `Sexpr`s with literals (as their literal type) and `Symbol`s as the leafs.

### CultSemantics

This is the default semantic structure. See [the semantics document](./semantics) for more details.

Cult supports "foreign semantics". Meaning that cult will search every layer of the module for symbols in foreign semantic layers (for example the cult CFFI module is a bunch of macros which manipulate the reading module's C++ semantics layer, so that later in the module, this system will find the foreign semantics).

### CppSemantics

This is a helper semantic structure for C++ code. It is for the following purposes:

* Modules written entirely in C/C++ (for example for bootstrapping, or as an "interpreter" extension). In this case these semantics simply provide the type/call signature and the exact pointer.
* Modules which wrap/expose C/C++ (e.g. through CFFI statements). In this case these semantics provide the expected type/call signature, and where to find the pointer (e.g. name/dll).
* Eventually, this semantics will be used for loading actual C/C++ header files as modules.
  * Finally, it might even be able to just compile C++ code using this object.

### BootstrapInterpreter

This is the default executor. It is designed to always be an availble fallback. It must always be availble, and must always have implementations available. Primarily it is used to execute various macros and compiler extensions used to implement macros and compiler extensions.

### LlvmBackend

A compiler and executor provider for `CultSemantics`.