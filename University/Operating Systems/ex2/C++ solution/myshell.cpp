#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>

/**
 * @file myshell.cpp
 * @brief The required myshell.c as a C++ file.
 */

 /**Does all initialization and setup needed for the process_arglist function.
  * @returns 0 on success, any other return value indicates an error.
  */
int prepare(void) {
    // SIGINT handling:
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed for SIGINT!");
        return -1;
    }

    // Zombie prevention:
    struct sigaction sa_chld;
    sa_chld.sa_handler = SIG_IGN;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction failed for SIGCHLD!");
        return -1;
    }
    
    return 0;
}

/**
 * @param count The number of non-NULL words in the parsed command line.
 * @param arglist The parsed command line. The final entry, at index count, is NULL (thus its length is
 * count+1).
 * @returns 1 if no error occurs, otherwise prints an error message and return 0.
 */
int process_arglist(int count, char **arglist) {
    // Background commands:
    if (strcmp(arglist[count - 1], "&") == 0) {
        arglist[count - 1] = NULL;

        pid_t pid = fork();

        if (pid == 0) { // Child
            execvp(arglist[0], arglist);
            perror("execvp failed in child process while running in the background!");
            exit(1);
        }
        else if (pid > 0) { return 1; } // Parent
        else {
            perror("fork failed while trying to execute commands in the background!");
            return 0;
        }
    }
    else {
        bool is_pipe = false;
        bool is_redirect = false;
        int pipe_ind = -1;
        int redirect_ind = -1;

        for (int i = 0; i < count; i++) {
            // Pipe detection:
            if ((!is_pipe) && (strcmp(arglist[i], "|") == 0)) {
                is_pipe = true;
                pipe_ind = i;
                if (is_redirect) { break; }
            }
            // Redirection detection:
            else if ((!is_redirect) && (strcmp(arglist[i], ">") == 0)) {
                is_redirect = true;
                redirect_ind = i;
                if (is_pipe) { break; }
            }
        }

        // Piping commands:
        if (is_pipe) {
            arglist[pipe_ind] = NULL;
            char** cmd2 = &arglist[pipe_ind + 1];

            int fd[2];
            if (pipe(fd) == -1) {
                perror("pipe failed!");
                return 0;
            }

            pid_t pid1 = fork();

            if (pid1 == 0) {    // First child
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);

                signal(SIGINT, SIG_DFL);
                execvp(arglist[0], arglist);
                perror("execvp failed for first command during piping!");
                exit(1);
            }
            else if (pid1 < 0) {
                perror("First fork failed while trying to execute commands using piping!");
                return 0;
            }

            pid_t pid2 = fork();

            if (pid2 == 0) {    // Second child
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);

                signal(SIGINT, SIG_DFL);
                execvp(cmd2[0], cmd2);
                perror("execvp failed for second command during piping!");
                exit(1);
            }
            else if (pid2 > 0) {    // Parent
                close(fd[0]);
                close(fd[1]);

                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
                return 1;
            }
            else {
                perror("Second fork failed while trying to execute commands using piping!");
                return 0;
            }
        }
        // Redirection commands:
        else if (is_redirect) {
            arglist[redirect_ind] = NULL;

            char* filename = arglist[redirect_ind + 1];
            int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file_fd == -1) {
                perror("File opening failed!");
                return 0;
            }

            pid_t pid = fork();

            if (pid == 0) { // Child
                dup2(file_fd, STDOUT_FILENO);
                close(file_fd);

                signal(SIGINT, SIG_DFL);
                execvp(arglist[0], arglist);
                perror("execvp failed when trying to execute the redirected output's command!");
                exit(1);
            }
            else if (pid > 0) { // Parent
                waitpid(pid, NULL, 0);
                close(file_fd);
                return 1;
            }
            else {
                perror("fork failed when trying to execute the redirected output's command!");
                close(file_fd);
                return 0;
            }
        }
        // Regular commands:
        else {
            pid_t pid = fork();

            if (pid == 0) { // Child
                signal(SIGINT, SIG_DFL);
                execvp(arglist[0], arglist);
                perror("execvp failed when trying to execute a regular command!");
                exit(1);
            }
            else if (pid > 0) { // Parent
                waitpid(pid, NULL, 0);
                return 1;
            }
            else {
                perror("fork failed when trying to execute a regular command!");
                return 0;
            }
        }
    }

    return 1;
}

/**Does all cleanups related to the process_arglist function.
 * @returns 0 on success, any other return value indicates an error.
 */
int finalize(void) {
    return 0;   // No cleanup neccessary
}