#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wallocator.h>

char * fill_buf(FILE *);
void compress(char *);

int main(int argc, char * argv[])
{
     FILE * fp;
     char * buffer;     

     if(argc == 1)
     {
	  printf("wzip: file1 [file2 ...]\n");
	  exit(1);
     }
     else 
     {
	  for(int i = 1; i < argc; i++)
	  {
	       if((fp = fopen(argv[i],"r")) == NULL)
	       {
		    printf("wzip: cannot open file\n");
		    exit(1);
	       }
               buffer = fill_buf(fp);
	       fclose(fp);
	  }
	  compress(buffer);
     }
     
     wfree(buffer);
     return 0;
}

char * fill_buf(FILE * fp)
{
     static char * buffer = NULL;
     fseek(fp, 0L, SEEK_END);
     long last = ftell(fp);
     int i;

     if(buffer == NULL)
     {
	  buffer = (char *) wcalloc(last + 1, sizeof(char));
	  if(buffer == NULL)
	  {
	       printf("wzip: cannot allocate memory\n");
	       exit(1);
	  }
	  
	  rewind(fp);
	  for(i = 0; i < last; i++)
	       buffer[i] = getc(fp);
	  buffer[last] = '\0';
     }
     else
     {
	  buffer = (char *) wrealloc(buffer, strlen(buffer) + last + 1);
	  if(buffer == NULL)
	  {
	       printf("wzip: cannot reallocate memory\n");
	       exit(1);
	  }

	  rewind(fp);
	  for(i = 0; buffer[i] != '\0'; i++)
	       ;
	  last += strlen(buffer);
	  while(i < last)
	       buffer[i++] = getc(fp);
	  buffer[last] = '\0';
     }

     return buffer;
}

void compress(char * buffer)
{
     unsigned int count = 0;
     char cur_ch = *buffer;

     while(*buffer != '\0')
     {
	  if(cur_ch != *buffer)
	  {
	       fwrite(&count, sizeof(unsigned int), 1 , stdout);
	       putchar(cur_ch);
	       cur_ch = *buffer;
	       count = 1;
	  }
	  else
	       count++;
	  
	  buffer++;
     }
     fwrite(&count, sizeof(unsigned int) , 1 , stdout);
     putchar(cur_ch);
}
	    
	  
     
	  
	  
	       
     
     
     
     
     
			 
