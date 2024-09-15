

//include section
#include <stdio.h>
#include <stdlib.h>

//define section
#define MAX_NAME 50
#define TEL_NUM 11
#define MAX_GRADES 10
#define MAX_COURSES 10
#define TABLE_SIZE 1000000
#define MAX_LAYERS 12
#define MAX_CLASSES 10

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

//prototypes section
float calculate_average(Student *student) ;
void init_school(Node* school[MAX_LAYERS][MAX_CLASSES]);
void print_stud(Student* student);
Student* create_student(char* line);
void insert_node(Node** rootPtr, Student* student);
void print_single_tree(Node* root);

int main() {
    printf("before fopen\n");

    //init the datastructre
    Node* school[MAX_LAYERS][MAX_CLASSES];
    init_school(school);


    FILE *file = fopen("students_with_class.txt", "r");
    if(file == NULL){
        perror("Error opening file\n");
        return 1;
    }
   
    char line[1024];
    fgets(line,sizeof(line),file);
    Student* stud = create_student(line);
    insert_node(&school[stud->layer][stud->class],stud);
    print_single_tree(school[stud->layer][stud->class]);
   

 
   
    fclose(file);
    return 0;
}
void print_single_tree(Node* root)
//print in order
{
    if (root == NULL) {
        return;
    }
    printf("in print_single_tree\n");
    print_single_tree(root->left);
    print_stud(root->student);
    print_single_tree(root->right);
}

void print_stud(Student* student)
{
     printf("%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d student avg: %f\n",student->first_name,student->last_name,student->tel_num,student->layer,student->class,
    student->grades[0],student->grades[1],student->grades[2],student->grades[3],student->grades[4],student->grades[5],
    student->grades[6],student->grades[7],student->grades[8],student->grades[9],student->average);
}

Student* create_student(char* line)
{
    Student* student = (Student*)malloc(sizeof(Student));
    if (student == NULL) {
        perror("Error allocating memory\n");
        return NULL;
    }
    sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
        student->first_name, student->last_name, student->tel_num, &student->layer, &student->class,
        &student->grades[0], &student->grades[1], &student->grades[2], &student->grades[3], &student->grades[4], &student->grades[5],
        &student->grades[6], &student->grades[7], &student->grades[8], &student->grades[9]);
    student->average = calculate_average(student);
    print_stud(student);
    return student;

}

void insert_node(Node** rootPtr, Student* student) {
    // Check if the tree node pointer itself is NULL
    if (*rootPtr == NULL) {
        *rootPtr = (Node*)malloc(sizeof(Node));
        if (*rootPtr == NULL) {
            perror("Error allocating memory\n");
            return;
        }
        (*rootPtr)->student = student; // Assigning student to the student field of the new node
        (*rootPtr)->left = NULL;
        (*rootPtr)->right = NULL;
    } else if ((*rootPtr)->student->average > student->average) {
        insert_node(&((*rootPtr)->left), student); // Recursive call for the left subtree
    } else {
        insert_node(&((*rootPtr)->right), student); // Recursive call for the right subtree
    }
}


void init_school(Node* school[MAX_LAYERS][MAX_CLASSES]) {
    for (int i = 0; i < MAX_LAYERS; i++) {
        for (int j = 0; j < MAX_CLASSES; j++) {
            school[i][j] = NULL;
        }
    }
}

float calculate_average(Student *student) {
    int sum = 0;
   
    for (int i = 0; i < MAX_GRADES; i++) {
        // Assuming -1 indicates no grade
            sum += student->grades[i];
           
        
    }
     printf("sum in calc avg: %d\n",sum);
    return (float)sum / MAX_GRADES;
}