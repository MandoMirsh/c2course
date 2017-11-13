/*
 * taskmz4.c
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
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc, char **argv)
{
	char s[41];
	char c;
	int fd1[2], fd2[2];
	int stlen;
	int fd,i,pid;
	if (argc == 1)
		{
			fprintf(stderr,"Need file name!\n");
			exit(3);
		}	
	fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0666);
	if (fd<0)
		{
			perror("open\n");
			exit(2);
		}
	pipe(fd1);
	pipe(fd2);
	
	//kyr'am
	if ((pid = fork())<0)
		{
			perror("fork\n");
			exit(1);
		}
	//ad
	if (pid==0)
		{
			fprintf(stderr,"Son started\n");
			close(fd2[0]);
			close(fd1[1]);
			for (;;)
			{
				fgets(s,40,stdin);
				if (!(strcmp("exit\n",s)))
					{
						close(fd1[0]);
						close(fd2[1]);
						close(fd);
						return 0;
					}
				stlen = strlen(s);
				write(fd2[1],s,stlen*sizeof(char));
				read(fd1[0],&c,sizeof(char));
				for (i=0;i<10;i++)
					write(fd,&c,sizeof(char));
			}
		}	
	//buir
	close(fd2[1]);
	close(fd1[0]);
	dup2(fd1[1],1);
	dup2(fd2[0],0);
	fprintf(stderr,"Parent works\n");
	for(;;)
	{
		i = 0;
		while (((stlen = read(0,&c,sizeof(char)))>0)&&(c!='\n'))
				s[i++] = c;
		if (stlen<0)
			{
				close(fd);
				close(fd2[0]);
				close(fd1[1]);
				wait(NULL);
				return 0;
			}
				
		s[i++] = '\n';
		s[i] = '\0';
		write(fd,s,strlen(s)*sizeof(char));
		c = '+';
		write(fd1[1],&c,sizeof(char));
	}
	
	return 0;
}

