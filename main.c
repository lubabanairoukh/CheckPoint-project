

//include section
#include <stdio.h>

//define section
#define MAX_NAME 50
#define TEL_NUM 10
#define MAX_GRADES 10
#define MAX_COURSES 10
#define TABLE_SIZE 1000000
//struct section


typedef struct Student{
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    char tel_num[TEL_NUM];
    int grades[MAX_GRADES];
    float average;
    int class; 
    int layer;
} Student;

typedef struct Node{
    Student* student;
    struct Node* left;
    struct Node* right;
} Node;


typedef struct HashTableEntry {
    Student **students; // Dynamic array of student pointers
    int count;     // Number of students with the same name
    int capacity;        // Current capacity of the array
} HashTableEntry;

typedef struct HashTable {
    HashTableEntry *table; // Array of HashTableEntry
    int size;              // Number of entries in the table
} HashTable;

typedef struct AverageLayer {
    float average_by_course[MAX_COURSES];
    int num_of_students;
} AverageLayer;

int main() {
    
    
    return 0;
}