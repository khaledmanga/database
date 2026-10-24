#pragma once

#include "include/common.h"
#include "storage/file_manager.h"

class Metadata {
private:
  u64 root_page_id = 0;
  u64 next_page_id = 0;
  Metadata();

public:
  Metadata(const Metadata &) = delete;
  Metadata &operator=(const Metadata &) = delete;

  ~Metadata() = default;

  static Metadata &getInstance();
  void init();
  void readMetadata();
  void writeMetadata();
  void decode(const Page &page);
  u64 getRootPageId() const;
  u64 getNextPageId() const;
  void updateNextPageId();
  Page encode();
};