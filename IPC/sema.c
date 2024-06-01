#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#include <wait.h>


int semop_p(int semid) {
	struct sembuf psembuf;
	psembuf.sem_num = 0;            // 第0个信号量
	psembuf.sem_op = -1;            // 申请操作
	psembuf.sem_flg = SEM_UNDO;     // 保证在进程退出后，能够主动释放信号量
 
	return semop(semid, &psembuf, 1);
}

// 释放信号量
int semop_v(int semid) {
	struct sembuf vsembuf[1];          // 信号量的集合
	vsembuf[0].sem_num = 0;            
	vsembuf[0].sem_op = 1;             // 释放操作
	vsembuf[0].sem_flg = SEM_UNDO;     
 
	return semop(semid, vsembuf, 1);   // 对信号量集中的1个信号量进行操作
}

int main(){
	int semid;
	if ((semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1) {   // 创建/获取信号量id
		perror("semget error");
		exit(1);
	}	
 
	if (semctl(semid, 0, SETVAL, 1) == -1) {      // 赋初值，0号信号量的初值设置为1
		perror("semctl error");
		exit(1);
	}
 
	int fd = open("/dev/tty", O_RDWR);      // 打开终端, ReaD & WRite
 
	if (fd == -1) {
		perror("open error");
		exit(1);
	}
 
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0){
		char buf[10] = "child\n";
		int i;
		
		semop_p(semid);
		for (i = 0; i < 50; i++) {
			write(fd, buf, strlen(buf)+1);
			// sleep(1);
		}
		semop_v(semid);
		
		close(fd);
	} else {
		char buf[10] = "father\n";
		int i;
 
		semop_p(semid);
		for (i = 0; i < 50; i++) {
			write(fd, buf, strlen(buf)+1);
			// sleep(1);
		}
		semop_v(semid);

        close(fd);
		waitpid(-1, NULL, 0); // `-1` for all child
		semctl(semid, 0, IPC_RMID);                 // 删除0号信号量
	}
	return 0;
}
