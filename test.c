#include <stdio.h>

void f2str(float n, char *a){
    n *= 100;
    int nn = n > 0 ? (int)n : ((int)n) * (-1);
    int i = 10;
    while (nn > 0){
        a[i--] = nn % 10 + 48;
        nn /= 10;
        if (i == 8)
            a[i--] = '.';
    }
    if (i == 7) a[i--] = 48;
    if (n < 0)  a[i--] = '-';
    while (nn <= 10)
        a[nn++] = i < 10 ? a[++i] : 48;
}

// int main(){
//     float x = 213647.62;   
//     scanf("%f", &x);
//     char buf[12];

//     f2str(x, buf);

//     printf("buffer is: %s\n", buf);

//     return 0;
// }

#include <stdlib.h>

char* floatToString(float number) {
    char buffer[20];
    sprintf(buffer, "%f", number);
    printf("%s\n", buffer);
}   

int main(){
	int i = 0;
	while (1){
		if (0x1){
			break;
			i++;
		}
	}
	printf("i = %d\n", i);
    return 0;
}
