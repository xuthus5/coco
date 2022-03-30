#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define COCO_TYPE_HASH (coco_hash_get_type())

G_DECLARE_FINAL_TYPE(CocoHash, coco_hash, COCO, HASH, AdwBin)

G_END_DECLS

gchar * sum_md5_32(GBytes *);

gchar * sum_md5_16(GBytes *);

gchar * sum_sha1(GBytes *);

gchar * sum_sha256(GBytes *);

gchar * sum_sha512(GBytes *);
