Universal Data Notation or UDN is an attempt to step towards a lisp (similar to EDN) but with smaller steps. UDN focuses on only a couple of major concerns.

## UTS

The ultimate tree structure (working name) is straight forward. There are only two kinds of things. Nodes and atoms. Atoms are a value kind of some sort. Nodes can be an atom, or a more complex structure.

Aside: The "node" and "atom" notation is merely for the tree data structure. A specific type could be used as both a node and an atom in a structure. Schemas dictate the translation of types to atoms and nodes.

A node has the following parts:
  * a primary atom
  * an ordered collection of nodes
  * a mapping of atoms to nodes
  
Nodes and atoms can have taints associated with them.

### Case Study: XML

Element name is primary atom
Attributes are the mapping.
Child Elements are the ordered collection.

### Case Study: YAML

### Case Study: EDN

### Case Study: Lisp
  
## UDN

First, the following are the configurable values:

* Delimiters, by default: `{}()[]<>`