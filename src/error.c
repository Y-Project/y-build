#include <stdio.h>

void print_help(void) {
    printf(
        "y-build COMMANDS:\n"
        "   build, b: Downloads all dependencies and builds the project.\n"
        "   test , t: Compiles all tests and runs them.\n"
        "   run  , r: Builds the programm and runs it.\n"
        "   init , i: Initializes a build file.\n"
        "   help , h: Displays this help text.\n"
        );
}

void print_unknown_command(char* command) {
    printf(
        "Unknown command: \"%s\"\n",
        command
    );
}

void print_missing_buildfile(void) {
    printf(
        "No buildfile found\n"
        "Consider creating one with \"y-build init\"\n"
    );
}

void print_buildfile_too_large(void) {
    printf(
        "Buildfile is over 1 MB\n"
        "This is most likely a severe error\n"
    );
}

void print_entry_not_found(char* missing_entry) {
    printf(
        "The entry \"%s\" was expected but not found\n"
        "Consider adding such entry\n",
        missing_entry
    );
}

void print_dep_parsing_error(char* dependency) {
    printf(
        "There was a fatal error while searching for %s\n"
        "Check if all necessary fields are present.\n",
        dependency
    );
}
