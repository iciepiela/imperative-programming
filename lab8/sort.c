#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
    const Date *da = (Date*)d1, *db =(Date*)d2;
    if ((*da).year == (*db).year){
        if ((*da).month==(*db).month){
            if ((*da).day == (*db).day){
                return 0;
            }
            if ((*da).day<(*db).day){
                return -1;
            }
            else{
                return 1;
            }
        }
        if ((*da).month<(*db).month){
            return -1;
        }
        else{
            return 1;
        }
    }
    if ((*da).year < (*db).year){
        return -1;
    }
    else{
        return 1;
    }
}

// compare foods 0 - same food, 1 - after, -1 - before
int cmp(const void *a, const void *b) {
    Food *pa = (Food*)a, *pb =(Food*)b;
    if ((*pa).price==(*pb).price){
        if (cmp_date(&(*pa).valid_date,&(*pb).valid_date)==0){
            if (strcmp((*pa).name,(*pb).name)==0){
                return 0;
            }
            else{
                return 1;
            }
        }
        if (cmp_date(&(*pa).valid_date,&(*pb).valid_date)==-1){
            return -1;
        }
        else{
            return 1;
        }
    }
    if((*pa).price<(*pb).price){
        return -1;
    }
    else{
        return 1;
    }

}


// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, int *result) {
    char *p = (char *) base;
    int low = 0, high = nmemb - 1, mid;

    int comp;
    if (nmemb== 0){
        *result = 0;
        return (void*) p;
    }
    while (low <= high) {
        mid = (low + high) / 2;
        comp = compar((const void *)key, (const void *) (p + mid * size));
        if (comp == 0) {
            *result = 1;
            return (void *) (p + mid * size);
        } else if (comp ==-1) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    // element not found
    *result = 0;
    return (void *) (p + low * size);
}


// print goods of given name
void print_art(Food *p, int n, char *art) {
    for (int i = 0; i<n;i++){
        if (strcmp(p[i].name,art)==0){
            printf("%.2f %d %02d.%02d.%d\n",p[i].price,p[i].amount,p[i].valid_date.day,
                   p[i].valid_date.month,p[i].valid_date.year);
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    int result;

    Food *pos = (Food*) bsearch2((void*)new,(void*)tab,*np,sizeof(tab[0]),compar,&result);

    if (result == 1){
        (*pos).amount+=(*new).amount;
    }
    else{
        size_t index = pos - tab;
        for (size_t i = *np; i > index; i--) {
            tab[i] = tab[i - 1];
        }
        tab[index] = *new;

        (*np)++;

    }
    return tab;
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    Food new_food;
    int np = 0;
    for (int i = 0;i<no;i++){
        if(sorted == 1) {
            fscanf(stream, " %20s %f %d %d.%d.%d", new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day,
                   &new_food.valid_date.month, &new_food.valid_date.year);
            tab = add_record(tab,&np,cmp,&new_food);
        }
        else{
            fscanf(stream, " %20s %f %d %d.%d.%d", new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day,
                   &new_food.valid_date.month, &new_food.valid_date.year);
            tab[i]= new_food;
            (np)++;
        }
    }
    return np;

}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    qsort(food_tab,n,sizeof (food_tab[0]),cmp_qs);
    int result;
    Food* pos = (Food*) bsearch2(&curr_date,food_tab,n,
                                 sizeof (food_tab[0]), cmp_bs,&result);

    struct tm date = {0};
    Date new_date;
    date.tm_year = curr_date.year - 1900;
    date.tm_mon = curr_date.month - 1;
    date.tm_mday = curr_date.day;

    time_t tym = mktime(&date);

    tym += days * 24 * 60 * 60;

    struct tm date2 = *localtime(&tym);

    new_date.year = date2.tm_year +1900;
    new_date.month = date2.tm_mon +1;
    new_date.day = date2.tm_mday;


    float value = 0;
    int cmp = 0;
    size_t i = pos - food_tab;
    while (cmp>=0&&i < n) {
        cmp = cmp_bs(&new_date,&food_tab[i]);
        if (cmp==0){
            value+=food_tab[i].price*food_tab[i].amount;
        }
        i++;
    }
    return value;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
    int child_no;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(const void *a, const void *b) {
    Person *person1 = (Person*)a, *person2 = (Person*)b;
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

int cmp_order(const void *a, const void *b) {
    Person person1 = *(Person *) a, person2 = *(Person *) b;
    if (cmp_date(&person1.born, &primo_date) < 0 && cmp_date(&person2.born, &primo_date) < 0) {
        if (person1.bits.sex == F && person2.bits.sex == M) return 1;
        else if (person1.bits.sex == M && person2.bits.sex == F) return -1;
    }
    return cmp_date(&person1.born, &person2.born);
}

// compare persons
int cmp_person(const void *a, const void *b) {
    Person pa = *(Person *)a, pb = *(Person *)b;
    if(pa.parent==NULL) return -1;
    else if(pb.parent==NULL) return 1;

    return strcmp(pa.parent,pb.parent);
}
int cmp_parent(const void *a, const void *b) {
    Parent pa = *(Parent *)a, pb = *(Parent *)b;
    return strcmp(pa.par_name,pb.par_name);
}
int cmp_parent_name(const void *a, const void *b) {
    char *key_name = (char*)a;
    Parent pb = *(Parent *)b;
    return strcmp(key_name,pb.par_name);
}


//int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
//
//}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
    int i = 0,index,child_no;
    int j;
    Person children[CHILD_MAX];
    Parent* pos;
    char *curr_name;
    while (i<size){
        curr_name = person_tab[i].name;
        pos = (Parent*) bsearch(curr_name,idx_tab,(size_t)no_parents,
                                 sizeof(idx_tab[0]),cmp_parent_name);
        if (pos !=NULL){
            index = (*pos).index;
            child_no = (*pos).child_no;

            memmove(children,&person_tab[index],sizeof(person_tab[0])*child_no);
            memmove(&person_tab[i+child_no+1],&person_tab[i+1],sizeof (person_tab[0])*(index-i-1));
            memmove(&person_tab[i+1],children,sizeof(person_tab[0])*child_no);
            j = pos - idx_tab;

            while (j>=0){
                idx_tab[j].index+=child_no;
                j--;
            }
        }
        i++;


    }
}

int cleaning(Person *person_tab, int n) {
    int num = n,i=0;
    while (i<num){
        if (person_tab[i].bits.in_line==no){
            memmove(&person_tab[i],&person_tab[i+1],sizeof (person_tab[0])*(num-1-i));
            num--;
        }
        else i++;
    }
    return num;
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
    Parent inx_tab[n];
    int parent_no = 0;
    qsort(person_tab,n,sizeof(person_tab[0]),cmp_person);

    int child_no = 0;
    for (int i = 1;i<n;i++) {
        child_no++;
        if (i==n-1 || strcmp(person_tab[i].parent, person_tab[i + 1].parent) != 0) {
            qsort(&person_tab[i-child_no+1],child_no,
                  sizeof (person_tab[0]), cmp_order);

            inx_tab[parent_no].par_name = person_tab[i].parent;
            inx_tab[parent_no].child_no = child_no;
            inx_tab[parent_no].index = i-child_no+1;
            parent_no++;

            child_no = 0;
        }
    }

    qsort(inx_tab,parent_no,sizeof (inx_tab[0]),cmp_parent);
    persons_shifts(person_tab,n,inx_tab,parent_no);

    return cleaning(person_tab,n);
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
            create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
