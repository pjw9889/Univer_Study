/*name print syscall*/
#include <stdio.h>
#include <syscall.h>

int sys_name_print(char * user1)
{
	printf("%s\n", user1);
	return 0;
}

int main() {

	char * user1;
	printf("input name :");
	scanf("%s", user1);

	sys_name_print(user1);
}