/**
 * @file pictDB.h
 * @brief Main header file for pictDB core library.
 *
 * Defines the format of the data structures that will be stored on the disk
 * and provides interface functions.
 *
 * The picture database starts with exactly one header structure
 * followed by exactly pictdb_header.max_files metadata
 * structures. The actual content is not defined by these structures
 * because it should be stored as raw bytes appended at the end of the
 * database file and addressed by offsets in the metadata structure.
 *
 * @author Mia Primorac
 * @date 2 Nov 2015
 */

#ifndef PICTDBPRJ_PICTDB_H
#define PICTDBPRJ_PICTDB_H

#include "error.h" /* not needed here, but provides it as required by
                    * all functions of this lib.
                    */
#include <stdio.h>       // for FILE
#include <stdint.h>      // for uint32_t, uint64_t
#include <openssl/sha.h> // for SHA256_DIGEST_LENGTH
#include <string.h>      // for strcmp, strncpy, strlen
#include <stdlib.h>      // for calloc and malloc
#include <inttypes.h>    // For printing types int stdint

#define CAT_TXT "EPFL PictDB binary"

/* constraints */
#define MAX_DB_NAME   31      // max. size of a PictDB name
#define MAX_PIC_ID    127     // max. size of a picture id
#define MAX_MAX_FILES 100000  // max. size of a database

/* For is_valid in pictdb_metadata */
#define EMPTY     0
#define NON_EMPTY 1

// pictDB library internal codes for different picture resolutions.
#define RES_THUMB 0
#define RES_SMALL 1
#define RES_ORIG  2
#define NB_RES    3

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief The header of the database, containing
 *        the pictDB configuration information.
 */
struct pictdb_header {
    /**
     * @brief Name of the picture database.
     */
    char db_name[MAX_DB_NAME + 1];
    /**
     * @brief Version of the database.
     */
    uint32_t db_version;
    /**
     * @brief Number of images contained in the database.
     */
    uint32_t num_files;
    /**
     * @brief Maximal number of images the database can contain.
     */
    uint32_t max_files;
    /**
     * @brief Maximal resolutions of the thumbnail and small images.
     */
    uint16_t res_resized[2 * (NB_RES - 1)];
    /**
     * @brief Unused 32 bit integer.
     */
    uint32_t unused_32;
    /**
     * @brief Unused 64 bit integer.
     */
    uint64_t unused_64;
};

/**
 * @brief The metadata of an image.
 */
struct pict_metadata {
    /**
     * @brief Name (identifier) of the image.
     */
    char pict_id[MAX_PIC_ID + 1];
    /**
     * @brief Hash code of the image.
     */
    unsigned char SHA[SHA256_DIGEST_LENGTH];
    /**
     * @brief Resolution of the original image.
     */
    uint32_t res_orig[2];
    /**
     * @brief Memory sizes (in bytes) of the resized images.
     */
    uint32_t size[NB_RES];
    /**
     * @brief Positions of the resized images in the database.
     */
    uint64_t offset[NB_RES];
    /**
     * @brief Indicates if the image is still in use (NON_EMPTY)
     *        or was erased (EMPTY).
     */
    uint16_t is_valid;
    /**
     * @brief Unused 16 bit integer.
     */
    uint16_t unused_16;
};

/**
 * @brief An image database.
 */
struct pictdb_file {
    /**
     * @brief File containing the database.
     */
    FILE* fpdb;
    /**
     * @brief Header of the database.
     */
    struct pictdb_header header;
    /**
     * @brief Metadata of the images.
     */
    struct pict_metadata* metadata;
};

/**
 * @brief Prints a database header informations.
 *
 * @param header The header to be displayed.
 */
void print_header(const struct pictdb_header* header);


/**
 * @brief Prints picture metadata informations.
 *
 * @param metadata The metadata of one picture.
 */
void print_metadata(const struct pict_metadata* metadata);


/**
 * @brief Displays (on stdout) pictDB metadata.
 *
 * @param db_file In memory structure with header and metadata.
 */
void do_list(const struct pictdb_file* db_file);


/**
 * @brief Creates the database called db_filename. Writes the header and the
 *        preallocated empty metadata array to database file.
 *
 * @param filename Path to the file we want to write to.
 * @param db_file In memory structure with header and metadata.
 */
int do_create(const char* filename, struct pictdb_file* db_file);

/**
 * @brief Open file containing, reads its content and writes it in memory
 *
 * @param filename The filename(path) of the file to read.
 * @param mode The opening mode e.g. read binary, write binary...
 * @param db_file The in memory structure of a database file to be filled
 * with data coming from the file.
 * @return 0 if no errors occur, an int coded in error.h in case of errors
 */
int do_open(const char* filename, const char* mode,
            struct pictdb_file* db_file);

/**
 * @brief Closes the stream in the in memory database file.
 *
 * @param db_file The in memory structure of a database file whose stream
 * is to be closed.
 */
void do_close(struct pictdb_file* db_file);

/**
 * @brief Deletes an image from a database
 *
 * @param db_file The in memory structure representing a database.
 * @param pict_id The ID of the image to remove from the database.
 * @return 0 if the deletion was successful, an error code otherwise.
 */
int do_delete(struct pictdb_file* db_file, const char* pict_id);

/**
 * @brief Converts a string to a resolution code.
 *
 * Valid codes are: RES_THUMB, RES_SMALL, RES_ORIG.
 *
 * @param resolution The string to convert.
 * @return A valid resolution code if the conversion was successful, -1 otherwise.
 */
int resolution_atoi(const char* resolution);

/**
 * @brief Reads an image from a database, resizes it in the asked resolution
 *        if need be and saves it to memory.
 *
 * @param pict_id       The ID of the image.
 * @param resolution    The resolution of the image.
 * @param image_address The memory address of the image.
 * @param size          The size of the image, in bytes.
 * @param db_file       The database.
 * @return 0 if the read was successful, an error code otherwise.
 */
int do_read(const char* pict_id, int resolution, const char** image_address,
            uint32_t* size, struct pictdb_file* db_file);

/**
 * @brief Adds an image to a database.
 *
 * @param new_image The image to insert.
 * @param size      The size of the image.
 * @param pict_id   The ID of the image.
 * @param db_file   The database.
 * @return 0 if the insertion was successful, an error code otherwise.
 */
int do_insert(const char* new_image, size_t size, const char * pict_id,
              struct pictdb_file* db_file);

#ifdef __cplusplus
}
#endif
#endif
