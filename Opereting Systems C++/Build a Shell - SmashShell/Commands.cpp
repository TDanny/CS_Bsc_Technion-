#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <fcntl.h>

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#define SYS_FAIL -1
#define MAX_SIGNALS 32
#endif

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}
//helper funcion for allocating new objects:
char** init_args(const char* cmd_line, int *num_of_args){
  char** args = (char**)malloc(COMMAND_MAX_ARGS * sizeof(char**));
  if (!args)
    return nullptr;
  for (int i = 0; i < COMMAND_MAX_ARGS; i++)
    args[i] = nullptr;
  int num = _parseCommandLine(cmd_line, args);
    if (num == -1)
    args = nullptr;
  *num_of_args = num;
  return args;
}



//free helper array (buffer) char**
void freeArgs(char** args , int num_of_args){
  if(!args)
    return;
  for(int i=0 ; i<num_of_args ; i++){
    if(args[i])
      free(args[i]);
  }
  free(args);
}

bool isNumer(const std::string &s){
  std::string::const_iterator it = s.begin();
  while (it != s.end() && (std::isdigit(*it) || *it == '-')) ++it;
  return !s.empty() && it == s.end();
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

Command::Command(const char* cmd_line , const char* original_cmd) : cmd_line(cmd_line) , original_cmd(original_cmd) {}

BuiltInCommand::BuiltInCommand(const char* cmd_line, const char* original_cmd ) : Command(cmd_line ,original_cmd)  {
  string cmd_s(cmd_line);
  if (_isBackgroundComamnd(cmd_s.c_str())) {
    char cmd_line_copy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmd_line_copy, cmd_s.c_str());
    _removeBackgroundSign(cmd_line_copy);
    cmd_s = cmd_line_copy;
  }

  char* cmdCopy= (char*) malloc(sizeof(char) * (cmd_s.length() + 1));
  strcpy(cmdCopy, cmd_s.c_str());
  this->cmd_line = cmdCopy;
}


// TODO: Add your implementation for classes in Commands.h 
std::string SmallShell::smash_prompt = "smash";
JobsList SmallShell::job_list;
pid_t SmallShell::pid = getppid();

SmallShell::SmallShell() :  last_dir(nullptr), is_pipe(false), current_process(-1), job_id_fg(-1),
                            last_cmd_fg(false), current_cmd(), alarm_list(), current_alarm_cmd(), fg_alarm(false),
                            alarm_pid(-1), current_duration(0) {}


SmallShell::~SmallShell() {
  if (last_dir) {
    free(last_dir);
  }
}

void SmallShell::setLastDirectory(char* dir) {
  last_dir = dir;
}

bool checker(string cmd_s){
  string cmd = cmd_s.substr(0, cmd_s.find_first_not_of(" \n")); //getting rid of the \n at the end of the string
  return (cmd == "chprompt&") || (cmd == "showpid&") || (cmd == "pwd&") || (cmd == "cd&") || (cmd == "jobs&") || (cmd == "fg&")
  || (cmd == "bg&") || (cmd == "quit&") || (cmd == "kill&");
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line, const char* original_cmd , bool is_alarm) {

 string cmd_s = _trim(string(cmd_line)); //remove whitespaces
  if (!checker(cmd_s) && _isBackgroundComamnd(cmd_s.c_str())){
    char cmd_line_copy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmd_line_copy, cmd_s.c_str());
    _removeBackgroundSign(cmd_line_copy);
    cmd_s = cmd_line_copy;
  }
   string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

  if(strstr(cmd_line, ">") != nullptr || strstr(cmd_line, ">>") != nullptr) { //means that the command ends with > or >>
    return new RedirectionCommand(cmd_line , original_cmd);
  }

  if (strstr(cmd_line, "|") != nullptr || strstr(cmd_line, "|&")){
    return new PipeCommand(cmd_line, original_cmd);
  }
  //cout << "the command is: " << cmd_line << endl;
  last_cmd_fg = false;
  if ( firstWord == "chprompt") {
    return new ChpromptCommand(cmd_line , original_cmd);
  } else if ( firstWord == "showpid") {
    return new ShowPidCommand(cmd_line, original_cmd);
  } else if ( firstWord == " pwd") {
    return new GetCurrDirCommand(cmd_line, original_cmd);
  } else if ( firstWord == "cd") {
    return new ChangeDirCommand(cmd_line, original_cmd, &last_dir);
  } else if (firstWord == "jobs") {
    return new JobsCommand(cmd_line, original_cmd);
  } else if(firstWord == "fg") {
    last_cmd_fg = true;
    return new ForegroundCommand(cmd_line, original_cmd);
  } else if ( firstWord == "bg") {
    return new BackgroundCommand(cmd_line, original_cmd);
  } else if (firstWord == "quit") {
    return new QuitCommand(cmd_line, original_cmd);
  } else if ( firstWord == "kill") {
    return new KillCommand(cmd_line, original_cmd);
  } else if (firstWord == "timeout") {
    return new TimeoutCommand(cmd_line, original_cmd);
  } else {
    return new ExternalCommand(cmd_line, original_cmd, is_alarm);
  }

    //TODO add more options for External Commands!
}



void SmallShell::executeCommand(const char* cmd_line , const char* original_cmd, bool alarm) {
  if(string(cmd_line).find_first_not_of(" \n\r\t\f\v") == string::npos)
    return;
  job_list.removeFinishedJobs();
  Command* cmd = CreateCommand(cmd_line, original_cmd, alarm);
  cmd->execute();
  delete cmd;
  current_process = -1;
  current_duration = 0;
  current_cmd = "";
  current_alarm_cmd = "";
 }


 ////////////////////////////////////////////alaram command////////////////////////////////////////////////////


 AlarmList::AlarmList() : alarms() {}

 void AlarmList::add_alarm(std::string command, time_t duration, pid_t pid){
  alarms.push_back(AlarmEntry(command, time(nullptr), duration, pid));
 }

 void AlarmList::delete_alarms() {
  if(alarms.empty())
    return;
  
  for (auto it = alarms.begin(); it != alarms.end(); ++it) {
    auto alarm_entry = *it;
    if (time(nullptr) >= alarm_entry.time_limit) {
      cout << "smash: timeout " << alarm_entry.duration << " " << alarm_entry.command << "timed out!" << endl;
      kill(alarm_entry.pid, SIGINT);
      alarms.erase(it);
      --it;
    }
  }

 }

 AlarmList::AlarmEntry::AlarmEntry(std::string command, time_t time_created, time_t duration, pid_t pid) :
 command(command), time_created(time_created), duration(duration), pid(pid) {
  time_limit = duration + time(nullptr) ; //assignment.
 }


 /////////////////////////////job list ///////////////////////////////////////////////
 JobsList::JobEntry::JobEntry(int job_id, pid_t job_pid, time_t time_created, std::string& command, bool isStopped):
  job_id(job_id), job_pid(job_pid), time_created(time_created), command(command), isStopped(isStopped) {}

  JobsList::JobsList() : job_list(), max_job_id(1) {}

  void JobsList::removeFinishedJobs() {
    if (job_list.empty()) {
      max_job_id = 1;
      return;
    }

    SmallShell& shell = SmallShell::getInstance(); 

    for (auto it = job_list.begin(); it != job_list.end(); ++it){
      if (!shell.is_pipe) {
        auto job = *it;
        int status_p;
        int ret_wait = waitpid (job.job_pid, &status_p, WNOHANG);
        if ((ret_wait == -1) || (ret_wait == job.job_pid)) {
          job_list.erase(it);
          --it;
        }
      }
    }

    int curr_max = 0;
    for (auto& job : job_list){
      if (job.job_id > curr_max){
        curr_max = job.job_id;
      }
    }
    max_job_id = curr_max + 1;
  }

  JobsList::JobEntry *JobsList::getJobById(int jobId) {
    for (auto& job : job_list){
      if (job.job_id == jobId){
        return &job;
      }
    }
    return nullptr;
  }

  void JobsList::killAllJobs(){
    for (auto& job : job_list){
      cout << job.job_pid << ": " << job.command << endl;
      kill(job.job_pid, SIGKILL);
    }
  }

  JobsList::JobEntry* JobsList::getLastJob(int *lastJobId) {
    int maxJobFound = -1;
    for (auto& job : job_list) {
      if (job.job_id > maxJobFound) {
        maxJobFound = job.job_id;
      }
    }
    *lastJobId = maxJobFound; 
    return getJobById(maxJobFound);
  }

JobsList::JobEntry* JobsList::getLastStoppedJob(int *jobId){
  int maxStopped = -1;
  for (auto& job : job_list) {
    if (job.isStopped) {
      maxStopped = job.job_id;
    }
  }
  *jobId = maxStopped;
  return getJobById(maxStopped);
}
void JobsList::removeJobById(int jobId) {
  for (auto it = job_list.begin(); it != job_list.end(); ++it) {
    auto job = *it;
    if (job.job_id == jobId){
      job_list.erase(it);
      break;
    }
  }
}


void JobsList::addJob(Command* cmd,  pid_t pid,const char* original_cmd ,bool isAlarmed , bool last_cmd_fg,  bool isStopped )
{
  removeFinishedJobs();
  string cmd_line(cmd->get_cmd_line());
  string org_cmd_line(original_cmd);
  SmallShell &smash = SmallShell::getInstance();
  if (last_cmd_fg){
    auto it = job_list.begin();
    for (; it != job_list.end(); ++it){
      auto job = *it;
      if (job.job_id > smash.job_id_fg) 
        break;
    }
    if(isAlarmed){
      job_list.insert(it, JobEntry(smash.job_id_fg, pid, time(nullptr), org_cmd_line, isStopped ));
    } else{
      job_list.insert(it, JobEntry(smash.job_id_fg, pid, time(nullptr), cmd_line, isStopped ));
    }
      
    return;
  } else{
    if(isAlarmed){
      job_list.push_back( JobEntry(max_job_id,pid, time(nullptr), org_cmd_line, isStopped));
    } else{
      job_list.push_back( JobEntry(max_job_id,pid, time(nullptr), cmd_line, isStopped));
    }
  }
  max_job_id++;
}
   
/* @@@@@@@@@@@@@@@   INTERNAL COMMANDS  @@@@@@@@@@@@@*/
// chprompt command:
ChpromptCommand::ChpromptCommand(const char* cmd_line, const char* original_cmd) : BuiltInCommand(cmd_line ,original_cmd ){}

void ChpromptCommand::execute(){
  int num_of_args =0;
  char** args = init_args(this->cmd_line, &num_of_args);
  if(!args){
    perror("smash error: malloc failed");
    return;
  }
  SmallShell& shell = SmallShell::getInstance();
    if (num_of_args == 1) {
        shell.smash_prompt = "smash"; //meaning no name entered after "chprompt"
    } 
    else { // A sentence/word have been inputed after "chprompt"
        shell.smash_prompt = args[1];
    }
    freeArgs(args , num_of_args);

}

//showpid Command
ShowPidCommand::ShowPidCommand(const char* cmd_line, const char* original_cmd) : BuiltInCommand(cmd_line, original_cmd){}

void ShowPidCommand::execute(){
  SmallShell& smash = SmallShell::getInstance();
  cout << "smash pid is " << smash.pid << endl;
}

//pwd Command 
GetCurrDirCommand::GetCurrDirCommand(const char* cmd_line, const char* original_cmd) : BuiltInCommand(cmd_line, original_cmd){}

void GetCurrDirCommand::execute(){
  long path_length = pathconf(".", _PC_PATH_MAX);
  char* buffer = (char*)malloc((size_t)path_length);
  if(buffer != nullptr){
    getcwd(buffer ,(size_t)path_length );
    cout << buffer << endl;
    free(buffer);

    //Apeerntly no need to deal with errors (according to the description)
  }
}


ChangeDirCommand::ChangeDirCommand(const char* cmd_line, const char* original_cmd ,  char** plastPwd) : BuiltInCommand(cmd_line, original_cmd), plastPwd(plastPwd){}

void ChangeDirCommand::execute(){
  int num_of_args;
  char** args = init_args(this->cmd_line, &num_of_args);
  if(!args){
    perror("smash error: malloc failed");
    return;
  }

  if(num_of_args >= 3){
    cerr << "smash error: cd: too many arguments" << endl;
  }
  else if(num_of_args == 1){
      cerr << "smash error: cd: no arguments" << endl;

  }
  else{ 
    long path_length = pathconf(".", _PC_PATH_MAX);
    char* buffer = (char*)malloc((size_t) path_length);
    if(!buffer){
      perror("smash error: malloc failed");
      freeArgs(args, num_of_args);
      return;
    }
    
    if (getcwd(buffer, (size_t) path_length) == NULL) {
      perror("smash error: getcwd failed");
      free(buffer);
      freeArgs(args, num_of_args);
      return;
    }
    string next_dir = args[1];
    if(next_dir == "-"){
      if(!(*plastPwd)){
        cerr << "smash error: cd: OLDPWD not set" << endl;
        free(buffer);
      }
      else{
        if(chdir(*plastPwd) == SYS_FAIL) {
          perror("smash error: chdir failed");
          free(buffer);
          freeArgs(args, num_of_args);
          return;
        }
        else{
          if(*plastPwd){
            free(*plastPwd);
          }
          *plastPwd = buffer;
        }
      }
    }
    else{
      if(chdir(args[1]) == SYS_FAIL){
        perror("smash error: chdir failed");
        freeArgs(args, num_of_args);
        return;
      }
      else{
        if(*plastPwd)
          free(*plastPwd);
        *plastPwd = buffer;
      }
      
    }
      
  }
  freeArgs(args, num_of_args);
}
 //Jobs Command



JobsCommand::JobsCommand(const char* cmd_line , const char* original_cmd) : BuiltInCommand(cmd_line, original_cmd) {}

void JobsCommand::execute()
{
  SmallShell &shell = SmallShell::getInstance();
  shell.job_list.removeFinishedJobs();
  for (auto it = shell.job_list.job_list.begin(); it != shell.job_list.job_list.end(); ++it)
  {
    auto job = *it;
    if (job.isStopped)
    {
      cout<< "[" << job.job_id << "] " << job.command << " : " << job.job_pid << " " << difftime(time(nullptr), job.time_created) <<
      " secs (stopped)"<< endl;
    }

    else{
      cout<< "[" << job.job_id << "] " << job.command << " : " << job.job_pid << " " << difftime(time(nullptr), job.time_created) <<
      " secs" << endl;
    }
  }
}
  //Fg Command

ForegroundCommand::ForegroundCommand(const char* cmd_line,  const char* original_cmd) : BuiltInCommand(cmd_line,original_cmd){}

void ForegroundCommand::execute() {
  int num_of_args; 
  char** args = init_args(this->cmd_line, &num_of_args);
  if (!args){
    perror("smash error: malloc failed");
    return;
  }
  SmallShell &smash = SmallShell::getInstance();
  if (num_of_args == 2 ){
    int job_id;
    try{
      if (!isNumer(args[1])){
        throw exception();
      }
      job_id = stoi(args[1]);
    } catch (exception & ){
      cerr << "smash error: fg: invalid arguments" << endl;
      freeArgs(args, num_of_args);
      return;
    }
    JobsList::JobEntry *job = smash.job_list.getJobById(job_id);
    if (job_id >= 0 && job){
      int job_pid = job->job_pid;
      if (job->isStopped){
        if(kill(job_pid, SIGCONT) == SYS_FAIL){
          perror("smash error: kill failed");
          freeArgs(args, num_of_args);
          return;
        }
      }
      int status;
      cout << job->command << " : " <<job_pid << endl;
      smash.job_id_fg = job_id;
      smash.current_cmd = job->command;
      smash.current_process = job_pid;
      smash.job_list.removeJobById(job_id);
      if (waitpid(job_pid, &status, WUNTRACED) == SYS_FAIL){
        perror("smash error: waitpid failed");
        freeArgs(args, num_of_args);
        return;
      }
    } else {
        cerr <<"smash error: fg: job-id " << job_id <<" does not exist" << endl;
    }
      
  } else if (num_of_args == 1){
    int last_job_id;
    JobsList::JobEntry *max_job = smash.job_list.getLastJob(&last_job_id);
    if (!max_job){
      cerr << "smash error: fg: jobs list is empty" << endl;
    }
    else {
      if (max_job->isStopped) {
        if (kill(max_job->job_pid, SIGCONT) == SYS_FAIL) {
          perror("smash error: kill failed");
          freeArgs(args, num_of_args);
          return;
        }
      }
      int status;
      smash.job_list.removeJobById(last_job_id);
      cout << max_job->command << " : " << max_job->job_pid<< endl;
      smash.current_process = max_job->job_pid;
      smash.job_id_fg = max_job->job_id;
      smash.current_cmd = max_job->command;
      if (waitpid(max_job->job_pid, &status, WUNTRACED) == SYS_FAIL) {
        perror("smash error: waitpid failed");
        freeArgs(args, num_of_args);
        return;
      }
    }
  } else {
    cerr << "smash error: fg: invalid arguments" <<endl;
  }
  freeArgs(args, num_of_args);
}

BackgroundCommand::BackgroundCommand(const char* cmd_line, const char* original_cmd) : BuiltInCommand(cmd_line, original_cmd){}

void BackgroundCommand::execute() {
  int num_of_args; 
  char** args = init_args(this->cmd_line, &num_of_args);
  if (!args){
    perror("smash error: malloc failed");
    return;
  }
  SmallShell &smash = SmallShell::getInstance();
  if (num_of_args == 2 ){
    int job_id;
    try{
      if (!isNumer(args[1]))
        throw exception();
      job_id = stoi(args[1]);
    } catch (exception & ){
      cerr << "smash error: bg: invalid arguments" << endl;
      freeArgs(args, num_of_args);
      return;
    }
      JobsList::JobEntry *job = smash.job_list.getJobById(job_id);
      if (job && job_id >= 0){
        int job_pid = job->job_pid;
        if (!job->isStopped){
          cerr << "smash error: bg: job-id " << job_id << " is already running in the background" << endl;
        } else{
            if(kill(job_pid, SIGCONT) == SYS_FAIL){
              perror("smash error: kill failed");
              freeArgs(args, num_of_args);
              return;
            }
          job->isStopped = false;
          cout << job->command << " : " << job_pid << endl;
        }
      } else {
        cerr << "smash error: bg: job-id " << job_id <<" does not exist" << endl;
      }    
  } else if(num_of_args == 1 ){
      int last_job_id;
    JobsList::JobEntry *max_job = smash.job_list.getLastStoppedJob(&last_job_id);
    if (last_job_id == -1){
      cerr << "smash error: bg: there is no stopped jobs to resume" << endl;
    } 
    else
    {
      if (kill(max_job->job_pid, SIGCONT) == SYS_FAIL) {
      perror("smash error: kill failed");
      freeArgs(args,num_of_args);
      return; 
    }
    max_job->isStopped = false;
    cout << max_job->command << " : " << max_job->job_pid << endl;
    }
    } else {
      cerr << "smash error: bg: invalid arguments" << endl;
    }
  freeArgs(args , num_of_args);
}

QuitCommand::QuitCommand(const char* cmd_line ,const char* original_cmd) : BuiltInCommand(cmd_line, original_cmd) {}

void QuitCommand::execute(){
  int number_of_args; 
  SmallShell& smash = SmallShell::getInstance();
  char** args = init_args(this->cmd_line, &number_of_args);

  if (number_of_args > 1 && string(args[1]).compare("kill") == 0) {
    cout << "smash: sending SIGKILL signal to " << smash.job_list.job_list.size() << " jobs:" << endl;
    smash.job_list.killAllJobs();
  }
  
    freeArgs(args , number_of_args);
    delete this;
    exit(0);
  }



KillCommand::KillCommand(const char* cmd_line, const char* original_cmd) : BuiltInCommand(cmd_line , original_cmd){}

void KillCommand::execute(){
  int num_of_args; 
  char** args = init_args(this->cmd_line, &num_of_args);
  if (!args){
    perror("smash error: malloc failed");
    return;
  }

  if (num_of_args != 3 ) {
    cerr << "smash error: kill: invalid arguments" << endl;
  }
  else {
    int sig_num;
    int job_id;
    try {
      if (!isNumer(args[2]))
        throw exception();
      char first_character = string(args[1]).at(0);
      char Minus = '-';
      if (first_character != Minus)
        throw exception();
      job_id = stoi(args[2]);

      if (!isNumer(string(args[1]).erase(0,1)))
        throw exception();
      sig_num = stoi(string(args[1]).erase(0,1));
      if(sig_num>MAX_SIGNALS)
        throw exception();
    }
    catch (exception &) {
      cerr << "smash error: kill: invalid arguments" << endl;
      freeArgs(args, num_of_args);
      return;
    }
    SmallShell& smash = SmallShell::getInstance();
    if ( JobsList::JobEntry* my_job = smash.job_list.getJobById(job_id)) {
      int job_pid = my_job->job_pid;

      if (kill(job_pid, sig_num) == SYS_FAIL) {
        perror("smash error: kill failed");
        freeArgs(args, num_of_args);
        return;
      }
      cout << "signal number " << sig_num << " was sent to pid " << job_pid << endl;

      if (sig_num == SIGTSTP) {
        my_job->isStopped = true;
      }
      else if (sig_num == SIGCONT) {
        my_job->isStopped = false;
      }
    } else {
      cerr << "smash error: kill: job-id " << job_id << " does not exist" << endl;
    }
  }
  freeArgs(args, num_of_args);
}
/////////////////////////////////////////////////// External Command /////////////////////////////////////////////////

ExternalCommand::ExternalCommand(const char* cmd_line, const char* original_cmd ,  bool is_alarm) : Command(cmd_line,original_cmd), is_alarm(is_alarm) {}

void ExternalCommand::execute() {
  string trimmed_command = _trim(string(cmd_line));
  string org_cmd_line(original_cmd);
  char cmd_line_copy[COMMAND_ARGS_MAX_LENGTH]; //creating a new cmd_line_command to transfer to the next functions
  strcpy(cmd_line_copy, trimmed_command.c_str());

  bool is_background = _isBackgroundComamnd(cmd_line);
  if(is_background)
    _removeBackgroundSign(cmd_line_copy);
  
  char exe[] = "/bin/bash";
  char exe_name[] = "/bin/bash";
  char flag[] = "-c";

  char *fork_args[] = {exe_name, flag, cmd_line_copy, nullptr};

  pid_t pid = fork(); 
  if(pid == 0) { //son
    if(setpgrp() == SYS_FAIL){
      perror("smash error: setpgrp failed");
      return;
    }   
    if(execv(exe,fork_args) == SYS_FAIL) {   
      perror("smash error: execv failed"); //WE NEED TO CHANGE TO exit() COMMAND
      return;
    }
    
  }else {
    SmallShell& smash = SmallShell::getInstance();
    if(smash.is_pipe)
      waitpid(pid, nullptr, WUNTRACED);
    else if(is_background) {
      if (is_alarm) {
        if(strcmp(cmd_line,original_cmd ) != 0){
          smash.job_list.addJob(this, pid, original_cmd , true);
          smash.alarm_list.add_alarm(cmd_line, smash.current_duration, pid);
        }
      }
      else{
        smash.job_list.addJob(this, pid , original_cmd);
        is_alarm = false;
      }
    }
    else {
      smash.current_process = pid;
      smash.current_cmd = cmd_line;
      if (is_alarm) {
        is_alarm = true;
        smash.fg_alarm = true;
      }
      int status_p;
      if (waitpid(pid, &status_p, WUNTRACED) == SYS_FAIL) {
        perror("smash error: waitpid failed");
        return;
      }
    }
  }


}



/////////////////////////////////// Redirection Commands: ////////////////////////////////////


RedirectionCommand :: RedirectionCommand (const char* cmd_line,const char* original_cmd) : Command(cmd_line, original_cmd) {
  string cmd_line_copy(cmd_line);
  int args_num;
  char** args = init_args(this->cmd_line, &args_num);
  string s = string(cmd_line);
  string character = s.find(">>") != std::string::npos ? ">>" : ">";

  string trimmed_command = _trim(s.substr(0, s.find(character))) ; 
  command = (char*)malloc(sizeof(char) * (strlen(trimmed_command.c_str()) +1 ));
  strcpy(command, trimmed_command.c_str());

  string trimmed_filename = _trim(s.substr(s.find(character) + character.length(), s.length()));
  if (_isBackgroundComamnd(trimmed_filename.c_str())){
    char trimmed_copy[FILENAME_MAX];
    strcpy(trimmed_copy, trimmed_filename.c_str());
    _removeBackgroundSign(trimmed_copy);
    trimmed_filename = _trim(string(trimmed_copy));
  }

  filename = (char*)malloc(sizeof(char) * (strlen(trimmed_filename.c_str()) +1));
  strcpy(filename, trimmed_filename.c_str());

  char doubled_arrows[] = ">>";

  if (strcmp(character.c_str(), doubled_arrows) == 0){
    append = true;

  } else {
    append = false;
  }
    stdout_copy = dup(1);
  if (close(1) == SYS_FAIL){
    perror("smash error: close failed");
    return;
  }
  if (append) {
    fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0655);
  } else {
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0655);

  }
  
  if (fd == SYS_FAIL) {
    perror("smash error: open failed");
    redirect_command_success = false;
  } else {
    redirect_command_success = true;
  }
  freeArgs(args,args_num);

}

void RedirectionCommand::execute() {
  if (redirect_command_success){
    SmallShell& shell = SmallShell::getInstance();
    shell.executeCommand(command,cmd_line);
  }
    free(filename);
  free(command);
  if (redirect_command_success){
    if (close(fd) == SYS_FAIL) {
      perror("smash error: close failed");
    }
  }
  if (dup2(stdout_copy, 1) == SYS_FAIL) {
    perror("smash error: dup2 failed");
  }

  if (close(stdout_copy) == SYS_FAIL) {
    perror("smash error: close failed");
  }
}


PipeCommand::PipeCommand(const char* cmd_line, const char* original_cmd) : Command(cmd_line, original_cmd) {
  string my_string = string(cmd_line);
  character = my_string.find("|&") == std::string::npos ? "|" : "|&";
  command_1 = my_string.substr(0, my_string.find(character));
  if (character == "|") {
    command_2 = my_string.substr(my_string.find(character) + 1, my_string.length());
  } else {
    command_2 = my_string.substr(my_string.find(character) + 2, my_string.length());
  }
}

void PipeCommand::execute() {
  int fields[2];
  pipe(fields);
  SmallShell& shell = SmallShell::getInstance();
  pid_t pid1 = fork(), pid2;
  if (pid1 == SYS_FAIL){
    perror("smash error: fork failed");
    if (close(fields[0]) == SYS_FAIL) {
      perror("smash error: close failed");
    }
    if (close(fields[1]) == SYS_FAIL) {
      perror("smash error: close failed");
    }
    return;
  }

  if (pid1 == 0) {//son 
    if(setpgrp() == SYS_FAIL) {
      perror("smash error: setpgrp failed");
      if (close(fields[0]) == SYS_FAIL) {
        perror("smash error: close failed");
      }
      if (close(fields[1]) == SYS_FAIL) {
        perror("smash error: close failed");
      }
      exit(1);
    }
    if (character == "|") {
      if (dup2(fields[1], 1) == SYS_FAIL) {
        perror("smash error: dup2 failed");
        if (close(fields[0]) == SYS_FAIL) {
          perror("smash error: close failed");
        }
        if (close(fields[1]) == SYS_FAIL){
          perror("smash error: close failed");
        }
        exit(1);
      }
    } else {
      if (dup2(fields[1], 2) == SYS_FAIL) {
        perror("smash error: dup2 failed");
        if (close(fields[0]) == SYS_FAIL){
          perror("smash error: close failed");
        }
        if (close(fields[1] == SYS_FAIL)) {
          perror("smash error: close failed");
        }
        exit(1);
      }
    }

    if (close(fields[0]) == SYS_FAIL) {
      perror("smash error: close failed");
    }
    if (close(fields[1]) == SYS_FAIL) {
      perror("smash error: close failed");
    }
    shell.is_pipe = true;
    shell.executeCommand(command_1.c_str(), cmd_line);
    exit(0);

  }else{
  
    pid2 = fork();

    if (pid2 == SYS_FAIL) {
      perror("smash error: fork failed");
      if (close(fields[0]) == SYS_FAIL) {
        perror("smash error: close failed");
      }
      if (close(fields[1]) == SYS_FAIL) {
        perror("smash error: close failed");
      }
      exit(1);
    }

    if (pid2 == 0) {//its the second son
      if (setpgrp() == SYS_FAIL) {
        perror("smash error: setpgrp failed");
        if (close(fields[0]) == SYS_FAIL) {
          perror("smash error: colse failed");
        }
        if (close(fields[1]) == SYS_FAIL) {
          perror("smash error: colse failed");
        }
        exit(1);
      } 

      if (dup2(fields[0], 0) == SYS_FAIL) {
        perror("smash error: dup2 failed");
        if (close(fields[0]) == SYS_FAIL) {
          perror("smash error: close failed");
        }
        if (close(fields[1]) == SYS_FAIL) {
          perror("smash error: close failed");
        }

        exit(1);
      }

      if (close(fields[0]) == SYS_FAIL) {
        perror("smash error: close failed");
      }
      if (close(fields[1]) == SYS_FAIL) {
        perror("smash error: close failed");
      }

      shell.is_pipe = true;
      shell.executeCommand(command_2.c_str(), cmd_line);
      exit(0);

    }else{
      if(close(fields[0]) == SYS_FAIL){
        perror("smash error: close failed");
      }

      if(close(fields[1]) == SYS_FAIL){
        perror("smash error: close failed");
      }


      if(waitpid(pid1, nullptr, WUNTRACED) == SYS_FAIL){
        perror("smash error: waitpid failed");
        return;
      }
      if(waitpid(pid2, nullptr, WUNTRACED) == SYS_FAIL){
        perror("smash error: waitpid failed");
        return;
      }

      return;
    }
  }
}




//////////////////////////////////// BONUS FUNCTIONS ////////////////////////////////////


TimeoutCommand::TimeoutCommand(const char* cmd_line, const char* original_cmd) : Command(cmd_line, original_cmd) {}

void TimeoutCommand::execute() {
  int number_of_args;
  char **args = init_args(cmd_line, &number_of_args);
  if (!args) {
    perror("smash error: malloc failed");
    return;
  }
  if (number_of_args <= 2) {
    cerr << "smash error: timeout: invalid arguments" << endl;
    freeArgs(args, number_of_args);
    return;
  }
  int Delay;
  try {
    if (!isNumer(args[1])) {
      throw exception();
    }
    Delay = stoi(args[1]);
  } catch (exception&) {
    cerr << "smash error: timeout: invalid arguments" << endl;
    freeArgs(args, number_of_args);
    return;
  }
  SmallShell &shell = SmallShell::getInstance ();
  alarm(Delay);
  string org_cmd_line(original_cmd);
  string newCmdLine;
  for (int i = 2; i < number_of_args; i++)
  {
    newCmdLine.append(string(args[i]));
    newCmdLine.append( " " );
  }
  shell.current_duration = Delay;
  char cmdLineCopy[COMMAND_ARGS_MAX_LENGTH];
  strcpy(cmdLineCopy, cmd_line);
  shell.current_alarm_cmd = string(cmdLineCopy);
  shell.executeCommand(newCmdLine.c_str(), org_cmd_line.c_str(), true );
  freeArgs(args, number_of_args); 
}




