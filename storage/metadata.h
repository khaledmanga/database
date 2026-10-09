#pragma once

#include "../common/common.h"
#include "page.h"

class Metadata {
private:
    u64 magic_number;
    u8 version;
    u16 page_size;
    u64 root_page_id;
    u64 next_page_id;

public:
    Metadata();

    void setRootPageId(u64 root_page_id);
    void setNextPageId(u64 next_page_id);
    u64 getRootPageId() const;
    u64 getMagicNumber() const;
    u64 getNextPageId() const;
    Page encode();
    Metadata decode(Page &page);
    void update_metadata(Pager &pager);
};
