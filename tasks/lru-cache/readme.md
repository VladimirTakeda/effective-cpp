# Lru Cache

## Problem

Implement class `LruCache` in `lru_cache.h`. This cache represents an associative cache `K -> V` of a limited `capacity`.
During `Set` operation, if cache is full, then evict an element using `least recently used` strategy. That is, remove the key (and corresponding value),
which hasn't been `Set` or `Get` for the longest period of time.

`Set` should update the value (and its modification time) if a given key exists in cache. `Get` should return a pointer to the corresponding value, or
`nullptr` if there is no such value.

**Both `Set` and `Get` should have O(1) asymptotic complexity**.

## Notes

- Organize elements into `list` and store iterators to this list in `unordered_map`.
