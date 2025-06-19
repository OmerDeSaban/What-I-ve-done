#define _GNU_SOURCE
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>

// arglist - a list of char* arguments (words) provided by the user
// it contains count+1 items, where the last item (arglist[count]) and *only* the last is NULL
// RETURNS - 1 if should continue, 0 otherwise
int process_arglist(int count, char** arglist);

// prepare and finalize calls for initialization and destruction of anything required
int prepare(void);
int finalize(void);

int main(void) {
	if (prepare() != 0) { exit(1); }
	
	while (true) {
		char** arglist = nullptr;
		char* line = nullptr;
		size_t size = 0;
		int count = 0;

		if (getline(&line, &size, stdin) == -1) {
			free(line);
			break;
		}
    
		arglist = static_cast<char**>(malloc(sizeof(char*)));
		if (arglist == nullptr) {
			std::cerr << "malloc failed: " << strerror(errno) << std::endl;
			exit(1);
		}
		arglist[0] = strtok(line, " \t\n");
    
		while (arglist[count] != nullptr) {
			++count;
			arglist = static_cast<char**>(realloc(arglist, sizeof(char*) * (count + 1)));
			if (arglist == nullptr) {
				std::cerr << "realloc failed: " << strerror(errno) << std::endl;
				exit(1);
			}
      
			arglist[count] = strtok(nullptr, " \t\n");
		}
    
		if (count != 0) {
			if (!process_arglist(count, arglist)) {
				free(line);
				free(arglist);
				break;
			}
		}
    
		free(line);
		free(arglist);
	}
	
	if (finalize() != 0) { exit(1); }

	return 0;
}