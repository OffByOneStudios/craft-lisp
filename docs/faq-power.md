This faq aims to answer questions along the lines of "how do I solve X problem (commonly solved with Y)". The majority of the answers are going to be multimethods and type hints.

#### Q: How can a type describe a required/expected set of behaviours (traits)?

Define an explicit feature:

```
(cult/def-feature IExample
    :explicit ; default
    
    ; The I short hand stands for both implementing type and self
    ; TODO: I0 for zeroeth? or perhaps _0 or #0 or $0
    (foo (I@IExample String))
    ; The difference between (implementation implementation) dispatch and (implementation interface) dispatch
    (bar (I IExample))
)

; -- Implement directly
(cult/def-type Implementation
    (:inherit IExample)
    
    ; if directly inhereted, it must be implemented before the type macro finishes
    ; these will override multi methods
    (:implement IExample
        (foo (Implementation String) ...)
        (bar (Implementation IExample) ...)
    )
    
    ...
)
    
; -- Implement out of band
; these will override multi methods
(cult/implement IExample Implementation
    (foo (Implementation String) ...)
    (bar (Implementation IExample) ...)
)
```

#### Q: How can a type describe an existential set of behaviours (interfaces)?

Define an implicit feature:

```
(cult/def-feature IExample
    :implicit
    
    (foo (I@IExample String))
    (bar (I IExample))
    (zop ((Type I)))
)

; -- Implement directly (see previous)
; -- Implement out of band (see previous)
; -- Implement implictly
(def-method foo ((a Implementation) (b String)) ...)
(def-method bar ((a Implementation) (b IExample)) ...)
(def-method zop ((_ (Type Implementation))) ...)
; now it's implemented...
```


#### Q: How can a type provide behaviours?

Define an abstract type and attach behaviours to it:

```
(cult/def-abstract AnExample)

(def-method foo (AnExample String) ...)
(def-method bar (AnExample IExample) ...)

; If IExample was implicit, the below is enough to add the implementation
; if it was explicit then AnExample would have to have inherited IExample (and hence it's interface constraints)
(cult/attach Thing AnExample)
; In the case of types does the following:
;(cult/proclaim SomeClass (cult/append is-a EffectedClasses))
;(cult/proclaim SomeClass (cult/prepend dispatch-order EffectedClasses))
```


#### Q: How can a concrete type inherit from more than one type (multiple inheritance)?

Inherit from each type. This does three things:
* For each parent class: add an is-a subtype relationship.
* Set the type's preferred dispatch order for dispatch ambiguities.
  * Deal with the diamond problem...
    * Breadth? Depth? Per case? Choose a strategy?
* For each parent type that is concrete: composite the parent's structure into the current structure.
  * Deal with the diamond problem...
    * Concrete diamond types (not subtypees) determine if they prefer conjoined inheritance (e.g. virtual), split inheritance, or some other strategy, it is an error if the diamond type has not chosen a strategy.
    * Feature types are a no-op because as long as the dispatch order is resolved it doesn't matter which feature hierarchy was reached.
    * Abstract types are a no-op because they are just implementations.

```
; -- With a type declaration
(cult/def-type Example
    (:inherit AlphaClass BetaClass)
    
    ...
)
```

#### Q: How can a type dispatch as more than one type (multiple inheritance)?

Subtype from both types.

```
; -- Out of line
(cult/proclaim Example (cult/set is-a [HierarchyAlphaClass HierarchyBetaClass]))
(cult/proclaim Example (cult/set dispatch-order [HierarchyAlphaClass HierarchyBetaClass])) ; must be set separately here


; -- With a type declaration
(cult/def-type Example
    (:is-a AlphaClass BetaClass) ; sets dispatch-order for convenience, unless set separately
    
    ...
)
```

* This will perform consistency checks: features will ensure the type implements them correctly, concretes will ensure the type has the correct structure (or that it has been entirely erased as necessary for the type ????? field reference overrides, function implementation overrides), abstracts don't have any checks, etc.


#### Q: How can a type copy behaviour from more than one type (multiple inheritance)?

Refactor each type so the behaviour-to-be-shared is in a separate abstract (or concrete) type instead. Basically refactor the behaviour into mixins (and put them at the front of the old type's dispatch list) and inherit those instead. Follow these steps:

#### Q: How can a type choose which copy of a behaviour to use on a per dispatchable basis (multiple inheritance)?

Refactor the involved types so the is-a dispatch list is correct if possible/preferable. If it's not, then define a new method for the type (becoming the most specific method) and simply invoke the correct super method:

```
(cult/def-method ambiguous ((object DiamondishRoot))
    ; Normally super would call the next most specific method, which would be by dispatch order but
    ; here we use the optional feature allowing us to provide new (less specific than the currently
    ; dispatching method) types to dispatch the next one too.
    ;
    ; The implementation should elide this call entirely as a dispatch hint.
    (super@ambiguous (BetaClass)))

; -- Helper macro:
(cult/override ambiguous (DiamondishRoot) (BetaClass))
```

If you find yourself doing this more than once for a given type (or for multiple types), consider refactoring the override methods to point at an abstract type, and add that to the inheritance list.

#### Q: How can I change the sort order of multimethods?

Avoid using this, it may seriously compromise dispatch (sanity, performance, safety), however:

```
(cult/def-method foo:my-implementation (...) ...) ; named implementations help

(cult/define -other-foo-implementation (dispatch foo (...)))

(cult/proclaim foo (insert-before methods foo:my-implementation -other-foo-implementation))
```

In most cases this will probably be ignored anyway due types determining their dispatch order.


#### Q: How can I have more complex behaviours that dispatch multiple times in different ways (CLOS multimethods)

Use a custom multi-method executor object:

```
; This is a representation of the `cult/clos-executor`
(cult/define my-mm-executor
    (cult/executor ; this is a macro to help write executors
        (dispatch :around :virtual :covariant (:wrapping
                (dispatch :before :all :covariant)
                (dispatch :default :virtual :covariant)
                (dispatch :after :all :contravariant)
        ))
))

(cult/def-multi foo
    ; defaults to `cult/simple-executor` which just does a (dispatch :virtual :covariant)
    (:executor my-mm-executor)
    
    ...
)


#### Q: How can I modify the behaviour of types out of band (aspect oriented programming)

Define an abstract type to dispatch off of, and add it to the relevant types. Create a generic method disaptching to that abstract type and attach it to the relevant multi-methods.

```
(cult/def-abstract EffectedClasses)
(cult/attach SomeClass EffectedClasses)

; if we only wanted to modify a single method, a normal `def-method` would suffice.
(cult/define -my-magic-method
    (cult/method
        (:type-vars B V)
        (:signature (a EffectedClasses) (b B) (args V :var-args))
        (:stage :before)
        (:predicate ...)
        (do 
            ...
        )
    ))
(cult/proclaim foo (cult/insert-sorted methods -my-magic-method))
(cult/proclaim bar (cult/insert-sorted methods -my-magic-method))

```

#### Q: How can I modify the behaviour of behaviours out of band (aspect oriented programming cut points)

There are two answers here, depending on how far down the AOP rabbit hole one wishes to go.

1. The simpler answer is to plan ahead (or slightly refactor existing code) by using multi-method combinators to set up phases to the behaviours in question so that when changes need to be made, the cut points are part of the code.

2. The more complex answer leverages the fact that we have full compiler/jit support at runtime. One may write an AOP library in language that is capable to rewriting the code in question at runtime (or compile time! or link time!).


## Other notes:

* Name ops (precedence):
    * same precedence (since these must appear first in the symbol)
        * unary `:` is the keyword operator
        * unary `.` is the anonymous form of the binary `.` operator (e.g. apply to first argument, rather than named value)
        * ?? unary `/` from root namespace
        * ?? unary `@` ignore module boundries, resolve globally
    * binary `/` is the directory symbol op, it's a way to describe the location of a symbol, and can be optional (e.g. if the namespace is currently being used) and clarifying (if the symbol s ambiguous between namespaces).
    * binary `@` is the source symbol op, it's a way to reference a symbol from a specific source (like a macro), unlike other name operators the "container" name is on the right.
      * ?? if the container is a keyword is that a namelayer
    * shared precedence, left to right
      * binary `.` runtime field access, closure construction, (and 1 arg calls?)
      * binary `:` compile/link time type access