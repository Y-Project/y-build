#include "../../y-lib/src/data.c"
#include "../../y-lib/src/safe_concat.c"

struct Dependency {
    char* name;
    char* version;
    char* source;
    char* link;
};

struct Dependency* dep_from_entries(struct Data* entries, size_t num_entries, char* name) {
    char* search_str = safe_strcat(name, "_version");
    size_t version_index = find_by_name(entries, num_entries, search_str);
    free(search_str);
    if (version_index == -1) {
        return NULL;
    }

    search_str = safe_strcat(name, "_source");
    size_t source_index = find_by_name(entries, num_entries, search_str);
    free(search_str);
    if (source_index == -1) {
        return NULL;
    }

    search_str = safe_strcat(name, "_link");
    size_t link_index = find_by_name(entries, num_entries, search_str);
    free(search_str);
    if (link_index == -1) {
        return NULL;
    }

    struct Dependency* tmp_dep = malloc(sizeof(struct Dependency));
    tmp_dep->name = name;
    tmp_dep->version = entries[version_index].value;
    tmp_dep->source = entries[source_index].value;
    tmp_dep->link = entries[link_index].value;
    return tmp_dep;
}
