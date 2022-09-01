#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <wallocator.h>

void * fill_buf(void * );
char * concatenation(char ** , int );
void compress(char * );

int main(int argc, char * argv[])
{
     pthread_t * p;
     char ** buffers;

     if(argc == 1)
     {
	  printf("wzip: file1 [file2 ...]\n");
	  exit(1);
     }
     else
     {
	  p = (pthread_t * ) wcalloc(argc - 1, sizeof(pthread_t));
	  if(p == NULL)
	  {
	       printf("wzip: cannot allocate memory\n");
	       exit(1);
	  }
	  buffers = (char ** ) wcalloc(argc - 1, sizeof(char * ));
	  if(buffers == NULL)
	  {
	       printf("wzip: cannot allocate memory\n");
	       exit(1);
	  }

	  for(int i = 1; i < argc; i++) 
	       if(pthread_create(&p[i - 1], NULL, fill_buf, argv[i]))
	       {
		    printf("wzip: cannot create thread\n");
		    exit(1);
	       }
	       
	  for(int i = 0; i < argc - 1; i++)
	       if(pthread_join(p[i], (void ** ) &buffers[i]))
	       {
		    printf("wzip: cannot wait for thread\n");
		    exit(1);
	       }
	  
	  compress(concatenation(buffers, argc - 1));

     }
     wfree(p);

     return 0;
}

void * fill_buf(void * arg)
{
     char * filename = (char * ) arg;
     char * buffer;
     FILE * fp;
     long last;

     if((fp = fopen(filename, "r")) == NULL)
     {
	  printf("wzip: cannot open file\n");
	  exit(1);
     }

     fseek(fp, 0L, SEEK_END);
     last = ftell(fp);

     buffer = (char *) wcalloc(last + 1, sizeof(char));
     if(buffer == NULL)
     {
	  printf("wzip: cannot allocate memory\n");
          exit(1);
     }

     rewind(fp);
     for(int i = 0; i < last; i++)
	  buffer[i] = getc(fp);
     buffer[last] = '\0';

     fclose(fp);
     return (void * ) buffer;
}

char * concatenation(char ** buffers, int size)
{
     unsigned int count = 0;
     char * buf;

     for(int i = 0; i < size; i++)
	  count += strlen(buffers[i]);

     buf = (char *) wcalloc(count + 1, sizeof(char)); // + 1 for '\0'
     if(buf == NULL)
     {
	  printf("wzip: cannot allocate memory\n");
          exit(1);
     }

     strcpy(buf, buffers[0]);
     for(int i = 1; i < size; i++)
	  strcat(buf, buffers[i]);

     for(int i = 0; i < size; i++)
	  wfree(buffers[i]);
     wfree(buffers);
     
     return buf;
}

void compress(char * buffer)
{
     unsigned int count = 0;
     char cur_ch = *buffer;

     for(int i = 0; buffer[i] != '\0'; i++)
     {
	  if(cur_ch != buffer[i])
	  {
	       fwrite(&count, sizeof(unsigned int), 1 , stdout);
	       putchar(cur_ch);
	       cur_ch = buffer[i];
	       count = 1;
	  }
	  else
	       count++;
     }	  
     fwrite(&count, sizeof(unsigned int) , 1 , stdout);
     putchar(cur_ch);

     wfree(buffer);
}

     

     
     
     

     


