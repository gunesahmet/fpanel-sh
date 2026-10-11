// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <pwd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include "Crypto.h"
#include <fstream>

#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")
// fsh --domain=domain.com --user=domain --command="ifconfig | grep 0.0.0.0"
// cat /usr/local/fpanel/etc/panel.conf > /var/www/domain.com/siktim.txt
char* username;
std::string domainname;
char* command;
std::string full_command;
struct passwd *pw;
int user_id;
int group_id;
Crypto _crypto;
std::string pwname;
std::string pwdir;
std::string shell;
std::string pwpath;
using std::ofstream;

std::string CmdOutput(std::string cmd)
{
    std::string data;
    FILE * stream;
    const int max_buffer = 8192;
    char buffer[max_buffer];
    // cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                data.append(buffer);
            }
        }
        pclose(stream);
    }
    return data;
}
//ofstream output;

// Greeting shell during startup
void init_shell()
{
    pw = getpwnam(username);
    if(pw==NULL) {
        printf("User %s does not exists!\n",username);
        exit(1);
    }
    user_id = pw->pw_uid;
    group_id = pw->pw_gid;

    char pwnamec[200];
    char pwdirc[200];
    char shellc[200];
    char pwpathc[500];

    sprintf(pwnamec, "USER=%s",pw->pw_name);
    sprintf(pwdirc, "HOME=%s",pw->pw_dir);

    sprintf(shellc, "SHELL=/bin/bash");
    sprintf(pwpathc, "PATH=/usr/bin:/bin:/vhost");
    pwname = std::string(pwnamec);
    pwdir = std::string(pwdirc);
    shell = std::string(shellc);
    pwpath = std::string(pwpathc);

    std::string domain_path = "/var/www/" + domainname;
    std::string domain_log_path = "/var/www/" + domainname + "/cron.log";
//    std::cout << domain_path << std::endl;
//    std::cout << domain_log_path << std::endl;
//    ofstream output(domain_log_path.c_str(), ofstream::out | ofstream::app);
    std::cout << "Command: <" << full_command << "> running for: " << domainname << std::endl;

    chdir(domain_path.c_str());
    if (chroot(domain_path.c_str()) != 0) {
        perror(domain_path.c_str());
        exit(-1);
    }

    setgid(group_id);
    setuid(user_id);
}

boost::program_options::variables_map commandLine(int argc, char** argv)
{
    boost::program_options::variables_map vm;

    try {
        bool t_given = false;
        bool b_given = false;

        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
                // First parameter describes option name/short name
                // The second is parameter to option
                // The third is description
                ("user,u", boost::program_options::value<std::string>(), "User")
                ("domain,d", boost::program_options::value<std::string>(), "Domain")
                ("command,c", boost::program_options::value<std::string>(), "Command")
                ;

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
//            std::cout << desc << std::endl;
            exit(-2);
        }

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return vm;
}

std::string runcmd(char** parsed)
{
    char *envps[] = { "TERM=xterm", pwname.c_str(), pwdir.c_str(), shell.c_str(), pwpath.c_str(), NULL };

    if (execvpe(parsed[0], parsed, envps) < 0) {

//        std::cout << "Defined TERM: "  << envps[0] << std::endl;
//        std::cout << "Defined USER: "  << envps[1] << std::endl;
//        std::cout << "Defined HOME: "  << envps[2] << std::endl;
//        std::cout << "Defined SHELL: " << envps[3] << std::endl;
//        std::cout << "Defined PATH: "  << envps[4] << std::endl;
        char** qq = envps;
        for (int i = 0; i <  sizeof(envps)/sizeof(char*); ++i) {
//                output << i;
//                output << qq;
//                output << *qq;
               std::cout << i << std::endl;
               std::cout << qq << std::endl;
               std::cout << *qq << std::endl;
               ++qq;
        }
       std::cout << "Could not execute command...";
       perror("execvpe");
    }
//    output.close();
    std::cout << "Command is done for " << domainname << std::endl;

    return {};
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 4, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
//            openHelp();
            return 1;
        case 4:
            username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;
        default:
            break;
    }

    return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe)
{
    char* strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    }
    else {
        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

bool isExists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char** argv)
{
    std::string secret_key = "87f5e1c8c1dd33cfd9dbf2990b60d595";
    std::string secret_iv = "87f5e1c8c1dd33cf";

    boost::program_options::variables_map vm = commandLine(argc, argv);
    std::string command;

    if (vm.count("domain")) {
        domainname = vm["domain"].as<std::string>();
    }
    else {
        std::cout << "Error domain is required" << std::endl;
        exit(1);
    }

    if (vm.count("user")) {
        std::string user = vm["user"].as<std::string>();
        username = user.c_str();
    }
    else {
        std::cout << "Error user is required" << std::endl;
        exit(2);
    }

    if (vm.count("command")) {
        command = vm["command"].as<std::string>();
        command = _crypto.Decrypt(std::string(command), secret_key, secret_iv);
        full_command = command;
    }
    else {
        std::cout << "Error command is required" << std::endl;
        exit(3);
    }
    init_shell();

    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;

    execFlag = processString(command.c_str(), parsedArgs, parsedArgsPiped);

/**
 *
    FSH calisma prensibi
    komut bir shell script olmali ve scriptte
    #!/bin/bash
    tanimlanmalidir.
    ornek komut dosyasi test.sh ise ve yolu direk kok dizinde(/var/www/domain.com/) ise ./test.sh olarak cagirilmali
    eger /var/www/domain.com/vhost icinde ise ./vhost/test.sh ve vhost/test.sh olarak cagirilmalidir.
 */
    runcmd(parsedArgs);
    return 0;
}