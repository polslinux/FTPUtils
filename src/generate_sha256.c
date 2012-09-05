/* Descrizione: Genera un hash data una stringa utilizzando 50000 volte sha256
 * Sviluppatore: Paolo Stivanin
 * Versione: 1.0-beta1
 * Copyright: 2012
 * Licenza: GNU GPL v3 <http://www.gnu.org/licenses/gpl-3.0.html>
 * Sito web: <https://github.com/polslinux/FTPUtils>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

int main(void){
	SHA256_CTX context;
 	unsigned char md[SHA256_DIGEST_LENGTH];
 	char *input = NULL, *compare = NULL;
 	static char hashed[65];
 	int i, n=1;
 	printf("Password: ");
 	if(scanf("%m[^\n]%*c", &input) == EOF){
 		perror("Scanf input");
 		return EXIT_FAILURE;
 	}
 	printf("Retype password: ");
 	if(scanf("%m[^\n]%*c", &compare) == EOF){
 		perror("Scanf compare");
 		return EXIT_FAILURE;
 	}
 	if(strcmp(input, compare) != 0){
 		printf("Password doesn't match\n");
 		free(input);
 		free(compare);
 		return EXIT_FAILURE;
 	}
 	size_t length = strlen((const char*)input);
 	SHA256_Init(&context);
 	SHA256_Update(&context, (unsigned char*)input, length);
 	SHA256_Final(md, &context);
 	free(compare);
 	free(input);
 	here:
 	n++;
 	for(i=0; i<SHA256_DIGEST_LENGTH; i++){
 		sprintf(hashed+(i*2), "%02x", md[i]);
 	}
 	SHA256_Init(&context);
 	SHA256_Update(&context, (unsigned char*)hashed, strlen(hashed));
 	SHA256_Final(md, &context);
 	if(n==50000){
 		printf("--> Write the above hash into the file '/etc/ftputils/auth':\n");
    	for(i=0; i<SHA256_DIGEST_LENGTH; i++){
   			printf("%02x", md[i]);
 		}
 		printf("\n");
 		return EXIT_SUCCESS;
	}
	goto here;
	return 0;
}