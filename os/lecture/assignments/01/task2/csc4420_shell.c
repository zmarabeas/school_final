#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_HISTORY 100
#define MAX_PROMPT 256

typedef struct {
    char **commands;
    int count;
    int capacity;
} History;

History history = {NULL, 0, 0};
char custom_prompt[MAX_PROMPT] = "CSC4420[%h]$ ";

void init_history() {
    history.capacity = 10;
    history.commands = malloc(history.capacity * sizeof(char*));
    history.count = 0;
}

void add_to_history(const char *command) {
    if (history.count >= history.capacity) {
        history.capacity *= 2;
        history.commands = realloc(history.commands, history.capacity * sizeof(char*));
    }
    history.commands[history.count] = malloc(strlen(command) + 1);
    strcpy(history.commands[history.count], command);
    history.count++;
}

void cleanup_history() {
    for (int i = 0; i < history.count; i++) {
        free(history.commands[i]);
    }
    free(history.commands);
}

void display_prompt() {
    char prompt[MAX_PROMPT];
    strcpy(prompt, custom_prompt);
    
    char history_num[20];
    sprintf(history_num, "%d", history.count + 1);
    
    char *pos = strstr(prompt, "%h");
    if (pos != NULL) {
        char new_prompt[MAX_PROMPT];
        int prefix_len = pos - prompt;
        strncpy(new_prompt, prompt, prefix_len);
        new_prompt[prefix_len] = '\0';
        strcat(new_prompt, history_num);
        strcat(new_prompt, pos + 2);
        strcpy(prompt, new_prompt);
    }
    
    printf("%s", prompt);
    fflush(stdout);
}

void cmd_url() {
    printf("Course Website: https://wayne.edu (course syllabus available)\n");
}

void cmd_hour() {
    printf("Class Times: 1:00pm to 2:15pm, Tuesday and Thursday\n");
}

void cmd_room() {
    printf("Classroom: State Hall, Room 1109\n");
}

void cmd_desp() {
    printf("Course Description:\n");
    printf("CSC 4420 Computer Operating Systems\n\n");
    printf("Operating system is an essential software layer to use various computing devices.\n");
    printf("This course covers the basic components and design principles of modern operating\n");
    printf("systems, including process and thread, CPU scheduling, memory management, file\n");
    printf("system and others. This course also provides hands-on programming experiences\n");
    printf("of using Linux system calls, and design and implementation of a cloud-based\n");
    printf("file system.\n\n");
    printf("Prerequisites: CSC 3100/3101 and CSC 2200/2201, or the instructor's permission.\n");
}

void cmd_text() {
    printf("Textbook: Modern Operating Systems, 5/E, 5th Edition, Pearson, 2022.\n");
}

void cmd_ref() {
    printf("Reference Books:\n");
    printf("- Operating System Concepts, 8th Edition, Wiley, 2008.\n");
    printf("- Understanding the Linux Kernel, 3rd Edition, O'Reilly Media, 2005.\n");
    printf("- Advanced UNIX Programming, 2nd Edition, Addison-Wesley, 2004.\n");
    printf("- Advanced Programming in the UNIX Environment, 1st Edition, Addison-Wesley, 1992 or 3rd edition, 2013.\n");
}

void cmd_prof() {
    printf("Professor: Lihao Xu\n");
}

void cmd_pol() {
    printf("Professor's Office Location: CS Building 14001.1\n");
}

void cmd_poh() {
    printf("Professor's Office Hours: 2:30pm to 3:00pm, Tuesday and Thursday\n");
}

void cmd_pma() {
    printf("Professor's Email: lihao AT wayne.edu\n");
}

void cmd_ta() {
    printf("Teaching Assistants:\n");
    printf("- Tyler LaFrance\n");
    printf("- Mona Zamiri\n");
}

void cmd_tol() {
    printf("TA Office Locations:\n");
    printf("- Tyler LaFrance: (Office location not specified)\n");
    printf("- Mona Zamiri: (Office location not specified)\n");
}

void cmd_toh() {
    printf("TA Office Hours:\n");
    printf("- Tyler LaFrance: (Office hours not specified)\n");
    printf("- Mona Zamiri: (Office hours not specified)\n");
}

void cmd_tma() {
    printf("TA Email Addresses:\n");
    printf("- Tyler LaFrance: lafrance@wayne.edu\n");
    printf("- Mona Zamiri: mona.zamiri@wayne.edu\n");
}

void cmd_history(int limit) {
    int start = 0;
    int end = history.count;
    
    if (limit > 0 && limit < history.count) {
        start = history.count - limit;
    }
    
    for (int i = start; i < end; i++) {
        printf("%d  %s\n", i + 1, history.commands[i]);
    }
}

void cmd_help() {
    printf("CSC4420 Shell - Available Commands:\n\n");
    printf("Course Information:\n");
    printf("  url      - Course website\n");
    printf("  hour     - Class times\n");
    printf("  room     - Classroom location\n");
    printf("  desp     - Course description\n");
    printf("  text     - Textbook information\n");
    printf("  ref      - Reference books\n");
    printf("  prof     - Professor's name\n");
    printf("  pol      - Professor's office location\n");
    printf("  poh      - Professor's office hours\n");
    printf("  pma      - Professor's email address\n");
    printf("  ta       - TA names\n");
    printf("  tol      - TA office locations\n");
    printf("  toh      - TA office hours\n");
    printf("  tma      - TA email addresses\n\n");
    printf("Shell Commands:\n");
    printf("  prompt <string> - Set custom prompt (use %%h for history number)\n");
    printf("  history [n]     - Show command history (optionally limit to n commands)\n");
    printf("  help            - Show this help message\n");
    printf("  exit/quit       - Exit the shell\n");
}

int process_command(char *input) {
    char *command = strtok(input, " \t\n");
    if (command == NULL) {
        return 0;
    }
    
    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
        return 1;
    } else if (strcmp(command, "url") == 0) {
        cmd_url();
    } else if (strcmp(command, "hour") == 0) {
        cmd_hour();
    } else if (strcmp(command, "room") == 0) {
        cmd_room();
    } else if (strcmp(command, "desp") == 0) {
        cmd_desp();
    } else if (strcmp(command, "text") == 0) {
        cmd_text();
    } else if (strcmp(command, "ref") == 0) {
        cmd_ref();
    } else if (strcmp(command, "prof") == 0) {
        cmd_prof();
    } else if (strcmp(command, "pol") == 0) {
        cmd_pol();
    } else if (strcmp(command, "poh") == 0) {
        cmd_poh();
    } else if (strcmp(command, "pma") == 0) {
        cmd_pma();
    } else if (strcmp(command, "ta") == 0) {
        cmd_ta();
    } else if (strcmp(command, "tol") == 0) {
        cmd_tol();
    } else if (strcmp(command, "toh") == 0) {
        cmd_toh();
    } else if (strcmp(command, "tma") == 0) {
        cmd_tma();
    } else if (strcmp(command, "history") == 0) {
        char *arg = strtok(NULL, " \t\n");
        int limit = arg ? atoi(arg) : 0;
        cmd_history(limit);
    } else if (strcmp(command, "help") == 0) {
        cmd_help();
    } else if (strcmp(command, "prompt") == 0) {
        char *new_prompt = strtok(NULL, "\n");
        if (new_prompt != NULL) {
            while (*new_prompt == ' ' || *new_prompt == '\t') new_prompt++;
            strncpy(custom_prompt, new_prompt, MAX_PROMPT - 1);
            custom_prompt[MAX_PROMPT - 1] = '\0';
            printf("Prompt set to: %s\n", custom_prompt);
        } else {
            printf("Usage: prompt <string>\n");
        }
    } else {
        printf("Unknown command: %s\n", command);
        printf("Type 'help' for available commands.\n");
    }
    
    return 0;
}

int main() {
    char input[MAX_INPUT];
    
    init_history();
    
    printf("CSC4420 Shell - Fall 2025\n");
    printf("Type 'help' for available commands.\n\n");
    
    while (1) {
        display_prompt();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        if (strlen(input) > 1) {
            input[strcspn(input, "\n")] = '\0';
            add_to_history(input);
            
            char input_copy[MAX_INPUT];
            strcpy(input_copy, input);
            
            if (process_command(input_copy)) {
                break;
            }
        }
    }
    
    printf("Goodbye!\n");
    cleanup_history();
    return 0;
}