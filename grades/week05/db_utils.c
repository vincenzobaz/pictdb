/* ** NOTE: undocumented in Doxygen
 * @file db_utils.c
 * @brief implementation of several tool functions for pictDB
 *
 * @date 2 Nov 2015
 */

#include "pictDB.h"
#include <stdint.h> // for uint8_t
#include <stdio.h> // for sprintf
#include <openssl/sha.h> // for SHA256_DIGEST_LENGTH
#include <inttypes.h>

/********************************************************************//**
 * Human-readable SHA
 */
static void
sha_to_string (const unsigned char* SHA,
               char* sha_string)
{
    if (SHA == NULL) {
        return;
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(&sha_string[i*2], "%02x", SHA[i]);
    }

    sha_string[2*SHA256_DIGEST_LENGTH] = '\0';
}

/********************************************************************//**
 * pictDB header display.
 */
void print_header(const struct pictdb_header header)
{
    printf("*****************************************\n"
           "**********DATABASE HEADER START**********\n"
           "DB NAME: %31s\n"
           "VERSION: %" PRIu32 "\n"
           "IMAGE COUNT: %" PRIu32 "\t\tMAX IMAGES: %" PRIu32 "\n"
           "THUMBNAIL: %" PRIu16 " x %" PRIu16 "\t"
           "SMALL: %" PRIu16 " x %" PRIu16 "\n"
           "***********DATABASE HEADER END***********\n"
           "*****************************************\n",
           header.db_name, header.db_version, header.num_files,
           header.max_files, header.res_resized[0], header.res_resized[1],
           header.res_resized[2], header.res_resized[3]);
}

/********************************************************************//**
 * Metadata display.
 */

/*
 *
 * @brief Prints the content of a metadata object
 *
 * @param metadata In memory object representing the metadata
 * describing an image.
 */
void print_metadata (const struct pict_metadata metadata)
{
    char sha_printable[2 * SHA256_DIGEST_LENGTH + 1];
    sha_to_string(metadata.SHA, sha_printable);

    printf(
        "PICTURE ID: %s\n"
        "SHA: %s\n"
        "VALID: %" PRIu16 "\n"
        "UNUSED: %" PRIu16 "\n"
        "OFFSET ORIG. : %" PRIu64 "\t\tSIZE ORIG. : %" PRIu32 "\n"
        "OFFSET THUMB.: %" PRIu64 "\t\tSIZE THUMB.: %" PRIu32 "\n"
        "OFFSET SMALL : %" PRIu64 "\t\tSIZE SMALL : %" PRIu32 "\n"
        "ORIGINAL: %" PRIu32 " x %" PRIu32 "\n"
        "*****************************************\n",
        metadata.pict_id, sha_printable, metadata.is_valid,
        metadata.unused_16,
        metadata.offset[RES_ORIG], metadata.size[RES_ORIG],
        metadata.offset[RES_THUMB], metadata.size[RES_THUMB],
        metadata.offset[RES_SMALL], metadata.size[RES_SMALL],
        metadata.res_orig[0], metadata.res_orig[1]);
}
