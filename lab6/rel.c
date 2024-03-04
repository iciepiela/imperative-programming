#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair *relation, int n){
    int flag = 0;
    for(int i = 0;i<n;i++){
        flag = 0;
        for(int j=0;j<n;j++){
            if(relation[i].first== relation[j].first){
                if (relation[j].first==relation[j].second){
                    flag = 1;
                }
            }
        }
        if(flag ==0){
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair *relation, int n){
    for(int i = 0;i<n;i++){
        for(int j=0;j<n;j++){
            if(relation[i].first== relation[j].first && relation[j].first==relation[j].second){
                return 0;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair *relation, int n){
    int flag = 0;
    for(int i = 0;i<n;i++){
        flag = 0;
        for(int j=0;j<n;j++){
            if(relation[i].first== relation[j].second && relation[j].first==relation[i].second){
                flag = 1;
            }
        }
        if(flag == 0){
            return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair *relation, int n){
    for(int i = 0;i<n;i++){
        for(int j=0;j<n;j++){
            if(relation[i].first== relation[j].second){
                if (relation[j].first==relation[i].second){
                    if(relation[i].first != relation[i].second){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair *relation, int n){
    int flag = 0;
    for(int i = 0;i<n;i++){
        for(int j=0;j<n;j++){
            if(relation[i].first== relation[j].second && relation[j].first==relation[i].second){
                return 0;
            }
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair *relation, int n){
    int flag = 1;
    for(int i = 0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k = 0;k<n;k++){
                if(relation[i].second == relation[j].first){
                    flag = 0;
                    if (relation[i].first==relation[k].first && relation[j].second==relation[k].second){
                        flag = 1;
                        break;
                    }
                }
            }
            if(flag == 0){
                return 0;
            }
        }
    }
    return 1;
}
//
// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair *relation, int n){
    if(is_reflexive(relation,n)==1 && is_antisymmetric(relation,n)==1 && is_transitive(relation,n)==1){
        return 1;
    }
    return 0;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair *relation, int n){
    int X[MAX_RANGE], i,j,k,flag = 1;
    for (i = 0;i<MAX_RANGE;i++){
        X[i]=0;
    }
    for (i = 0;i<n;i++){
        X[relation[i].first]++;
        X[relation[i].second]++;
    }
    for (i = 0;i<n;i++){
        for (j=0;j<n;j++){
            if(X[i]>0 && X[j]>0){
                flag = 0;
                for (k = 0;k<n;k++){
                    if ((relation[k].first==i && relation[k].second==j) ||
                        (relation[k].first==j && relation[k].second==i)){
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair *relation, int n){
    if(is_partial_order(relation,n)==1 && is_connected(relation,n)==1){
        return 1;
    }
    return 0;
}

int get_domain(const pair *relation, int n, int *domain){
    int X[MAX_RANGE], i,j=0;
    for (i = 0;i<MAX_RANGE;i++){
        X[i]=0;
    }
    for (i = 0;i<n;i++){
        X[relation[i].first]++;
        X[relation[i].second]++;
    }
    for (i = 0; i<MAX_RANGE;i++){
        if(X[i]>0){
            domain[j]=i;
            j++;
        }
    }
    return j;
}

int find_max_elements(const pair *relation, int n, int *max_elements){
    int X[MAX_REL_SIZE],i,j,flag,con = 0;
    int X_size = get_domain(relation, n, X);
    for (i = 0;i<X_size;i++){
        flag = 1;
        for(j = 0;j<n;j++){
            if (X[i]==relation[j].first && X[i]!=relation[j].second){
                flag = 0;
            }
        }
        if (flag == 1){
            max_elements[con] = X[i];
            con++;
        }
    }
    return con;
}
int find_min_elements(const pair *relation, int n, int *min_elements){
    int X[MAX_REL_SIZE],i,j,flag,con = 0;
    int X_size = get_domain(relation, n, X);
    for (i = 0;i<X_size;i++){
        flag = 1;
        for(j = 0;j<n;j++){
            if (X[i]==relation[j].second && X[i]!=relation[j].first){
                flag = 0;
            }
        }
        if (flag == 1){
            min_elements[con] = X[i];
            con++;
        }
    }
    return con;
}


// Case 3:

int composition (const pair *relation, int n, const pair *relation_2, int n_2, pair *comp_relation){
    int size_comp = 0,i,j;
    pair new_pair;

    for(i = 0;i<n;i++){
        for (j = 0;j<n_2;j++){
            if(relation[i].second==relation_2[j].first){
                new_pair.first = relation[i].first;
                new_pair.second = relation_2[j].second;
                size_comp = add_relation(comp_relation,size_comp,new_pair);
            }
        }
    }

    return size_comp;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    return 0;
}

int insert_int (int *tab, int n, int new_element) {
    return 0;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    for (int i = 0;i<n;i++){
        if (tab[i].first==new_pair.first){
            if (tab[i].second==new_pair.second){
                return n;
            }
        }
    }
    tab[n]=new_pair;
    n++;
    return n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n,j=0;
    pair new_pair;
    scanf("%d",&n);
    for (int i = 0;i<n;i++){
        scanf("%d %d",&new_pair.first,&new_pair.second);
        j = add_relation(relation,j,new_pair);
    }
    return j;
}

void print_int_array(const int *array, int n) {
    printf("%d\n",n);
    for (int i = 0;i<n;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

