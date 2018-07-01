# simplehashtest
A hash table implementation I made to use somewhere else

I made this for two reasons
* I wanted a hash table implementation to use somewhere else.
* I wanted to figure out how hash tables work by making one.

Long story short: I didn't want to learn how to use someone else's work if I'm only going to use it once, especially since I'm probably going to use it in a personal project (prototype) where it doesn't matter how good it is. So, I figured I might as well make it myself so I can at least learn from it.

### Test Program Usage

Compile with `make`.

Run `./a.out x y z` to make a hash table with x buckets, y entries, and then access each entry z times (for timing purposes).

Only the retrieval phase is timed.

Run without arguments to manually test it, using the following commands:

Command   | Action
---       | ---
`key s`   | Print the hash key of "s"
`set s n` | Set "s" to "n", adding the entry if undefined
`get s`   | Get the value of the entry for "s"

On my machine, the retrieval phase of `./a.out 1000 10000 100` takes 0.4 seconds.

### Library Usage

I'm writing this mostly as a reminder to myself when I use it later.

Values are stored as `void *`, but with typecasting you can store anything of the same width.

Function                     | Notes
---                          | ---
`new_table(size,destructor)` | "Size" represents number of buckets. "Destructor" is a function pointer used to destroy stored values.
`insert(table,string,value)` | If an entry is already present, it will destroy the old value and assign the new one.
`void *lookup(table,string)` | Returns null when no entry is found.
`free_table(table)`          | Destroys the table's memory and all stored values.

See test.c, past `NOT_ENOUGH_ARGS` for an example of storing `long`s as values instead of malloc'd blocks.

As for storing pointers to multiple types, it's easy enough to implement your own variable type system with `enum` and `union`; I won't explain it here.
