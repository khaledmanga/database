#pragma once

#include "include/common.h"

struct Metadata {
  u64 root_page_id = 0;
  u64 next_page_id = 0;

  void updateNextPageId();
};

Metadata decodeMetadata(const Page &page);
Page encodeMetadata(const Metadata &metadata);
