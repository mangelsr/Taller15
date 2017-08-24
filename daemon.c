#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){

    pid_t process_id = 0;

    // Crea el proceso hijo
    process_id = fork();
    
    // Verifica retorno del fork()
    if (process_id < 0){
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }

    // Mata el proceso del padre
    if (process_id > 0){
        printf("process_id of child process %d\n", process_id);
        exit(0);
    }

    umask(0);

    int fd = open("./log_cpu.log", O_WRONLY|O_CREAT|O_TRUNC);
    
    setsid();

    close(0);
    close(1);
    close(2);

    while(1){
        void *buf = malloc(sizeof(void *)*100);
        FILE *fp = popen("top -bn2 | grep '%Cpu' | tail -1", "r");
        fread(buf, 100, 1, fp);
        write(fd, buf, 100);
    }
    
    return (0);
}