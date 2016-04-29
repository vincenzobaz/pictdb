#include "pictDB.h"
#include "image_content.h"

size_t get_index(struct pictdb_file* db_file, const char* picID) {
    for (size_t i = 0; i < db_file->header.max_files; ++i) {
        if (db_file->metadata[i].is_valid == NON_EMPTY
            && strcmp(picID, db_file->metadata[i].pict_id) == 0) {
            return i;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    --argc;
    ++argv;

    struct pictdb_file db_file;
    do_open(argv[0], "rb+", &db_file);

    size_t index = get_index(&db_file, argv[1]);
    lazily_resize(RES_THUMB, &db_file, index);
    lazily_resize(RES_SMALL, &db_file, index);

    do_list(&db_file);

    do_close(&db_file);

    return 0;
}
