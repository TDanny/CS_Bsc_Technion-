#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "Commands.h"
#include "signals.h"

int main(int argc, char* argv[]) {
    if(signal(SIGTSTP , ctrlZHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-Z handler");
    }
    if(signal(SIGINT , ctrlCHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-C handler");
    }

    struct sigaction Action;
    memset(&Action, '\0', sizeof(Action));
    Action.sa_flags = SA_RESTART;
    Action.sa_handler = &alarmHandler;
    sigemptyset(&Action.sa_mask);
    sigaction(SIGALRM, &Action, NULL);


    SmallShell& smash = SmallShell::getInstance();
    while(true) {
        smash.is_pipe=false;
        std::string cmd_line;
        std::string original_cmd;
        std::cout << smash.smash_prompt <<"> ";
        std::getline(std::cin, cmd_line);
        smash.original_cmd = cmd_line;
        smash.executeCommand(cmd_line.c_str() , cmd_line.c_str());
        if(smash.is_pipe){
            break;
        }
    }
    return 0;
}