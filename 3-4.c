/*
 * 3-4.c
 * 
 * Copyright 2017 MandoMirsh <seriu007@COMPUTER-PK>
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

int main (int argc, char ** argv)
{
	char *s,c;
	int fd[2];
	int i;
	pipe(fd);
	s = malloc(sizeof(char)*5);
	c = ' ';
	pid = fork();
	if (pid<0)
		{
			perror("fork\n");
			exit(2);
		}
	if (pid)
		{
			write(fd[1],&c,1);
		}
	else
		{
			read(fd[0],s,5);
			printf("Hallo\n");
		}
	close(fd[0]);
	close(fd[1]);
			

	return 0;
	
}
