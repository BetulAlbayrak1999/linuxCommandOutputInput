/*
 ls -1 | cut -d'.' -f1 | sort | uniq | tee dirents
yukarıdaki terminal komutlarını C programında exec ve pipe sistem çağrısıyla gerçekleştiren bir C programı yazınız.
Pipeları kullanmanız zorunludur.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define NULL 0
void no_one_listening(int signal){
    write(1, "No one is listening!\n", 21);
}
int main()
{
    signal(SIGPIPE, no_one_listening);
    int pipe_fd[2];
    int pipe_fd2[2];
    int pipe_fd3[2];
    int pipe_fd4[2];
    pipe2(pipe_fd, O_CLOEXEC);
    pipe(pipe_fd);
    if(fork() == 0){
    	close(pipe_fd[0]);
    	dup2(pipe_fd[1], 1);
    	execlp("ls", "ls", "-1", (char*)NULL);
    	exit(-1);
    }

    {
    	close(pipe_fd[1]);
    	dup2(pipe_fd[0], 0);
    	execlp("cut", "cut", "-d.", "-f1", (char*)NULL);
    	exit(-1);
    }
    //ikinci kisim
    signal(SIGPIPE, no_one_listening);
    pipe2(pipe_fd2, O_CLOEXEC);
    pipe(pipe_fd2);
    if(fork() == 0){
    	close(pipe_fd2[0]);
    	dup2(pipe_fd2[1], 1);
    	execlp("cut", "cut", "-d.", "-f1", (char*)NULL);
    	exit(-1);
    }

    {
    	close(pipe_fd2[1]);
    	dup2(pipe_fd2[0], 0);
    	execlp("sort", "sort", (char*)NULL);
    	exit(-1);
    }
    //ucuncu kisim
    signal(SIGPIPE, no_one_listening);
    pipe2(pipe_fd3, O_CLOEXEC);
    pipe(pipe_fd3);
    if(fork() == 0){
    	close(pipe_fd3[0]);
    	dup2(pipe_fd3[1], 1);
    	execlp("sort", "sort", (char*)NULL);
    	exit(-1);
    }

    {
    	close(pipe_fd3[1]);
    	dup2(pipe_fd3[0], 0);
    	execlp("uniq", "uniq", (char*)NULL);
    	exit(-1);
    }
    //dorduncu kisim
    signal(SIGPIPE, no_one_listening);
    pipe2(pipe_fd4, O_CLOEXEC);
    pipe(pipe_fd4);
    if(fork() == 0){
    	close(pipe_fd4[0]);
    	dup2(pipe_fd4[1], 1);
    	execlp("uniq", "uniq", (char*)NULL);
    	exit(-1);
    }

    {
    	close(pipe_fd4[1]);
    	dup2(pipe_fd4[0], 0);
    	execlp("tee dirents", "tee dirents", (char*)NULL);
    	exit(-1);
    }


    return 0;
}
