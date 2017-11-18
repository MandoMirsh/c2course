/*
 * testa.c
 * 
 * Copyright 2017 Черевков С.Н. <s02160255@debian>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
int cmdpar(int argc, char ** argv, int * r, int * n, int * o, int * m)
{
	int i=0, k=0,key =0,j =0,t =0;
	
	if (argc ==1) 
	{
		fprintf(stderr, "Error: program needs arguments!\n");
		exit(5);
	}
	
	if ((argc ==2)&&(argv[1][0]!='-')&&(argv[1][0]!='+')) return 0;
	/*solus*/
	
	
	do
	{
		i++;
		t = strlen(argv[i]);
		if (argv[i][0]=='-')
			{
				key+=1;
				/*is length correct*/
				if (t<2)
					{
						fprintf(stderr, "Error: key not specified!\n");
						exit(2);
					}
				if (t>2)
					{
						fprintf(stderr, "Error: key is too long!\n");
						exit(2);
					}
				/*what key is exactly?*/
				if(argv[i][1]=='o')
					{
						if (k&4)
							{
								fprintf(stderr, "Error: double specifying a key\n");
								exit(4);
							}
						if ((i == (argc-1))||(argv[i+1][0]=='-')||(argv[i+1][0]=='+'))
							{
								fprintf(stderr, "Error: no file name after -o key\n");
								exit(5);
							} 
						k+=4;
						*o = i;
						key -= 1;
					}
				if(argv[i][1] =='m')
					{
						if (k&8)
							{
								fprintf(stderr, "Error double specifying a key\n");
								exit(4);
							}
						k+=8;
						*m = i;
						key -= 1;
					}
				if(argv[i][1] =='r')
					{
						if (k&1)
							{
								fprintf(stderr, "Error double specifying a key\n");
								exit(4);
							}
						k+=1;
						*r = i;
						key -=1;
					}
			if (key)
				{
					fprintf(stderr, "Error: wrong key!\n");
					exit(3);
				}

				
			}
		if (argv[i][0]=='+')
			{/*will need to rewrite if will need more than one + key*/
				if (t<2)
					{
						fprintf(stderr, "Error: key not specified!\n");
						exit(2);
					}/*we must got a value*/
				for (j = 1;j<t;j++)
						{
							if ((argv[i][j]<'0')||(argv[i][j]>'9'))
								{
									fprintf(stderr,"Error: wrong key!\n");
									exit(3);
								}
						}
				if(*n!=0)
					{
						fprintf(stderr, "Error double specifying a key\n");
						exit(4);
					}
				*n = i;
				k+=2;
			}
			
	}
	while(i<argc-1);
	return k;
    
}

int sortstart(int argc, char ** argv, char ***sargs,int * offset, int * sargc,char ** out)
{
	/*filenames "from" array forming*/
	int mode,buf,i,k;
	int r=0,n=0,o=0,m=0;
	/*mode*/
	mode = cmdpar(argc,argv,&r,&n,&o,&m);
	/*offset*/
	if (n!=0)
		(*offset) = atoi(argv[n]+1);
	printf("off : %d\n",*offset);
	/*out*/
	if (o==0)
		(*out) = NULL;
	else
		{
			(*out) = malloc(sizeof(char)*strlen(argv[o+1])+1);
			strcpy(*out,argv[o+1]);
		}
	/*sargs*/
	buf = argc - 1 - (r!=0) - (n!=0) - (m!=0) - 2*(o!=0);
	printf("buf = %d\n",buf);
	if (buf ==0)
		{
			fprintf(stderr,"No file specified!\n");
			exit(6);
		}
	(*sargc)  = buf;
	if (((*sargs) = malloc((sizeof(char*))*buf--))==NULL)
		return -1;
	
	k=o;
	if (o!=0)
		k++;
	for (i = 1;i<argc;i++)
		{
			if ((i!=r)&&(i!=n)&&(i!=m)&&(i!=o)&&(i!=k))
				{
					printf("i = %d\n%s\n",i,argv[i]);
					/*sargs[buf--] = argv[i];
					fprintf("%s\n",sargs[buf]);*/
					(*sargs)[buf] = malloc(sizeof(char)*(strlen(argv[i])+1));
					strcpy(((*sargs)[buf--]),argv[i]);
				}				
		}
		
	return mode;
}
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
void versargs(char *** sargs, int * sargc)
{
	int i;
	int fd;
	i=0;
	while(i<(*sargc))
		{
			fd = open((*sargs)[i],O_RDONLY);
			if (fd<0)
				{
					printf("%s\n",(*sargs)[i]);
					free((*sargs)[i]);
					(*sargs)[i] = (*sargs)[*sargc-1];
					(*sargc)--;
				}
			else
				{
					close(fd);
					i++;
				}
		}
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
					perror("read");
				}
			else
			{
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
						else 
							s = s1;
					}
			}
		}
	if (j ==0)
		{
			free(s);
			return NULL;
		}
	s[j++] = '\0';	
	return s;
}

int filelen(fd)
/*need newly opened file*/
{
	int i=0;
	char *s=NULL;
	lseek(fd,0,SEEK_SET);
	while ((s = fdgets(fd))!=NULL)
		{
			free(s);
			i++;
		}
	lseek(fd,0,SEEK_SET);
	return i;
}
void submerge(int fd1,int fd2,int fdres,int r)
{
	int j1,j2;
	char *s1,*s2;
	j1 = filelen(fd1);
	j2 = filelen(fd2);
	if ((j1))
		{
			s1 = fdgets(fd1);
		}
	if ((j2))
		{
			s2 = fdgets(fd2);
		}
	while((j1)&&(j2))
		{
 			if (((strcmp(s1,s2))*(1+(r)*(-2))>0))
				{
					write(fdres,s2,strlen(s2));
					free(s2);
					s2 = fdgets(fd2);
					j2--;
				}
			else
				{
					write(fdres,s1,strlen(s1));
					free(s1);
					s1 = fdgets(fd1);
					j1--;
				}
		}
	while(j2)
		{
			write(fdres,s2,strlen(s2));
			free(s2);
			s2 = fdgets(fd2);
			j2--;
		}
	while(j1)
		{
			write(fdres,s1,strlen(s1));
			free(s1);
			s1 = fdgets(fd1);
			j1--;
		}
}

void merge(char * name,int r)
/*name must be real name of file.*/
{
	char *s1,*s2,*s;
	int fd1,fd2,fd;
	int k;
	int i,j;
	fd = open(name,O_RDONLY);
	if ((k = filelen(fd))<2)
		{
			return;
		}
	else
	{
		/*more than two lines
		  create subfiles*/
		s1 = newbuf();
		s2 = newbuf();
		//open them into writing
		fd1 = open(s1,O_WRONLY|O_CREAT|O_APPEND|O_TRUNC,0666);
		fd2 = open(s2,O_WRONLY|O_CREAT|O_APPEND|O_TRUNC,0666);
		//write to the first one
		j = k/2;
		for (i=0;i<j;i++)
			{
				s = fdgets(fd);
				write(fd1,s,strlen(s));
				free(s);
			}
			close(fd1);
		//write to the second one	
		while(i<k)
			{
				s = fdgets(fd);
				write(fd2,s,strlen(s));
				free(s);
				i++;
			}
		close(fd2);
		close(fd);
		/*merge subfiles*/
		merge(s1,r);
		merge(s2,r);
		/*now we use submerge to sort lines back into file*/
		fd1 = open(s1,O_RDONLY);
		fd2 = open(s2,O_RDONLY);
		fd = open(name,O_WRONLY|O_APPEND|O_TRUNC);	
		submerge(fd1,fd2,fd,r);
		/*finished. now we close all files and dispose of the subfiles*/
	
		close(fd1);
		close(fd2);
		close(fd);
		remove(s1);
		remove(s2);
		free(s1);
		free(s2);
	}
}
void sort(char *name, int offset,int fdout, int r)
/*name must be valid*/
{
	char *s,*s1;
	int fd0,fd;
	int flen;
	int i;
	fd0 = open(name, O_RDONLY);
	flen = filelen(fd0);
	if (flen<=offset)
		return;
	s1 = newbuf();
	if ((fd = open(s1,O_CREAT|O_WRONLY|O_APPEND|O_TRUNC,0666))<0)
		{
			perror("creating file");
		}
	else
		{
		/*skip*/
		for (i=0;i<offset;i++)
			{
				s = fdgets(fd0);
				free(s);
			}
		/*write to subfile*/
		while(i<flen)
			{
				s = fdgets(fd0);
				write(fd,s,strlen(s));
				free(s);
				i++;
			}
		close(fd);
		close(fd0);
		merge(s1,r);
		/*file sorted.*/
		fd = open(s1,O_RDONLY);
		flen = filelen(fd);
		for (i = 0;i<flen;i++)
			{
				s = fdgets(fd);
				write(fdout,s,strlen(s));
				free(s);
			}
		close(fd);
		remove(s1);		
	}
}	

void mfunc(char **sargs,int sargc,int fdout,int r)
/*if it was an m key*/
{
	char *s1,*s2,*s3;
	int fd1,fd2,fd3;
	int i;
	
	s2 = newbuf();
	s3 = newbuf();
	fd2 = open(s2,O_CREAT|O_WRONLY,0666);
	close(fd2);
	fd3 = open(s3,O_CREAT|O_WRONLY,0666);
	close(fd3);
	//подготовили, теперь вращаем
	for (i=0;i<sargc;i++)
	{
		s1 = sargs[i];
		fd1 = open(s1,O_RDONLY);
		fd2 = open(s2,O_RDONLY);
		fd3 = open(s3,O_WRONLY|O_TRUNC|O_APPEND,0666);
		submerge(fd1,fd2,fd3,r);
		//закрываем дескрипторы файлов
		close(fd1);
		close(fd2);
		close(fd3);
		
		//перетасовываем имена
		
		s1 = s2;
		s2 = s3;
		s3 = s1;
	}
	remove(s3);
	fd1 = open(s2,O_RDONLY);
	while((s1 = fdgets(fd1))!=NULL)
		{
			write(fdout,s1,strlen(s1));
			free(s1);
		}
	remove(s2);
	free(s1);
	free(s2);
	free(s3);
}

int main(int argc, char **argv)
{
	int i,flen;
	char *s=NULL/*!!!*/,*s1=NULL/*!!!*/,*s2;
	char **sargs;
	int fdest=1,offset=0/*!!!*/,sargc,mode;
	/*start sequence*/
	mode = sortstart(argc,argv, &sargs,&offset,&sargc,&s1);
	if (mode <0)
		fprintf(stderr,"Error in sortstart function. Might be something with your system though\n");
	/*verifying file names*/
	versargs(&sargs,&sargc);
	if (s1)
		{
			if((fdest = open(s1,O_CREAT|O_WRONLY|O_TRUNC|O_APPEND,0666))<0)
				{
					perror("can't open destination file");
					fdest = 1;
				}
		} 
	if (!(mode&8))
		{
			for (i=0;i<sargc;i++)
				sort(sargs[i],offset,fdest,mode&1);
		}
	else
		{
			mfunc(sargs,sargc,fdest,mode&1);
		}
	/*exit sequence*/
	
	/*freeing filename array*/
	for (i=0;i<sargc;i++)
		free(sargs[i]);
	free(sargs);
	
	if (s1)
		{
			close(fdest);
			free(s1);
		}
	return 0;
}

