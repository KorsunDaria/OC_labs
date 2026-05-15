#include "stdio.h"
#include "stdlib.h"
#include <string.h>


struct Student {
    char name[50];
    int gpa;
    int age;
};

int main(){
    struct Student group[3];
    char *pattern[] = {"name", "age", "gpa"};
    for (int i=0; i<3;i++){
        printf("Enter %s: ", pattern[i]);
        scanf("%s", group[0].name);
    }
    int sum;
    for(int i=0; i<3; i++){
        sum+=group[i].gpa;
    }
    printf("Average GPA: %d", sum/3);

    int max=group[0].age;;
    int name_max = 0;
    for (int i=0; i<3; i++){
        if (group[i].age>max){
            max=group[i].age;
            name_max=i;
        }
    }
    printf("Oldest student: %s", group[name_max].name);

}