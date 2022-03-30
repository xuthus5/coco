#include "hash.h"

gchar * sum_md5_32(GBytes * body)
{
    return g_compute_checksum_for_bytes (G_CHECKSUM_MD5, body);
}

gchar * sum_md5_16(GBytes * body)
{
    gchar * t = sum_md5_32 (body);
    char cipher_substring [16];
    strncpy(cipher_substring, &t[8], 16);
    return cipher_substring;
}

gchar * sum_sha1(GBytes * body)
{
    return g_compute_checksum_for_bytes (G_CHECKSUM_SHA1, body);
}

gchar * sum_sha256(GBytes * body)
{
    return g_compute_checksum_for_bytes (G_CHECKSUM_SHA256, body);
}

gchar * sum_sha512(GBytes * body)
{
    return g_compute_checksum_for_bytes (G_CHECKSUM_SHA512, body);
}

