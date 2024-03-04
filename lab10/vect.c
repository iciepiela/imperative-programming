#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->element_size = element_size;
    vector->capacity = block_size;
    vector->size = 0;
    vector->data = malloc((vector->capacity)*(vector->element_size));
}


// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if (new_capacity > vector->capacity)
    {
        void *new_data = realloc(vector->data, vector->element_size*new_capacity);
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    size_t new_capacity = vector->capacity * 2 ;

    while (new_size > vector->capacity){
        reserve(vector, new_capacity);
    }
    void *prev_end = (char *)vector->data + vector->element_size*vector->size;
    memset(prev_end, 0, vector->element_size*(vector->capacity-vector->size));
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    size_t prev_size = vector->size;
    resize(vector,vector->size+1);
    void* position =vector->data+(prev_size*vector->element_size);
    memcpy(position,value,vector->element_size);
}

// Remove all elements from the vector
void clear(Vector *vector) {
    free(vector->data);
    vector->capacity = 0;
    vector->size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    resize(vector,vector->size+1);
    size_t prev_size = vector->size;
    void* ins_pos = (char*)vector->data+vector->element_size*index;
    void* move_pos = (char*)ins_pos+vector->element_size;
    memmove(move_pos,ins_pos,vector->element_size*(prev_size-index-1));
    memcpy(ins_pos,value,vector->element_size);
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
    void* del_pos = (char*)vector->data+vector->element_size*(index+1);
    void* move_pos = (char*)del_pos-vector->element_size;
    memmove(move_pos,del_pos,vector->element_size*(vector->size-index-1));
    vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    char *start = vector->data;
    size_t i = 0,j = 0;
    while(i<vector->size){
        if (!cmp(value,start+j)){
            erase(vector,i);
        }
        else{
            i++;
            j+=vector->element_size;
        }

    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    char *start = vector->data;
    size_t i = 0,j = 0;
    while(i<vector->size){
        if (predicate(start+j)){
            erase(vector,i);
        }
        else{
            i++;
            j+=vector->element_size;
        }

    }

}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    vector->data = realloc(vector->data,vector->element_size*vector->size);
    vector->capacity = vector->size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    const int *pv1 = v1, *pv2 = v2;
    if (*pv1<*pv2) return -1;
    if (*pv1>*pv2) return 1;
    else return 0;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    const char *c1 = v1,*c2 = v2;
    if (*c1<*c2) return -1;
    if (*c1>*c2) return 1;
    else return 0;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    const Person *per1 = p1, *per2 = p2;
    int age = per2->age-per1->age;
    if (age!=0) return age;
    int first_name = strcmp(per1->first_name,per2->first_name);
    if (first_name!=0) return first_name;
    int last_name = strcmp(per1->last_name,per2->last_name);
    return last_name;
}

// predicate: check if number is even
int is_even(void *value) {
    int val = *((int*)value);
    return val%2==0;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char c = *((char*)value);
    return (c=='a' || c=='e' || c=='u' || c=='i' || c=='o' || c=='y' || c=='A' || c=='E' || c=='U' || c=='I' || c=='O' || c=='Y');
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person *per = person;
    return per->age>25;
}

// print integer value
void print_int(const void *v) {
    int *val = (int*)v;
    printf("%d ",*val);
}

// print char value
void print_char(const void *v) {
    char *c = (char*)v;
    printf("%c ",*c);
}

// print structure Person
void print_person(const void *v) {
    const Person *pv = v;
    printf("%d %s %s\n", pv->age, pv->first_name, pv->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%zu\n",vector->capacity);
    char* start = vector->data;
    for(size_t i = 0, j = 0;i < vector->size;i++,j+=vector->element_size){
        print(start+j);
    }
}

// read int value
void read_int(void* value) {
    scanf(" %d", (int *)value);
}

// read char value
void read_char(void* value) {
    scanf(" %c", (char *)value);
}

// read struct Person
void read_person(void* value) {
    Person *p = value;
    scanf("%d %s %s", &p->age, p->first_name, p->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
//        print_vector(vector,print_int);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

