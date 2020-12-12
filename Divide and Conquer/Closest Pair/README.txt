Find two points which have the least distance between them = closest pair.

Split all vertices along the vertical line. If all points have same x value, than split along the horizontal line.
Find closest pair in the divided set of points.
To merge, we need to check edges across the split line.
The number of check must be 6 maximum, because we can ignore points farther than minimum distance of divided sets.
Therefore, complexity is O(nlogn).
