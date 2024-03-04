#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
    char text[10000],c;
    int n=0,state = 0,i,j,k;
    int w = -1, l =0,flag = 1,num_of_id = 0;
    while ((c = getchar()) != EOF) {
        text[n++] = c;
    }
    text[n] = '\n';
    i = 0;

    for (i = 0;i<n;i++){
        if ((isalnum(text[i])||text[i]=='_') && state==IN_ID){
            l++;
            identifiers[w][l] = text[i];
        }

        if ((isalpha(text[i]) || text[i] == '_') && state==0){
            l=0;
            w++;
            identifiers[w][l] = text[i];
            state = IN_ID;
        }

        if ((isalnum(text[i])==0&&text[i]!='_')&&state==IN_ID){
            state =0;
        }
        if (text[i-1]=='/'&&text[i]=='/'&&state!=IN_STRING&& state != IN_BLOCK_COMMENT&& state != IN_ID) {
            state = IN_LINE_COMMENT;
        }
        if ((text[i]=='\n')&&state==IN_LINE_COMMENT){
            state = 0;

        }
        if (text[i-1]=='/'&&text[i]=='*'&&state!=IN_STRING&& state != IN_LINE_COMMENT&& state != IN_ID) {
            state = IN_BLOCK_COMMENT;
        }
        if (text[i-1]=='*'&&text[i]=='/'&&state==IN_BLOCK_COMMENT) {
            state = 0;
        }
        if ((text[i]=='\"'||text[i]=='\'')&& state==IN_STRING&&text[i-1]!=(char)(92)) {
            state = 0;
        }
        else if ((text[i]=='\"'||text[i]=='\'')&&state!=IN_STRING&& state != IN_LINE_COMMENT&& state != IN_ID &&state!=IN_BLOCK_COMMENT) {
            state = IN_STRING;
        }


    }
    for (i = 0; i<=w; i++ ){
        flag = 1;
        j = 0;
        while(j<32&&flag==1){
            if (strcmp(identifiers[i],keywords[j])==0){
                flag = 0;
            }
            j++;
        }
        k = i-1;
        while(k>=0&&flag==1){
            if(strcmp(identifiers[i],identifiers[k])==0){
                flag=0;
            }
            k--;
        }
        if (flag==1){
            num_of_id++;
//            printf("%s",identifiers[i]);
//            printf("\n");
        }
    }


    return num_of_id;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

