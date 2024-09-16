## Checkpoint


important to allocate and release resources properly.
we will be using C to allocate and release resources.

## use of valgrind

valgrind is a tool that helps in finding memory leaks in the code.
example:
```bash
valgrind --leak-check=full ./a.out
```

each student has 10 grades
each grade is in the same order (fr example english, math, science, etc)
we need to have some functions
add stud
remove stud
10 classes 12 "layers"
100k students from a file

we need to load them into our program

and create some menu to interact with the program

we would want to sort it, than we can find with binary search with is logn

for each choice of how we structe our db, we need to say why, what algorithms, what is the tradeoff

text structure:
name , telephone, שכבה , class, 10 grades 
the order is the same for every row


on the second step, when adding removing we will need to check the input of the user!
anything wrong should be error


searching, name and last name can be identical, so we need for the user to choose the right one he wants

finishing the program, we save the data to a file, and the next time we run the program we load the data from the file




------------------------------------------------------------
Our decisions on how to structure the data:
so first:
we will have a two dimensional array, with, school layers as index, where each school layers has 12 classes.

[layer1][class1][class2]...[class12]
[layer2][class1][class2]...[class12]
[layer3][class1][class2]...[class12]

each class, is a ptr to a binary search tree, per class, each Node in the tree, is a student.
the tree will be sorted, by the avg of grades of that student.

this will help us, search by avg grade, to find the best or worst studnets, in average time of logn.

the problem arises, when we want to search a student by name, in order solve it, we will have a hash table, that will have the name of the student as a key, and the value will be a ptr to that specific node, in that specifc tree.

student  name  = avi itzhak
hash(avi itzhak) = 1234
hash table[1234][0] = ptr to the node in the tree


if we have a student with the same name, we should create another ptr to the other node, 
Basicly, each cell in the hashmap array ,  will have another dynamic array of pointers to the nodes, that have the same name.

in order to get information like avg of layers, per course, and that kind of information, we will hold a static array, size of scholl layers
each cell will have another array, size of 10 (total courses) , and each cell will represent the avg of that course.
we will calculate all the avg after we finish loading, and update the avg dynamicly, after each update/delete/addition.
[layer1][avg_course1][avg_course2]...[avg_course10]
[layer2][avg_course1][avg_course2]...[avg_course10]
[layer3][avg_course1][avg_course2]...[avg_course10]


////////////////////////////////////////////////////////////
things to notice for step 1:
1. we need to load the data from the file.
2. before we load it, we need to call all our database structs, and also make sure, they dont have garbage in them. לאתחל אותם
3. lets say we have read from the file succsefully, we need to print all the list of the students, and send eitan a slack msg.
4. we also need to run a report on valgrind, and only than slack him.
5. before the report of valgrind, we need to create a deleting function, that will release all of the data we have allocated.
6. we need to make sure, that we dont have any memory leaks, and that we have freed all the memory we have allocated.


   // in C sscanf need a ptr to the adress
    // if we send an array (any) we dont need a & cus its already a ptr


compile with -g for valgrind:
```bash
gcc -g your_program.c -o your_program
```

run with valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all  --leak-resolution=high --trace-children=no --show-below-main=yes --num-callers=50 --log-file=memory_check.log ./your_program
```


```c
unsigned int hash(const char* str, int table_size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % table_size;
}

```
This function uses the djb2 algorithm by Dan Bernstein, which is a simple and fast hash function. It shifts the hash by 5 places to the left (equivalent to multiplying by 32) and then adds the hash itself again, effectively multiplying by 33, before adding the ASCII value of the current character.

we must remeber, that in average because of the hash function
we have around 100K students, and we have 103K cells in the hash table, so we have around 1 student per cell, so the search time is O(1) in average, and in worst case, we have 2 students per cell, so the search time is O(2) in worst case.
WORST CASE if all students names are the same, we will have 100K students in the same cell, so the search time will be O(100K) in worst case.


```c
typedef struct AverageLayer
{
    float average_by_course[MAX_COURSES];
    int num_of_students;
} AverageLayer;
```

this works the following way:
first when inserting a student, we will sum the grades, in the averagelayer[MAX_LAYERS] , we for loop for all the student courses and add all the grades to the average_by_course[allcourses] and we increment the num_of_students by 1.

when we finish enterting all the students, we need to divide the sum of the grades by the num of students, for every layer, for all courses