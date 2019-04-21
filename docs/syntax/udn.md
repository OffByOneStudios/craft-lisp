Universal Data Notation or UDN is an attempt to step towards a lisp (similar to EDN) but with smaller steps. UDN focuses on only a couple of major concerns.

## UTS

The ultimate tree structure (working name) is straight forward. There are only two kinds of things. Nodes and atoms. Atoms are a value kind of some sort. Nodes can be an atom, or a more complex structure.

Aside: The "node" and "atom" notation is merely for the tree data structure. A specific type could be used as both a node and an atom in a structure. Schemas dictate the translation of types to atoms and nodes.

A node has the following parts:
  * primary: the primary atom
  * ordering: an ordered collection of nodes
  * mapping: a mapping of atoms to nodes

This data structure the entirety of the UTS description.
  
Aside: Nodes and atoms often have taints associated with them based on how it was parsed. This can be a type system for example. For example a UTS parser will often taint atoms with a type (string, number), less often they will taint nodes (dictionary, list).

### Case Study: XML

Mapping to XML:

* primary: Element Symbol
* ordering: Children elements
* mapping: Attributes

However attributes can't map to node style children. To fix this a compatibility parser could support a children mapping syntax found in some languages.

```
<Foo bar="atom to atom mapping">
  <Foo.baz>
    An arbitrary mapping.
  </Foo.baz>
</Foo>
```

This naturally still works with the ordering system.

### Case Study: YAML

Mapping to YAML:

* primary: Type symbol
* ordering: List syntax
* mapping: Dict syntax

However this prevent ordering and mapping in the same node. To fix this a compatibility parser could support using the null key as a source for ordered lists on that node.

### Case Study: EDN

Is a mess for representing this kind of data, and it takes too many additional liberties on top of the structure parsing (identifiers, metadata, keywords, etc.)

## UDN

UDN is a configurable UTS parser that is designed to be a simple and straight forward as possible. It uses a taint system to pass on notable information if it is required.

Below are the possible bits of configuration:

* Node Delimiters, by default: `{}`,`()`,`[]`,`<>`, these delimit a node.
* Inline Delimiters, by default: `,`, these allow for short nodes to be generated.
* Atom Parsers, by default: `"`, these allow for escape character parsing and more importantly, for ignoring other special characters inside of atoms.
* Mapping Assignment, by default `=`, `:`, these allow for the invocation of mapping behavior.
* Primary Atom Seperator, by default `|`, these allow seperaqting the primary atom from the rest of the node.
* Comment Character, by default `;`, these allow for everything following to be treated as not-important.
* Space, by default all whitespace, these separate terms.

This is the limit of syntax in a UDN parser, and all of it may be configured (including multiple options for each). In general if multiple choices are allowed in any of the above a taint is placed on the relevant atom or node to represent that. In addition a reactive version of the parser allows the system to replace a reader mid parse.

UDN also standardizes some optional parse control features. Including directives (default `%`) and document separators (`...` and `---`) for those use cases.

### Specification

#### Atoms

Atoms can be anything that doesn't contain the above special characters.

Additionally atoms can be explicitly described using atom parsers:

```
"An atom with spaces"
"An (atom with delimiters)"
```

#### Root

The root of the document is a special node (the manipulation of which can be set by the parser and directives. But for now we will use this property to display some features of UDN.

#### Nodes as a List

A node can contain a sequence of elements:

```
apple banana orange
```
```
apple
banana
orange
```

These are both effectively turning the root node into a list of 3 elements, we can make it an explicit node by using delimiting characters:

```
(apple banana orange)
```
```
<apple banana orange>
```

And now the root node contains a single node which is effectively a list.

#### Nodes as a Mapping

A node can map atoms to elements:

```
first-name: Joe last-name: Bob
age: 15
job-title: "This is Joe Bob's Job Title"
```

The mapping character can appear in any position:

```
a=15 b = 17 c= 19 d =21
```

And again we can use any delimiting character

```
(a=15 b = 17 c= 19 d =21)
```
```
[a=15 b = 17 c= 19 d =21]
```

#### Nodes as Both

A node can be both simultaneously:

```
(apple banana freshness=15 orange total-weight: 14kg)
```

Where we have one object containing three atoms in sequence and two key/value pairs.

#### Nodes with Primary Atom

We can also add primary atoms to nodes by using the primary atom separation character.

```
[Basket|
  apple banana
  freshness=15 orange
  total-weight: 14kg
]
```

An optional closing atom can also be used (spaces are also optional) they must match:

```
[Basket |
  apple banana
  freshness=15 orange
  total-weight: 14kg
| Basket]
```

As well as in arbitrary locations (they must match).

```
[Basket|
  apple banana
  freshness=15|Basket|orange
  total-weight: 14kg
|Basket]
```

### Schema

Generally speaking UDN is meant to be interpreted using a schema (or higher level system) which translates atoms into specific types of data.

Additionally this system can interpret the tree however it wishes.

## Lisp

The point of UDN is that we can use it as the basis of a lisp. While retaining the data entry characteristics of XAML. We must however work around a couple of things.

1. Symbolic function names involving and of `<>=` are problematic, they could still be referenced with a "<=" (if the string taint didn't effect it...)
2. Keywords are traditionally referenced with a leading `:` which we now use for dict assignment. We still have a need for self evaluating symbols.

Some solutions for these problems involve:

1.
  a. Prefer written out names for everything. And we still have quite the symbol space available to us.
  b. Use separate modes for symbol heavy things (like math)
  c. Support latex symbols `\foo`
  d. Support unicode symbols
2.
  a. Use a different keyword symbol ( `#`? )

