# semstable
A simple implementation of a hash table

I made this for two reasons
* I wanted a hash table implementation to use somewhere else.
* I wanted to figure out how hash tables work by making one.

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
`hash_key s`  | Print the hash key of "s"
`insert s n`  | Set "s" to "n", adding the entry if undefined
`lookup s`    | Get the value of the entry for "s"
`expunge s`   | Destroy data for "s"
`new_table n` | Make a new table with size "n"

Memory safety is verified through the use of Valgrind.

### Library Usage

I'm writing this mostly as a reminder to myself when I use it later, so this section is somewhat simplified.

The library itself is contained within src/hash.\*

Values are stored and returned as `void *`. For more type info, see src/hash.h

Function                     | Notes
---                          | ---
`new_table(size,destructor)` | "Size" represents number of bucket locations. "Destructor" is a function pointer used to destroy stored values.
`insert(table,string,value)` | Adds an entry for string. If an entry for string is already present, it will destroy the old value and assign the new one.
`lookup(table,string)`       | Returns the value of the entry for string, or null if no entry is found.
`expunge(table,string)`      | If an entry for string is found, destroys its value and removes the entry.
`free_table(table)`          | Destroys the table's memory and all stored values.

The table will not check for null values before destroying.

For an example of storing `long`s as values instead of malloc'd blocks, see test.c past `NOT_ENOUGH_ARGS`.

As for storing pointers to multiple types, it's easy to implement a variable type system with struct, enum, and union, and make a destructor with a switch/case block.
My LISP dialect uses a similar variable type system.

### Performance

Generated on my desktop PC with the included benchmark script:

| Trials/Word: 100 | Size: 10 | Size: 100 | Size: 1000 | Size: 10000 | Size: 100000 | 
| --- | --: | --: | --: | --: | --: | 
| Words: 1 | 0.007 ms | 0.007 ms | 0.007 ms | 0.007 ms | 0.007 ms | 
| Words: 10 | 0.078 ms | 0.071 ms | 0.075 ms | 0.076 ms | 0.066 ms | 
| Words: 100 | 0.750 ms | 0.665 ms | 0.581 ms | 0.580 ms | 0.629 ms | 
| Words: 1000 | 44.336 ms | 10.355 ms | 8.738 ms | 7.043 ms | 6.283 ms | 
| Words: 10000 | 4391.766 ms | 590.359 ms | 167.462 ms | 138.469 ms | 79.374 ms | 

