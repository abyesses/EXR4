//
//  main.c
//  EXR4
//
//  Created by Abraham Esses on 2/15/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
void creaNivel(int,int,int);
void creaNivel(int  n,int p,int curr){
    int tube[2];

    pid_t pid;
    pipe(tube);
    for (int i=0;i<n;i++) {
        pid = fork();
        if(pid < 0){
            perror("Fork");
        }
        if (pid == 0) {
            char k;
            close(tube[1]);
            read(tube[0], &k, sizeof(char));
            printf("Leyendo testigo: '%c' desde PID:%d y PPID: %d por 5 segundos\n",k,getpid(),getppid());
            sleep(5);
            if(curr <= n){
                creaNivel(n,p,curr+1);
            }
            exit(0);
        }
        else
        {
            char a = 'a';
            close(tube[0]);
            write(tube[1], &a, sizeof(char));
            printf("Enviando testigo '%c' PID:%d, PPID:%d\n",a,getpid(),getppid());
            wait(NULL);
        }
    }
}
int main(int argc, const char * argv[]) {
    
    int num_procesos = 0;
    int niv_procesos = 0;
    int c,index;
    //pid_t * pid, *pid_p;
    //pid_t main_pid,main_pid2,main_pid3;
    while ((c = getopt (argc, argv, "n: p:")) != -1)
        switch (c)
    {
        case 'n':
            niv_procesos = atoi(optarg);
            break;
        case 'p':
            num_procesos = atoi(optarg);
            break;
        case '?':
            if ( optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }
    
    for (index = optind; index < argc; index++)
        printf ("El argumento no es una opción válida %s\n", argv[index]);
    printf ("niveles = %d, procesos = %d, \n",
            niv_procesos,num_procesos);
    
    creaNivel(num_procesos,niv_procesos,0);
    return 0;

}
