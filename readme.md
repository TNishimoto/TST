
# Truncated Suffix Tree

This program is a C++ implementation of the truncated suffix tree with space-efficient representation. 
Our implementation is based on the idea in [https://www.sciencedirect.com/science/article/pii/S030439751500523X].

Let Q be the number of distinct q-grams in input string T.
Then this truncated suffix tree uses O(Q) space. 

# Compile
cmake -DCMAKE_BUILD_TYPE=Release .
make

# Usage

You can construct q-truncated suffix tree using the following command. 
./construct.out -i input_file -o output_file -q qgram_length -a option

-i : the file path of input text, 
-o : the file path of output data(q-TST), 
-q : the value q, 
-a : 0 : without count vector, 1 : with count vector(internal nodes + leaves), 2 : with only internal node count vector.

You can enumerate all q-grams which prefix is P in the text T using the following command.
./search.out -i tst -q query_file -m locate
-i : the file path of q-TST of T, 
-q : the file path of query file.
(each line of query file represents a pattern P)

You can count P in the text T using the following command. 
./search.out -i tst -q query_file -m count
-i : the file path of q-TST of T, 
(the q-TST needs the count vector(-a 1))
-q : the file path of query file. 
(each line of query file represents a pattern P)

# Example
./construct.out -i test/test.txt -o test.tst -q 5 -a 1
./search.out -i test.tst -q test/query.txt -m locate

/*
query : 1 / cam
q-gram : came!, Count : 1
Total count : 1
query : 2 / He
q-gram : He we, Count : 1
q-gram : He to, Count : 1
q-gram : He le, Count : 1
q-gram : He ch, Count : 1
Total count : 4
query : 3 / tr
q-gram : tree,, Count : 1
Total count : 1
query : 4 / snack
q-gram : snack, Count : 1
Total count : 1
query : 5 / the
q-gram : the w, Count : 2
q-gram : the t, Count : 1
q-gram : the s, Count : 2
q-gram : the m, Count : 3
q-gram : the c, Count : 1
q-gram : the b, Count : 2
q-gram : the T, Count : 1
q-gram : the J, Count : 3
*/

./search.out -i test.tst -q test/query.txt -m count

/*
query : 1 / cam
the pattern occurs 1 times
query : 2 / He
the pattern occurs 4 times
query : 3 / tr
the pattern occurs 1 times
query : 4 / snack
the pattern occurs 1 times
query : 5 / the
the pattern occurs 15 times
Finish.
*/