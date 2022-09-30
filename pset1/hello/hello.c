#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // greetings to user
    string name = get_string("What's your name?\n");
    printf("hello, %s\n", name);
}