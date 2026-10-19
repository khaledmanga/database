#include "tree.h"

Metadata readMetadata() {
    Page page = readPage(0);
    
    Metadata metadata;
    
    metadata.decode(page);
    
    return metadata;
}