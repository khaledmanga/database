#pragma once

#include "file_manager.h"
#include "../common/common.h"

class Pager {
public:
    Pager(FileManager &file);

    Page readPage(u64 page_id);
    void writePage(u64 page_id, const Page &page);

private:
    FileManager &file;
};