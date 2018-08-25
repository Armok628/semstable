# semstable
A simple, efficient, and memory-safe hash table for use in C

It is memory-safe in that you can supply a "destructor" (to borrow from OOP terminology) to a hash table
for use on pointers when they are removed from the table,
thus removing some of the burden of memory management in an already-complex project.

For example, this hash table implementation is used in my garbage-collected LISP dialect, [ISPL](../../../ispl).

### Test Program Usage

Compile with `make`.

Run `./a.out x y z` to make a hash table with x buckets, y entries, and then access each entry z times (for timing purposes).

Alternatively, you can use something like `./a.out size=x words=y tests=z`, as well as any of the following, and in any order.

Argument       | Action
---            | ---
`--dump`       | Print out number of buckets in each location after test
`--expunge`    | Manually remove each entry before cleaning up
`--time_only`  | Print out only retrieval phase duration
`--no_adds`    | Do not report entry additions
`--print_keys` | Report hash keys with entry additions
`seed=n`       | Manually set the PRNG seed

That said, only the retrieval phase is timed.

Run without (or with insufficient) arguments to manually test it, using the following commands (which mirror their function's name):

Command       | Action
---           | ---
`fnv_1a s`    | Print the hash key of "s"
`insert s n`  | Set "s" to "n", adding the entry if undefined
`lookup s`    | Get the value of the entry for "s"
`expunge s`   | Destroy data for "s"
`new_table n` | Make a new table with size "n"

Memory safety is verified through the use of Valgrind.

### Library Usage

I'm writing this mostly as a reminder to myself when I use it later, so this section is somewhat simplified.

The library itself is contained within src/hash.\*

The hash function used is FNV-1a. I had come up with my own initially, but FNV-1a has a better dispersion pattern.

Values are stored and returned as `void *`. For more type info, see src/hash.h

Function                     | Notes
---                          | ---
`new_table(size,destructor)` | "Size" represents number of bucket locations. "Destructor" is a function pointer used to destroy stored values.
`insert(table,string,value)` | Adds an entry for string. If an entry for string is already present, it will destroy the old value and assign the new one.
`lookup(table,string)`       | Returns the value of the entry for string, or null if no entry is found.
`expunge(table,string)`      | If an entry for string is found, destroys its value and removes the entry.
`free_table(table)`          | Destroys the table's memory and all stored values.

The table will not check for null values before destroying, so make sure your destructor can handle NULL.

For an example of storing `long`s as values instead of malloc'd blocks, see test.c past `NOT_ENOUGH_ARGS`.

As for storing pointers to multiple types, it's easy to implement a variable type system with struct, enum, and union, and make a destructor with a switch/case block.
My aforementioned LISP dialect uses a similar system.
