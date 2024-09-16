

// include section
#include <stdio.h>
#include <stdbool.h>
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

enum Selection {
    Insert = 0,
    Delete = 1,
    Edit = 2,
    Search = 3,
    Showall = 4,
    Top10 = 5,
    UnderperformedStudents = 6,
    Average = 7,
    Export = 8,
    Exit = 9
};
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
    double average_by_course[MAX_COURSES];
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

//avg layer
void init_avg_layers(AverageLayer average_layer[MAX_LAYERS], Student* stud);
void finish_init_avg(AverageLayer average_layer[MAX_LAYERS]);
void print_all_avg(AverageLayer average_layer[MAX_LAYERS]);
void clear_avg_layer(AverageLayer average_layer[MAX_LAYERS]);

void insert_node(Node **rootPtr, Student *student);
void print_single_tree(Node *root);
void print_school(Node *school[MAX_LAYERS][MAX_CLASSES]);
void delete_tree(Node* root);
void delete_function(Node* school[MAX_LAYERS][MAX_CLASSES]);
void delete_everything(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table);





// insert new student or edit a student:
void editStudent(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table, AverageLayer average_layer[MAX_LAYERS]);
int is_alpha(const char *str) ;
void capitalize_first_letter(char* str) ;
int is_digits(const char *str) ;
void insertNewStudent(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table,AverageLayer average_layer[MAX_LAYERS]) ;



// main section
int main()
{
    printf("before fopen\n");

    // init the datastructre
    Node *school[MAX_LAYERS][MAX_CLASSES];
    HashTable* hash_table = create_hash_table(100003);
    AverageLayer average_layer[MAX_LAYERS];
    init_program(school, hash_table, average_layer);
    //print_all_avg(average_layer);
   // print_school(school);
    //hash table/map 
    menu(school, hash_table, average_layer);
    
    //delete function
    //delete_everything(school, hash_table);
    
    return 0;
}
// functions section//////////////////////////////////

void init_program(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table, AverageLayer average_layer[MAX_LAYERS])
{
    clear_avg_layer(average_layer);
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
        init_avg_layers(average_layer, stud);
        insert_hash(hash_table, stud, index);
    }
    finish_init_avg(average_layer);
    fclose(file);
}
void menu(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table, AverageLayer average_layer[MAX_LAYERS]) {
	int input;
	do {
		printf("\n|School Manager<::>Home|\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("\t[0] |--> Insert\n");
		printf("\t[1] |--> Delete\n");
		printf("\t[2] |--> Edit\n");
		printf("\t[3] |--> Search\n");
		printf("\t[4] |--> Show All\n");
		printf("\t[5] |--> Top 10 students per course\n");
		printf("\t[6] |--> Underperformed students\n");
		printf("\t[7] |--> Average per course\n");
		printf("\t[8] |--> Export\n");
		printf("\t[9] |--> Exit\n");
		printf("\n\tPlease Enter Your Choice (0-9): ");
		if (scanf("%d", &input) != 1) {
			// Invalid input, clear the input buffer
			while (getchar() != '\n');
			printf("\nInvalid input. Please enter a number between 0 and 9.\n");
			continue;
		}
		switch (input) {
		case Insert:
            insertNewStudent(school,  hash_table, average_layer);
			break;
		// case Delete:
		// 	deleteStudent(school);
		// 	break;
		// case Edit:
		// 	editStudentGrade(school);
		// 	break;
		// case Search:
		// 	searchStudent(school);
		// 	break;
		// case Showall:
		// 	printAllStudents(school);
		// 	break;
		// case Top10:
		// 	printTopNStudentsPerCourse(school);
		// 	break;
		// case UnderperformedStudents:
		// 	printUnderperformedStudents(school, 15);
		// 	break;
		// case Average:
		// 	printAverage(school);
		// 	break;
		// case Export:
		// 	exportDatabase(school, "dataExport.txt");
		// 	break;
		// case Exit:
		// 	delete_everything(school,hash_table);
		// 	break;
		default:
			printf("\nThere is no item with symbol \"%d\". Please enter a number between 0 and 9!\n", input);
			break;
		}
		// Clear the input buffer
		while (getchar() != '\n');
	} while (input != Exit);
}




void clear_avg_layer(AverageLayer average_layer[MAX_LAYERS])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        average_layer[i].num_of_students = 0;
        for (int j = 0; j < MAX_COURSES; j++)
        {
            average_layer[i].average_by_course[j] = 0;
        }
    }
}

void print_all_avg(AverageLayer average_layer[MAX_LAYERS])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        printf("Layer %d\n", i+1);
        printf("Number of students: %d\n", average_layer[i].num_of_students);
        for (int j = 0; j < MAX_COURSES; j++)
        {
            printf("Course %d: %f\n", j+1, average_layer[i].average_by_course[j]);
        }
    }
}
void finish_init_avg(AverageLayer average_layer[MAX_LAYERS])
{
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        for (int j = 0; j < MAX_COURSES; j++)
        {
            average_layer[i].average_by_course[j] /= average_layer[i].num_of_students;
        }
    }
}
void init_avg_layers(AverageLayer average_layer[MAX_LAYERS], Student* stud)
{
    
    for (int i = 0; i < MAX_GRADES; i++)
    {
        
        average_layer[stud->layer-1].average_by_course[i] += stud->grades[i];
    }
    
    average_layer[stud->layer-1].num_of_students++;
 
}

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




///----------------------------------------------------



void insertNewStudent(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table,AverageLayer average_layer[MAX_LAYERS]) {
    // Allocate memory for the new student
    Student* new_student = (Student*)malloc(sizeof(Student));
    if (new_student == NULL) {
        perror("Error allocating memory for new student\n");
        return;
    }

    // Collect the student's details from the user
    printf("Enter first name: ");
    scanf("%s", new_student->first_name);
    if (strlen(new_student->first_name) == 0 || !is_alpha(new_student->first_name) ||strlen(new_student->first_name) >=MAX_NAME) {
        printf("It's incorrect input: first name should only contain letters and cannot be empty.\n");
        free(new_student);
        return;
    }


    printf("Enter last name: ");
    scanf("%s", new_student->last_name);
    if (strlen(new_student->last_name) == 0 || !is_alpha(new_student->last_name) || ||strlen(new_student->last_name) >=MAX_NAME) {
        printf("It's incorrect input: last name should only contain letters and cannot be empty.\n");
        free(new_student);
        return;
    }

    printf("Enter phone number (10 digits): ");
    scanf("%s", new_student->tel_num);
    if (strlen(new_student->tel_num) != 10 || !is_digits(new_student->tel_num)) {
        printf("It's incorrect input: phone number must be exactly 10 digits.\n");
        free(new_student);
        return;
    }

    printf("Enter the student's layer (1-12): ");
    scanf("%d", &new_student->layer);
     if (new_student->layer < 1 || new_student->layer > 12) {
        printf("It's incorrect input: layer must be a number between 1 and 12.\n");
        free(new_student);
        return;
    }

    printf("Enter the student's class (1-10): ");
    scanf("%d", &new_student->class);
    if (new_student->class < 1 || new_student->class > 10) {
        printf("It's incorrect input: class must be a number between 1 and 12.\n");
        free(new_student);
        return;
    }

    printf("Enter the student's grades (10 courses):\n");
    for (int i = 0; i < MAX_GRADES; i++) {
        printf("Grade for course %d: ", i + 1);
        scanf("%d", &new_student->grades[i]);
        if (new_student->grades[i] < 0 || new_student->grades[i] > 100) {
            printf("It's incorrect input: grades must be between 0 and 100.\n");
            free(new_student);
            return;
        }
    }

    // Calculate the student's average
    new_student->average = calculate_average(new_student);
    capitalize_first_letter(new_student->first_name);
    capitalize_first_letter(new_student->last_name);
    // Concatenate first and last name for hashing
    char full_name[FULL_NAME];
    concat_names(full_name, new_student->first_name, new_student->last_name);

    // Get the hash index for the full name
    unsigned int index = hash(full_name, hash_table->size);

    // Insert the student into the hash table at the appropriate index
    insert_hash(hash_table, new_student, index);

    // Insert the student into the school's binary search tree based on the layer and class
    insert_node(&school[new_student->layer - 1][new_student->class - 1], new_student);


    for(int i=0; i<MAX_COURSES;i++){
        update_avg_courses_single(average_layer,new_student,true,i);

    }
    average_layer[new_student->layer-1].num_of_students++;
    
    printf("Student %s %s successfully inserted.\n", new_student->first_name, new_student->last_name);
}


void capitalize_first_letter(char* str) {
    if (str[0] >= 'a' && str[0] <= 'z') {
        str[0] = str[0] - 'a' + 'A'; // Convert the first letter to uppercase
    }
}

int is_alpha(const char *str) {
    while (*str) {
        if (!isalpha(*str)) {
            return 0; // False if any character is not a letter
        }
        str++;
    }
    return 1; // True if all characters are letters
}

// Helper function to check if a string contains only digits
int is_digits(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0; 
        }
        str++;
    }
    return 1; // True if all characters are digits
}




void editStudent(Node* school[MAX_LAYERS][MAX_CLASSES], HashTable* hash_table, AverageLayer average_layer[MAX_LAYERS])
{
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];

    unsigned int index = search(hash_table, first_name, last_name);
    if (hash_table->table[index].count == 0) {
        printf("Error: Student not found.\n");
        return;
    }

    // Iterate through the list of students at the found index
    for (int i = 0; i < hash_table->table[index].count; i++) {
        printf("Searching for student at index: %d\n", index);

        // Compare the first and last names to find the exact match
        if (strcmp(hash_table->table[index].students[i]->first_name, first_name) == 0 &&
            strcmp(hash_table->table[index].students[i]->last_name, last_name) == 0) {

            printf("Success: Student found.\n");
            print_stud(hash_table->table[index].students[i]);

            // Confirm if the user wants to edit this student
            if (user_choice("Are you sure you want to edit this student? (y/n): ")) {
                // Get the student to update
                Student* student_to_update = hash_table->table[index].students[i];
                
                int choice;
                printf("\nWhat would you like to update?\n");
                printf("1. Phone Number\n");
                printf("2. Grade for a specific course\n");
                printf("Enter your choice (1 or 2): ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: {
                        // Update phone number
                        char new_phone[11];
                        printf("Enter the new phone number (10 digits): ");
                        scanf("%s", new_phone);
                        if (strlen(new_phone) != 10 || !is_digits(new_phone)) {
                            printf("Error: Invalid phone number. It must be exactly 10 digits.\n");
                            return;
                        }
                        strcpy(student_to_update->tel_num, new_phone);
                        printf("Success: Phone number updated successfully.\n");
                        break;
                    }
                    case 2: {
                        // Update grade for a specific course
                        int course_index, new_grade;
                        printf("Enter the course number (1-10): ");
                        scanf("%d", &course_index);
                        if (course_index < 1 || course_index > MAX_GRADES) {
                            printf("Error: Invalid course number. It must be between 1 and 10.\n");
                            return;
                        }
                        printf("Enter the new grade (0-100): ");
                        scanf("%d", &new_grade);
                        if (new_grade < 0 || new_grade > 100) {
                            printf("Error: Invalid grade. It must be between 0 and 100.\n");
                            return;
                        }
                        student_to_update->grades[course_index - 1] = new_grade;

                        // Recalculate the student's average after updating the grade
                        student_to_update->average = calculate_average(student_to_update);
                        //average
                        update_avg_courses_single(average_layer,student_to_update,false,course_index-1);
                        printf("Success: Grade updated successfully.\n");
                        break;
                    }
                    default:
                        printf("Error: Invalid choice. Please enter 1 or 2.\n");
                        break;
                }
    
                printf("\nSuccess: Student information updated successfully.\n");
                break;
            }
        }
    }
}
void update_avg_courses_single( AverageLayer average_layer[MAX_LAYERS],Student *student, bool new_student,int index){
    average_layer[student->layer-1].average_by_course[index]*= average_layer[student->layer-1].num_of_students;
    average_layer[student->layer-1].average_by_course[index]+=student->grades[index];
    if(new_student){
        average_layer[student->layer-1].average_by_course[index]/=average_layer[student->layer-1].num_of_students+1;
    }
    else{
        average_layer[student->layer-1].average_by_course[index]/=average_layer[student->layer-1].num_of_students;
    }
}