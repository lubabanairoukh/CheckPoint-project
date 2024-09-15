

// include section
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
// define section
#define MAX_NAME 50
#define TEL_NUM 11
#define MAX_GRADES 10
#define MAX_COURSES 10

#define MAX_LAYERS 12
#define MAX_CLASSES 10
#define FULL_NAME (MAX_NAME * 2 +2)
// struct section

//the struct to hold the student data
typedef struct Student
{
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    char tel_num[TEL_NUM];
    int grades[MAX_GRADES];
    float average;
    int class;
    int layer;
} Student;

//struct Node to hold the student and the left and right pointers
typedef struct Node
{
    Student *student;
    struct Node *left;
    struct Node *right;
} Node;

//hash table single entry struct
// count is current number of students with the same name
// capacity is the current capacity of the array
typedef struct HashTableEntry
{
    Student **students; // Dynamic array of student pointers
    int count;          // Number of students with the same name
    int capacity;       // Current capacity of the array
} HashTableEntry;

//hash table struct
// table is the array of HashTableEntry
// size is the number of entries in the table
typedef struct HashTable
{
    HashTableEntry *table; // Array of HashTableEntry
    int size;              // Number of entries in the table
} HashTable;

//struct to hold the average of the students per course of a layer
typedef struct AverageLayer
{
    float average_by_course[MAX_COURSES];
    int num_of_students;
} AverageLayer;


// prototypes section
//hash functions:
void test_hash(HashTable* table, const char* name);
HashTable* create_hash_table(int size);
void delete_hash(HashTable* hash_table);
void insert_hash(HashTable* table,Student* stud,unsigned int index);
unsigned int hash(const char* str, int table_size);
void concat_names(char* full_name, char* first_name, char* last_name);


float calculate_average(Student *student);

void init_school(Node *school[MAX_LAYERS][MAX_CLASSES]);
void print_stud(Student *student);
Student *create_student(char *line);


void insert_node(Node **rootPtr, Student *student);
void print_single_tree(Node *root);
void print_school(Node *school[MAX_LAYERS][MAX_CLASSES]);
void delete_tree(Node* root);
void delete_function(Node* school[MAX_LAYERS][MAX_CLASSES]);
void delete_everything(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table);


// main section
int main()
{
    printf("before fopen\n");

    // init the datastructre
    Node *school[MAX_LAYERS][MAX_CLASSES];
    HashTable* hash_table = create_hash_table(100003);
    init_school(school);

    FILE *file = fopen("students_with_class.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file\n");
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        Student *stud = create_student(line);
        //-1 cus range is 0- 11 (layers) 0-9 (classes)
        char full_name[FULL_NAME];
        concat_names(full_name, stud->first_name, stud->last_name);
        unsigned int index = hash(full_name, hash_table->size);
        insert_node(&school[stud->layer-1][stud->class-1], stud);
        insert_hash(hash_table, stud, index);
    }
   // print_school(school);
    //hash table/map 

    test_hash(hash_table, "Jennette Fehr");
    //delete function
    delete_everything(school, hash_table);
    fclose(file);
    return 0;
}
// functions section//////////////////////////////////
void test_hash(HashTable* table, const char* name)
{
    printf("test hash with name: %s\n", name);
    unsigned int index = hash(name, table->size);
    printf("index: %d\n", index);
    for (int i = 0; i < table->table[index].count; i++)
    {
        print_stud(table->table[index].students[i]);
    }
}

void insert_hash(HashTable* table,Student* stud,unsigned int index)
{
    if(table->table[index].count == 0)
    {
        table->table[index].students = (Student**)malloc(sizeof(Student*));
        if (table->table[index].students == NULL)
        {
            perror("Error allocating memory\n");
            return;
        }
        table->table[index].students[0] = stud;
        table->table[index].count++;
        table->table[index].capacity++;
    }
    else
    {
        if(table->table[index].count == table->table[index].capacity)
        {
            table->table[index].students = (Student**)realloc(table->table[index].students, sizeof(Student*) * table->table[index].capacity * 2);
            if (table->table[index].students == NULL)
            {
                perror("Error allocating memory\n");
                return;
            }
            for (int i = table->table[index].count;i<table->table[index].capacity * 2;i++)
            {
                table->table[index].students[i] = NULL;
            }
            table->table[index].capacity *= 2;
        }
        table->table[index].students[table->table[index].count] = stud;
        table->table[index].count++;
    }
}

void concat_names(char* full_name, char* first_name, char* last_name)
{
    memset(full_name, 0, FULL_NAME);

    strcpy(full_name, first_name);

    strcat(full_name, " ");
    strcat(full_name, last_name);
}


void delete_everything(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table)
{
    delete_function(school);
    delete_hash(hash_table);
}
HashTable* create_hash_table(int size)
{
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    if (hash_table == NULL)
    {
        perror("Error allocating memory\n");
        return NULL;
    }
    hash_table->table = (HashTableEntry*)malloc(size * sizeof(HashTableEntry));
    if (hash_table->table == NULL)
    {
        perror("Error allocating memory\n");
        free(hash_table);
        return NULL;
    }
    hash_table->size = size;
    for (int i = 0; i < size; i++)
    {
        hash_table->table[i].students = NULL;
        hash_table->table[i].count = 0;
        hash_table->table[i].capacity = 0;
    }
    return hash_table;
}

void delete_hash(HashTable* hash_table)
{
    for (int i = 0; i < hash_table->size; i++)
    {
        free(hash_table->table[i].students);
    }
    
    free(hash_table->table);
    free(hash_table);
}
// Function to delete the entire school structure
void delete_function(Node* school[MAX_LAYERS][MAX_CLASSES])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        for (int j = 0; j < MAX_CLASSES; j++)
        {
            delete_tree(school[i][j]);
        }
    }
}

// Function to delete a binary tree
void delete_tree(Node* root)
{
    if (root == NULL)
    {
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    free(root->student);
    free(root);
}

// Function to print the entire school structure
void print_school(Node *school[MAX_LAYERS][MAX_CLASSES])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        for (int j = 0; j < MAX_CLASSES; j++)
        {
            printf("Layer %d, Class %d\n", i, j);
            print_single_tree(school[i][j]);
        }
    }
}

// Function to print a single binary tree (in-order traversal)
void print_single_tree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    
    print_single_tree(root->left);
    print_stud(root->student);
    print_single_tree(root->right);
}

// Function to print a single student's information
void print_stud(Student *student)
{
    printf("%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d student avg: %f\n", student->first_name, student->last_name, student->tel_num, student->layer, student->class,
           student->grades[0], student->grades[1], student->grades[2], student->grades[3], student->grades[4], student->grades[5],
           student->grades[6], student->grades[7], student->grades[8], student->grades[9], student->average);
}

// Function to create a student from a line of text
Student *create_student(char *line)
{
    Student *student = (Student *)malloc(sizeof(Student));
    if (student == NULL)
    {
        perror("Error allocating memory\n");
        return NULL;
    }
    sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
           student->first_name, student->last_name, student->tel_num, &student->layer, &student->class,
           &student->grades[0], &student->grades[1], &student->grades[2], &student->grades[3], &student->grades[4], &student->grades[5],
           &student->grades[6], &student->grades[7], &student->grades[8], &student->grades[9]);
    student->average = calculate_average(student);
   // print_stud(student);
    return student;
}

// Function to insert a student into a binary tree
void insert_node(Node **rootPtr, Student *student)
{
    // Check if the tree node pointer itself is NULL
    if (*rootPtr == NULL)
    {
        *rootPtr = (Node *)malloc(sizeof(Node));
        if (*rootPtr == NULL)
        {
            perror("Error allocating memory\n");
            return;
        }
        (*rootPtr)->student = student; // Assigning student to the student field of the new node
        (*rootPtr)->left = NULL;
        (*rootPtr)->right = NULL;
    }
    else if ((*rootPtr)->student->average > student->average)
    {
        insert_node(&((*rootPtr)->left), student); // Recursive call for the left subtree
    }
    else
    {
        insert_node(&((*rootPtr)->right), student); // Recursive call for the right subtree
    }
}

// Function to initialize the school structure
void init_school(Node *school[MAX_LAYERS][MAX_CLASSES])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        for (int j = 0; j < MAX_CLASSES; j++)
        {
            school[i][j] = NULL;
        }
    }
}

// Function to calculate the average grade of a student
float calculate_average(Student *student)
{
    int sum = 0;

    for (int i = 0; i < MAX_GRADES; i++)
    {
        // Assuming -1 indicates no grade
        sum += student->grades[i];
    }
    //printf("sum in calc avg: %d\n", sum);
    return (float)sum / MAX_GRADES;
}

unsigned int hash(const char* str, int table_size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
       
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
        
    }
    
    return hash % table_size;
}