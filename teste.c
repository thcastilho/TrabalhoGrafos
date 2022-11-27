#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* fp = fopen("grafoAdjG.txt", "r+");

    int a, b, d;
    char c;

    fscanf(fp, "%d %d %c %d", &a, &b, &c, &d);
    printf("%d %d %c %d", a, b, c, d);

    fclose(fp);
}