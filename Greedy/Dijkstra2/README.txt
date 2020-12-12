With given maximum number of recharging,
find the minimum range of charge that make possible to visit all nodes from all nodes.

My solution:
Find the smallest edge of each node. Sum of all those edge is starting point.
Test and if failed, twice the range, untill it succeeded.
Once it success, use binary search algorithm with last failed range and last success range.