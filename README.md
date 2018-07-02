# simplehashtable
A simple implementation of a hash table

I made this for two reasons
* I wanted a hash table implementation to use somewhere else.
* I wanted to figure out how hash tables work by making one.

### Test Program Usage

Compile with `make`.

Run `./a.out x y z` to make a hash table with x buckets, y entries, and then access each entry z times (for timing purposes).

Alternatively, you can use something like `./a.out size=x words=y tests=z`, as well as any of the following, and in any order.

Argument  | Action
---       | ---
--dump    | Print out number of buckets in each location after test
--silent  | Print out only retrieval phase duration
--expunge | Manually remove each entry before cleaning up

That said, only the retrieval phase is timed.

Run without (or with insufficient) arguments to manually test it, using the following commands (which mirror their function's name):

Command       | Action
---           | ---
`hash_key s`  | Print the hash key of "s"
`insert s n`  | Set "s" to "n", adding the entry if undefined
`lookup s`    | Get the value of the entry for "s"
`expunge s`   | Destroy data for "s"
`new_table n` | Make a new table with size "n"

On my machine, the retrieval phase of `./a.out 1000 10000 100` takes 0.3 seconds.

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

Generated on my PC with table.pl

 | Trials/Word: 100 | Size: 10 | Size: 100 | Size: 1000 | Size: 10000 | 
 | :-- | --: | --: | --: | --: | 
 | Words: 1 | 0.000052 | 0.000052 | 0.000052 | 0.000052 | 
 | Words: 10 | 0.000547 | 0.000499 | 0.000492 | 0.000492 | 
 | Words: 100 | 0.005163 | 0.004814 | 0.004731 | 0.004472 | 
 | Words: 1000 | 0.071027 | 0.034830 | 0.027958 | 0.043342 | 
 | Words: 10000 | 4.415370 | 0.743896 | 0.351098 | 0.309820 | 
