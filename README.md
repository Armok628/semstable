# simplehashtable
A simple implementation of a hash table

I made this for two reasons
* I wanted a hash table implementation to use somewhere else.
* I wanted to figure out how hash tables work by making one.

### Test Program Usage

Compile with `make`.

Run `./a.out x y z` to make a hash table with x buckets, y entries, and then access each entry z times (for timing purposes).

Alternatively, you can use something like `./a.out size=x words=y tests=z`, as well as any of the following, and in any order.

Argument    | Action
---         | ---
`--dump`    | Print out number of buckets in each location after test
`--silent`  | Print out only retrieval phase duration
`--expunge` | Manually remove each entry before cleaning up
`seed=n`    | Manually set the PRNG seed

That said, only the retrieval phase is timed.

Run without (or with insufficient) arguments to manually test it, using the following commands (which mirror their function's name):

Command       | Action
---           | ---
`hash_key s`  | Print the hash key of "s"
`insert s n`  | Set "s" to "n", adding the entry if undefined
`lookup s`    | Get the value of the entry for "s"
`expunge s`   | Destroy data for "s"
`new_table n` | Make a new table with size "n"

### Library Usage

I'm writing this mostly as a reminder to myself when I use it later.

Values are stored and returned as `void *`, but with typecasting you can store anything of the same width.

Function                     | Notes
---                          | ---
`new_table(size,destructor)` | "Size" represents number of bucket locations. "Destructor" is a function pointer used to destroy stored values.
`insert(table,string,value)` | Adds an entry for string. If an entry for string is already present, it will destroy the old value and assign the new one.
`lookup(table,string)`       | Returns the value of the entry for string, or null if no entry is found.
`expunge(table,string)`      | If an entry for string is found, destroys its value and removes the entry.
`free_table(table)`          | Destroys the table's memory and all stored values.

The table will not check for null values before destroying.

For an example of storing `long`s as values instead of malloc'd blocks, see test.c past `NOT_ENOUGH_ARGS`.

As for storing pointers to multiple types, it's easy enough to implement your own variable type system with `enum` and `union`; I won't explain it here.

### Performance

Generated on my PC with the included benchmark script:

 | Trials/Word: 100 | Size: 10 | Size: 100 | Size: 1000 | Size: 10000 | Size: 100000 | 
 | --- | --: | --: | --: | --: | --: | 
 | Words: 1 | 0.083 ms | 0.083 ms | 0.083 ms | 0.083 ms | 0.083 ms | 
 | Words: 10 | 0.843 ms | 0.812 ms | 0.871 ms | 0.755 ms | 0.760 ms | 
 | Words: 100 | 5.639 ms | 2.059 ms | 2.063 ms | 2.194 ms | 2.100 ms | 
 | Words: 1000 | 36.457 ms | 24.259 ms | 21.834 ms | 21.538 ms | 21.688 ms | 
 | Words: 10000 | 5242.109 ms | 715.941 ms | 264.932 ms | 212.193 ms | 208.782 ms | 

