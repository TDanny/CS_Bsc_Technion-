#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;



void ctrlCHandler(int sig_num) {
    cout << "smash: got ctrl-C" << endl;
    SmallShell& shell = SmallShell::getInstance();
    if(shell.current_process != -1) {
        int my_process_num = shell.current_process;
        shell.current_cmd = "";
        shell.current_process = -1;
        kill(my_process_num, SIGINT);
        cout << "smash: process " << my_process_num << " was killed" << endl;
    }
}

void ctrlZHandler(int sig_num) {
    cout << "smash: got ctrl-Z" << endl;
    SmallShell& shell = SmallShell::getInstance();
    if(shell.current_process != -1) {
        bool my_temp = shell.last_cmd_fg;
        Command* my_command = shell.CreateCommand(shell.current_cmd.c_str(), shell.original_cmd.c_str() ,false);
        if(my_temp) { // if last command was fg so insert with already given job id
            shell.job_list.addJob(my_command, shell.current_process,shell.original_cmd.c_str(),shell.fg_alarm, true, true);
        } else {
            shell.job_list.addJob(my_command, shell.current_process,shell.original_cmd.c_str(),shell.fg_alarm, false , true);
        }
        kill(shell.current_process, SIGSTOP);
        cout << "smash: process " << shell.current_process << " was stopped" << endl;
        shell.current_cmd = "";
        shell.current_process = -1;
        delete my_command;
    }
}

void alarmHandler(int sig_num) {
    cout << "smash: got an alarm" << endl;
    SmallShell& my_shell = SmallShell::getInstance();
    if(my_shell.fg_alarm){
        kill(my_shell.current_process, SIGKILL);
        cout << "smash: " << my_shell.current_alarm_cmd << " timed out!" << endl;
        my_shell.current_alarm_cmd = "";
        my_shell.fg_alarm = false;
    } else {
        
        my_shell.alarm_list.delete_alarms();
    }
}


