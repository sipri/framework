#include <stdio.h>

int main()
{
    long long a, b, c;
    scanf("%lld %lld %lld", &a, &b, &c);

    long long x = a / c + ((a % c) ? 1 : 0);
    long long y = b / c + ((b % c) ? 1 : 0);

    printf("%lld", x * y);
    return 0;
}
