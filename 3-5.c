#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
char * newbuf()
{
	char * s;
	int i;
	if ((s = malloc(sizeof(char)*(20)))==NULL)
		return s;
	
	i = (int)time(NULL);
	sprintf(s, "%dbuf.tmp",i);
	sleep(1);	
	return s;
}
char * fdgets(int fd)
/*fd - file handle(readable)
//returns string from current position to nrearest '\n' or to the end of file*/
{
	char *s,*s1,c;
	int i=0,j=0,k,t,lever = 0;
	
	i = 4;
	if ((s=malloc(i))==NULL)
		return s;
	while(!(lever)&&((k=read(fd,&c,1))!=0))
		{
			if (k<0)
				{
					perror("fdgets read");
				}
			s[j++]=c;
			lever = (c=='\n');
			
			if(i==j)
				{
					t=0;
					i*=2;
					s1 = NULL;
					while((!(s1)&&(t<4)))
						{
							s1 = realloc(s,i);
							t++;
						}
					if (t==4)
						{
							perror("realloc");
							exit(5);
						}
				}
		}
	if (j ==0)
		{
			free(s);
			return NULL;
		}
		
	return s;
}


int main(int argc, char **argv)
{
	int fd1[2];
	int fd2[2];
	int fd,fdout;
	int pid;
	int i,j,k,slen;
	char *s,*s1;
	char c;
	if (argc<2)
		{
			fprintf(stderr,"program need file\n");
			exit(2);
		}
	fd = open(argv[1],O_RDONLY);
	if (fd<0)
	{
		fprintf(stderr,"can't open file\n");
		exit(3);
	}
	pipe(fd1);
	pipe(fd2);
	pid = fork();
	if (pid<0)
		{
			perror("fork");
			exit(5);
		}
	if (pid>0)
		{
			s1 = newbuf();
			close(fd1[1]);
			close(fd2[0]);
			fdout = open(s1,O_CREAT|O_WRONLY|O_TRUNC|O_APPEND);
			if (fdout<0)
				{
					perror("buf creation");
					close(fd1[0]);
					close(fd2[1]);
					close(fd);
					exit(6);
				}
					
			while ((s = fdgets(fd))!=NULL)
				{
					k = strlen(s);
					printf("father sending : %s",s); 
					write(fd1[0],s,strlen(s));
					if(s[k-1]!='\n');
						{
							c = '\n';
							write(fd1[0],&c,1);
						}
						free(s);
					s = fdgets(fd2[1]);
					printf("Father got this: %s",s);
					write(fdout,s,k);
					free(s);
				}
			close(fd1[0]);
			close(fd2[1]);
			close(fd);
			close(fdout);
			//rewrite argv[1] with info from s1
			//remove(s1);
			wait(NULL);/*waiting for son*/
			return 0;
		}
	else
		{
			close(fd);
			close(fd1[0]);
			close(fd2[1]);
			while((s = fdgets(fd1[1]))!=NULL)
			{
				printf("Son got string : %s",s);
				k = strlen(s)-1;
				i = k/2;
				for (j=0;j<i;j++);
					{
						c = s[j];
						s[j] = s[k-j];
						s[k-j] = c;
					}
				printf("Son made this : %s",s);
				write(fd2[0],s,strlen(s));
				
			}	
			
		}
			
			
			
			
	
	
	return 0;
}

