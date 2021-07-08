# A solution to the traveling salesman problem using 3-opt optimization algorithm
In order to solve the traveling salesman problem, I designed a version of 3-opt algorithm. The 3-opt algorithm starts with an initial tour and deletes 3 edges. These edges can be reconnected with 8 different combinations, original tour included.


![image](https://user-images.githubusercontent.com/43146149/124963789-e75ede80-e028-11eb-87f8-b7de0f402014.png)


I named these cases with numbers 0-7 and the nodes with letters A-F. Using this notation, reconnected tours for each case are given below.

A→B→C→D→E→F	(Original case 0, edges A→B, C→D, E→F, are removed.)

A→E→D→C→B→F	(Case 1)

A→B→C→E→D→F	(Case 2)

A→C→B→D→E→F	(Case 3)

A→C→B→E→D→F	(Case 4)

A→E→D→B→C→F	(Case 5)

A→D→E→C→B→F	(Case 6)

A→D→E→B→C→F	(Case 7)

In each case, the segments between the reconnected nodes are reversed.
These cases result in longer or shorter tours. For each case, the change of length of the tour is calculated with:

Gain = Length of added edges – Length of deleted edges

For example, gain in case 1 is:
Length(A→E) + Length(B→F) – Length(A→B) – Length(E→F)

The amount of gain can be calculated for each case and reconnected with the case that results in a maximum gain. The algorithm is built on the iteration of this process for each edge until no further improvement is made.

The algorithm I designed starts with an initial tour, constructed by a simple nearest-neighbor algorithm.
- While the tour improves,
  - Start the loop
  - For each three edges,
    - Calculate the amount of gain for each case,
    - If a shorter tour is possible, make the case that gives the shortest tour the current tour and break the loop.
      
This algorithm greatly improves the tour given by the nearest-neighbor algorithm.
