
# Scope
The goal is to create statistics for analyzing a text file. For example, we want to determine the frequencies of words in the text, the frequency of words with a certain length, etc.

![image](https://github.com/craciun25alexandra/Dictionary-search/assets/94574197/46a740f9-acb5-47ab-aa35-9be02d2e5158)

# Functionalities:

```
Word insertion
Display hash table
Display words starting with a specific letter and of a certain length
Display words that appear at most n times
```
# Code Explain:


The main structure of the hash table is maintained. In it, lists with nodes storing a TElem value are stored (based on a hash code related to the first letter of the word), representing the word's length and a sublist of words. The sublist of words consists of TCuv nodes,
each containing a string and a number of occurrences. Both the table and the lists are implemented generically.

After opening the input files, each line is read one by one, and the first word is compared with the two commands, "insert" and "print". In the case of insertion, the string is copied into a variable of type TCuv, initialized with a number of occurrences equal to 1. The logic of sorting the list in ascending order of occurrences is as follows: Before adding to the hash, the word is checked if it appears in any sublist by calling the FindValueInTable subroutine. If found in the sublist, the found node is deleted, its memory is freed, and the function returns the number of occurrences. Then, the number of occurrences is updated with the result of the function +1, and it is added in order to the hash table with the new number of occurrences. The same procedure is performed if the string does not appear, returning the value 0 and inserting it into the list with a number of occurrences equal to 1. This process was designed for a progressive update of the list. In the case where the given word is the head of the sublist, it should not change its position, so only its occurrence number is incremented directly in the function, and FindValueInTable returns -1, in which case the memory for the TCuv variable created for reading is deallocated in the main. Two subprograms are used for ordered insertion, one for inserting the sublist and one for inserting into the main list. Certain casts are used in the main list to introduce the sublist.

For printing, three different subprograms are used depending on the type. Printing the table is a regular one, while printing with the given letter and number traverses exactly that sublist, and the last one traverses the entire hash table in order and prints if the conditions are met.

Finally, memory deallocation is performed.
