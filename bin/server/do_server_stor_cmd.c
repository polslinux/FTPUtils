#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> /* per usare uint32_t invece di size_t */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <inttypes.h> /* per printare il tipo di dato uint32_t */
#include "../prototypes.h"

void do_server_stor_cmd(const int f_sockd, const int m_sockd){
  int fd;
  uint32_t fsize, fsize_tmp, nread = 0, total_bytes_read = 0, fn_size = 0;
  char *stor_filename = NULL, *other = NULL;
  void *filebuffer = NULL;
  char buf[256];

  memset(buf, 0, sizeof(buf));
  if(recv(f_sockd, &fn_size, sizeof(fn_size), MSG_WAITALL) < 0){
    perror("Errore durante la ricezione della lunghezza del nome del file");
    onexit(f_sockd, m_sockd, 0, 2);
  }
  if(recv(f_sockd, buf, fn_size+5, 0) < 0){
    perror("Errore ricezione nome file");
    onexit(f_sockd, m_sockd,0 ,2);
  }
  other = NULL;
  stor_filename = NULL;
  other = strtok(buf, " ");
  stor_filename = strtok(NULL, "\0");
  stor_filename = strdup(stor_filename);

  if(strcmp(other, "STOR") == 0){
    printf("Ricevuta richiesta STOR\n");
  } else onexit(f_sockd, m_sockd, 0 ,2);

  memset(buf, 0, sizeof(buf));
  if(recv(f_sockd, buf, 3, 0) < 0){
    perror("Errore ricezione conferma file");
    onexit(f_sockd, 0, 0, 1);
  }    

  other = NULL;
  other = strtok(buf, "\0");
  if(strcmp(other, "NO") == 0){
    printf("ERRORE: il file richiesto non esiste\n");
    onexit(f_sockd, m_sockd, 0 ,2);
  }

  fsize = 0;
  if(recv(f_sockd, &fsize, sizeof(fsize), 0) < 0){
    perror("Errore nella ricezione della grandezza del file");
    onexit(f_sockd, m_sockd, 0 ,2);
  }
  fd = open(stor_filename, O_CREAT | O_WRONLY, 0644);
  if (fd  < 0) {
    perror("open");
    onexit(f_sockd, m_sockd, 0 ,2);
  }
  fsize_tmp = fsize;
  filebuffer = malloc(fsize);
  if(filebuffer == NULL){
    perror("malloc");
    onexit(f_sockd, m_sockd, fd, 3);
  }
  total_bytes_read = 0;
  nread = 0;
  while((total_bytes_read != fsize) && ((nread = read(f_sockd, filebuffer, fsize_tmp)) > 0)){
    if(write(fd, filebuffer, nread) != nread){
      perror("write RETR");
      onexit(f_sockd, m_sockd, 0, 2);
    }
    total_bytes_read += nread;
    fsize_tmp -= nread;
  }
  close(fd); /* la chiusura del file va qui altrimenti client entra in loop infinito e si scrive all'interno del file */

  memset(buf, 0, sizeof(buf));
  strcpy(buf, "226 File trasferito correttamente");
  if(send(f_sockd, buf, 33, 0) < 0){
    perror("Errore invio conferma upload");
    onexit(f_sockd, m_sockd, 0, 2);
  }
  memset(buf, 0, sizeof(buf));
  free(filebuffer);
  free(stor_filename);
}