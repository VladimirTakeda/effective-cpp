# Hashmap

## Problem

In this task you should implement a special version of hashmap, designed for a lot of insertions and deletions, which is common for handling orders
in HFT for a example.

Use open addressing scheme from the lecture with step 1 for probing. Use a different deletion mechanism, which is called
backshift deletion. The idea is simple: marking elements as deleted isn't effective when a lot of deletions are performed. This way a table
consists of a lot of "empty" elements, which are still needed to be examined by lookup operations. So instead of marking a bucket as deleted, do the following:
1. Maintain current free bucket, initially its the one being deleted
2. Start probing buckets until you encounter an empty one
3. For every filled bucket check whether its worth to move it to the current free bucket (its worth if the distance from it's initial position decreases). If it is, then move this bucket to the current free one and set current position as a new free spot.

There is a good picture with example here: https://codecapsule.com/2013/11/17/robin-hood-hashing-backward-shift-deletion/.

For a example, consider the following buckets (letters denote different elements and numbers denote the distance to the initial hash position of each element, '.' stands for a free bucket, the first one is being deleted):
```
.abcdefg.
.1234067.
^
```

Following the algorithm we move all elements before 'e' by one bucket to the left:
```
abcd.efg.
0123.067.
     ^
``` 
Its not worth to move 'e', so we don't touch it and move both 'f' and 'g':
```
abcdfeg..
0123406..
```

## Implementation details

Implement template `HashMap<K, V>` in `hashmap.h`. The interface is given there already, note the following:

1. hasher is an object that maps `K -> size_t`. You can use the resulting number to calculate the bucket number, e.g. `hash(key) % buckets_count`. In this task
the number of buckets is always some power of 2, so you can use `hash(key) & (t - 1)`, where `2^t` is the size of table.
2. You should maintain load factor 0.5 at most. Which means, that whenever half of the table is occupied, rehash it to another table 2 times bigger.
3. The same is true for the constructor taking expected amount of elements, so find the next power of 2, that at least 2 times bigger.
4. For empty table start with initial size 8.
5. As you can see, you should also implement `iterator` type, which actually behaves like `const_iterator`. It should be a forward iterator.
You can check `test.cpp` for usages.

There is a small benchmark there, you should see a performance boost comparing to unordered_map.
