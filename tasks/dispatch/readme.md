# Dispatching

## Problem

This task consists of 2 subtasks.

### Advance

Implement function `Advance(iterator, n)` in `advance.h`, which shifts given iterator by `n` elements. This function should have O(1)
    complexity for random-access iterators and raw pointers. It should also support negative shifts for bidirectional iterators. Use __dispatching__
    to create a function, that behaves differently depending on an iterator's tag. You can find an example here: https://en.cppreference.com/w/cpp/iterator/iterator_tags.

### Clear

Implement function `Clear` in `clear.h`, which does the following:
  - if `value` is a raw pointer, it calls `delete` and sets it to `nullptr`.
  - if `value` is `unique_ptr` or `shared_ptr`, it calls `reset` method.
  - if `T` defines `Clear` method, it calls this method.
  - otherwise `Clear` does nothing.

You should implement a simple concept to check for `Clear` method existance. Use partial template specializations to implement the function after that.
