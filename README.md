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
`add s n` | Add value "n" to entry "s", or set to "n" if already present.
`set s n` | Set entry "s" to "n". Won't work if not `add`ed.
`get s`   | Get the value of the entry for "s"

On my machine, the retrieval phase of `./a.out 1000 10000 100` takes 0.4 seconds.

### Library Usage

I'm writing this mostly as a reminder to myself when I use it later.

Values are stored as `void *`, but with typecasting you can store anything of the same width.

Function                        | Notes
---                             | ---
`new_table(size,destructor)`    | The destructor provided is used when destroying table values. Use `NULL` to do nothing instead.
`add_entry(table,string,value)` | If an entry is already present, it will destroy the old value and assign the new one.
`get_entry(table,string)`       | This function returns null when no entry is found.
`set_entry(table,string,value)` | Does nothing when no entry is found. This function can be substituted by `add_entry`.
`free_table(table)`             | Destroys the table's memory, and destroys all stored values using the destructor.

See test.c, past `NOT_ENOUGH_ARGS:` for an example of storing `long`s as values instead of malloc'd blocks.
