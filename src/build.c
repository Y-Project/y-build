#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "error.c"
#include "dependency.c"
#include "bootstrap_include.h"

int main(int argc, char** argv) {
    // Check argument count and display help message if insuficcient
    if (argc < 2) {
        print_help();
        return 1;
    // Check wether the first argument was "build" or "b", if so start the build process
    } else if (strcmp(argv[1], "build") == 0 || strcmp(argv[1], "b") == 0) {
        // Open file and abort if not found
        FILE* buildfile;
        if ((buildfile = fopen("y.build", "r")) == NULL) {
            print_missing_buildfile();
            return 1;
        }

        // Read the file into a string
        size_t length;
        char* buffer = safe_read(buildfile, 1048576, &length);
        if (buffer == NULL) {
            print_buildfile_too_large();
            return 1;
        }

        // Parse string into list of entries
        size_t num_entries = 0;
        struct Data* entries = y_data_parse(buffer, &num_entries);

        // Try to find the "name" entry, abort if not found, same with version
        size_t name_index = find_by_name(entries, num_entries, "name");
        if (name_index == -1) {
            print_entry_not_found("name");
            free(entries);
            free(buffer);
            return 1;
        }
        size_t version_index = find_by_name(entries, num_entries, "version");
        if (version_index == -1) {
            print_entry_not_found("version");
            free(entries);
            free(buffer);
            return 1;
        }

        printf("Starting to build %s@%s...\n", entries[name_index].value, entries[version_index].value);

        // Find dependies entry
        size_t dependencies_index = find_by_name(entries, num_entries, "dependencies");
        size_t dependency_count;
        char** dependencies;
        if (dependencies_index != -1) {
            // Split the dependency string into a array
            dependencies = split_into(entries[dependencies_index].value, ",", &dependency_count);
        } else {
            dependency_count = 0;
        }
        printf("Gathering %zu dependencies...\n", dependency_count);

        rmdir_rf("build/");
        mkdir("build", 0777);
        mkdir("build/deps", 0777);

        for (size_t i = 0; i < dependency_count; i++) {
            struct Dependency* current_dep = dep_from_entries(entries, num_entries, dependencies[i]);
            if (current_dep == NULL) {
                print_dep_parsing_error(dependencies[i]);
                free(dependencies);
                free(entries);
                free(buffer);
                return 1;
            }
            if (strcmp(current_dep->source, "local") == 0) {
                mkdir(safe_strcat("build/deps/", current_dep->name), 0777);
            }

            free(current_dep);
        }

        
        free(dependencies);
        free(entries);
        free(buffer);
        return 0;
    // Check wether the first argument was "test" or "t", if so start building and executing tests
    } else if (strcmp(argv[1], "test") == 0 || strcmp(argv[1], "t") == 0) {
        printf("Test");
        return 0;
    // Check wether the first argument was "run" or "r", if so start the build process and run the program
    } else if (strcmp(argv[1], "run") == 0 || strcmp(argv[1], "r") == 0) {
        printf("Run");
        return 0;
    // Check wether the first argument was "init" or "i", if so initialize a build file
    } else if (strcmp(argv[1], "init") == 0 || strcmp(argv[1], "i") == 0) {
        printf("Init");
        return 0;
    // If no valid command was found print the unknown command and all known ones
    } else {
        print_unknown_command(argv[1]);
            print_help();
            return 1;
    }
}
