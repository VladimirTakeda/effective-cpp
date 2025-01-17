# Cache

## Problem

Write a program, that calculates the size, latency and associativity of all cpu data-caches empirically. This is a free-style task, where
you have to conduct experiments and write a report. So you can use any tools and approaches you see fit. However, there are a few rules:

1. Your program shouldn't use any hardware info to get the result. But you definitely should compare your results with real values on your system, and make corresponding conclusions. Don't forget to include the hardware characteristics of your cpu caches (and ram) in the report.
2. Your program may not output the results immediately, it may just output some data, that you use to make some plots for a example. So you're not
required to make a single executable, that just gives the result. However, all the steps from your report should be reproducible.
3. For your main measurement program make a single cmake target `cache-benchmark`. This task should be a separate cmake project, not a subdirectory
for `tasks` project.
4. Report should be uploaded as a pdf file in this directory.

This task is reviewed manually, the grading system is following:
- 600 points for the correct experiment for cache sizes and latency. There are may be some fines here in case of some mistakes. Note, that if you get slightly different results from the actual ones, it doesn't automatically mean there is something wrong with your experiment.
- 300 points for cache associativity calculation
- 100 points for easily reproducible builds and experiment

When you measure size/latency stats, there are 2 main issues you can encounter with:
1. Cache pollution due to other applications working. The same applies to your program as well, since you probably gonna run multiple iterations of measurements. So try to minimize the effect of other sources writing to cache.
2. Prefetching. Your measurements should be complex enough for cpu/compiler not to optimize/prefetch them. On the other hand, they should be simple enough to not introduce any overhead.

Before working with associativity, its recommended to watch the corresponding video. You should exploit the feature, that cache lines with the same block index are cached into the same set.
