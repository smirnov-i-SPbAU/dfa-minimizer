To compile: from this directory run `bash setup.sh` or `make`

Executable file is `minimizer`

## Options ##

-i 

	input file name (default: default/in.gr)
-o

	output file name (default: default/out.gr)
-p

	print file name (default: default/outpr.png)
-s

	filename to save dot-formatted graph (default: default/out.gv)
-n

	disable printing

Graph format:

First line contains alphabet symbols: small latin letters and digits without splitters.

Second line contains one non-negative integer V -- number of vertexes.

Third line contains one integer s (1 <= s <= V) -- number of start vertex.

Fourth line contains integer T -- number of vertexes and T unique integers t_i -- numbers of terminal vertexes. All integers are splitted with spaces.

Next V lines contains |ALPHABET| integers splitted by spaces. j-th integer in i-th (a_{i,j}) line means edge from i-th vertex to a_{i, j} vertex by an edge with j-th symbol of alphabet. a_{i, j} = V + 1 means that there is no edge from i-th vertex with j-th symbol of alphabet on it.
