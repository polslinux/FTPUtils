#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

int main(){

char *host, *user, *pass;

host = malloc(64 * sizeof(char)); /* space for 64 chars */
if(host == NULL){
	printf("\n--> ERRORE: memoria non allocata\n");
	return EXIT_FAILURE;
}

user = malloc(64 * sizeof(char)); /* space for 64 chars */
if(user == NULL){
	printf("\n--> ERRORE: memoria non allocata\n");
	return EXIT_FAILURE;
}

pass = malloc(64 * sizeof(char)); /* space for 64 chars */
if(pass == NULL){
	printf("\n--> ERRORE: memoria non allocata\n");
	return EXIT_FAILURE;
}

puts("--> Inserisci <hostname username>:");
scanf("%s %s", host,user);
pass = getpass("--> Inserisci password: ");

printf("Host: %s\nUser: %s\nPass: %s\n", host,user,pass);

free(host);
free(user);
free(pass);

return EXIT_SUCCESS;
}