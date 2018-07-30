#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int main()
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4096];
    char w_buf[4096];
    int writenum;
    int rnum;

    memset(r_buf, 0, sizeof(r_buf));

    if (pipe(pipe_fd) < 0)
    {
        cout << "[PARENT], pipe create error" << endl;
        return -1;
    }

    if ((pid = fork()) == 0)
    {
        close(pipe_fd[1]);

        while (1)
        {
            rnum = read(pipe_fd[0], r_buf, 500);
            cout << "[CHILD] readnum is " << rnum << endl;
            if (rnum == 0)
            {
                cout << "[CHILD] all the writer of pipe are closed. break and exit." << endl;
                break;
            }
        }
        close(pipe_fd[0]);
        exit(0);
    }
    else if (pid > 0)
    {
        close(pipe_fd[0]);
        memset(w_buf, 0, sizeof(w_buf));

        if ((writenum = write(pipe_fd[1], w_buf, 1024)) == -1)
        {
            cout << "[PARENT] write to pipe error" << endl;
        }
        else {
            cout << "[PARENT] the bytes write to pipe is " << writenum << endl;
        }

        sleep(15);
        cout << "[PARENT] I will close the last write end of pipe." << endl;
        close(pipe_fd[1]);

        sleep(2);
        return 0;

    }
}