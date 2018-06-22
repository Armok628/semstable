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

Command | Action
`key s` | Print the hash key of "s"
`add s n` | Add value "n" to entry "s"
`set s n` | Set entry "s" to "n". Won't work if not `add`ed.
`get s` | Get the value of the entry for "s"

On my machine, the retrieval phase of running `./a.out 1000 10000 100` takes 0.4 seconds.

### Library Usage

*(to-do)*
