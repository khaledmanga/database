#pragma once

#include "file_manager.h"
#include "../common/common.h"

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
};

class Pager {
	public:
		Pager(FileManager file);
		Page readPage(u64 page_id);
		void writePage(u64 page_id, Page &page);
}
