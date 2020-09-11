htab - a simple and efficient hashtable for C
=============================================

this is the hashtable implementation used by musl libc's hsearch() function
(originally authored by Szabolcs Nagy aka nsz), refactored to provide a nicer
and threadsafe API and enhanced with a delete and an iterator function.

i've been looking for a good general-purpose hashtable implementation for C
for a while, but what i found wasn't really satisfactory.

most are really complex, hard to use and one can only hope they're not full
of bugs. some of them provide type safety but use non-standard extensions
like `typeof`, others expand macros that span over dozens or even hundreds of
lines.

my requirements were basically:

- fast
- simple to use
- bug-free
- well-tested
- well-documented or self-explanatory
- contained in one source file and header
- readable
- as little code as possible
- liberally licensed (MIT or PD)

when i found out that POSIX provides a built-in `hsearch` hashtable
implementation (which, however has an odd interface, is not threadsafe,
and lacks delete and iterator functions), i figured that the best i could come
up with is to take musl's version and adapt it to my needs.
and indeed, the result satisfies every single requirement from the above list.
the code is less than 200 lines, easily readable, and amazingly fast:
in my tests it was about 20% faster than the well known `khash`.

nsz made another hashtable library called [genht](http://repo.hu/projects/genht),
but unfortunately it lacks decent documentation, consists of multiple files,
and the latest sources are only available via a svn checkout.
i have no doubts though that it is of highest quality too.

-------------------------------------------------------------------------------

this hash table implementation is currently hardwired to be used with string
keys. it would be trivial to support other types too, but the more generic,
the more messy the API becomes.

see test.c for usage examples.
