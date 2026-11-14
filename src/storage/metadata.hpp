#ifndef META_META_HPP
#define META_META_HPP

#include "include/common.hpp"

struct Metadata {
  u8 root_page_id;
  u8 next_page_id;
};

Page convertMetadataToPage(const Metadata &metadata);
Metadata convertPageToMetadata(const Page &page);

#endif