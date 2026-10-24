// #include "tree.h"

// BPlusTree *BPlusTree::search(const KV &kv, u64 &pageId) {
//   pageId = Metadata::getInstance().getRootPageId();

//   while (true) {
//     Page page{};
//     FileManager::getInstance().readPage(pageId, page);

//     BPlusTree *bpt = new BPlusTree();
//     bpt->decode(page);

//     if ((BNodeType)bpt->types == BNodeType::LEAF) {
//       for (size_t i = 0; i < bpt->KVs.size(); ++i) {
//         if (bpt->KVs[i].key == kv.key) {
//           return bpt;
//         }
//       }

//       delete bpt;
//       return nullptr;
//     }

//     size_t i = 0;
//     while (i < bpt->KVs.size() && !(kv.key < bpt->KVs[i].key)) {
//       ++i;
//     }

//     pageId = bpt->pointers[i];
//     delete bpt;
//   }
// }

// bool BPlusTree::update(const KV &kv) {
//   u64 pageId = 0;
//   BPlusTree *bpt = search(kv, pageId);

//   if (!bpt) {
//     return false;
//   }

//   for (size_t i = 0; i < bpt->KVs.size(); ++i) {
//     if (bpt->KVs[i].key == kv.key) {
//       bpt->KVs[i].value = kv.value;
//       bpt->update_offset();
//       FileManager::getInstance().writePage(pageId, bpt->encode());
//       delete bpt;
//       return true;
//     }
//   }

//   delete bpt;
//   return false;
// }

// void BPlusTree::insert(const KV &kv) {
//   u64 rootId = Metadata::getInstance().getRootPageId();
//   Page page{};
//   FileManager::getInstance().readPage(rootId, page);
//   BPlusTree root;
//   root.decode(page);

//   SplitProperty sp = insertRecursive(root, rootId, kv);

//   if (sp.split) {
//     KV promoted;
//     promoted.key = sp.promotedKey;

//     BPlusTree newRoot;
//     newRoot.types = (u16)BNodeType::INTERNAL;
//     newRoot.KVs = {promoted};
//     newRoot.pointers = {rootId, sp.rightPageId};
//     newRoot.nkeys = 1;
//     newRoot.update_offset();

//     u64 newId = Metadata::getInstance().getNextPageId();
//     Metadata::getInstance().updateNextPageId();
//     FileManager::getInstance().writePage(newId, newRoot.encode());
//     Metadata::getInstance().setRootPageId(newId);
//   }
// }

// SplitProperty BPlusTree::insertRecursive(BPlusTree &node, u64 pageId, const KV &kv) {
//   bool isLeaf = (BNodeType)node.types == BNodeType::LEAF;

//   if (isLeaf) {
//     size_t i = 0;

//     while (i < node.KVs.size() && node.KVs[i].key < kv.key) {
//       ++i;
//     }

//     node.KVs.insert(node.KVs.begin() + i, kv);
//     node.nkeys = node.KVs.size();
//     node.update_offset();
//   } else {
//     size_t i = 0;

//     while (i < node.KVs.size() && !(kv.key < node.KVs[i].key)) {
//       ++i;
//     }

//     Page page{};
//     FileManager::getInstance().readPage(node.pointers[i], page);
//     BPlusTree child;
//     child.decode(page);

//     SplitProperty sp = insertRecursive(child, node.pointers[i], kv);

//     if (!sp.split) {
//       return {};
//     }

//     KV promoted;
//     promoted.key = sp.promotedKey;
//     node.KVs.insert(node.KVs.begin() + i, promoted);
//     node.pointers.insert(node.pointers.begin() + i + 1, sp.rightPageId);
//     node.nkeys = node.KVs.size();
//     node.update_offset();
//   }

//   if (node.nkeys <= MAXIMUM_CHILDREN_IN_ONE_NODE) {
//     FileManager::getInstance().writePage(pageId, node.encode());
//     return {};
//   }

//   size_t mid = node.nkeys / 2;

//   std::vector<u16> promotedKey;
//   if (!isLeaf) {
//     promotedKey = node.KVs[mid].key;
//   }

//   BPlusTree rightNode;
//   rightNode.types = node.types;

//   u64 rightId = Metadata::getInstance().getNextPageId();
//   Metadata::getInstance().updateNextPageId();

//   size_t start = isLeaf ? mid : mid + 1;
//   size_t total = node.KVs.size();

//   for (size_t i = start; i < total; ++i) {
//     rightNode.KVs.push_back(node.KVs.back());
//     node.KVs.pop_back();
//   }

//   std::reverse(rightNode.KVs.begin(), rightNode.KVs.end());

//   if (isLeaf) {
//     promotedKey = rightNode.KVs.front().key;
//   } else {
//     node.KVs.pop_back();

//     rightNode.pointers.assign(node.pointers.begin() + mid + 1, node.pointers.end());
//     node.pointers.resize(mid + 1);
//   }

//   node.nkeys = node.KVs.size();
//   node.update_offset();
//   rightNode.nkeys = rightNode.KVs.size();
//   rightNode.update_offset();

//   FileManager::getInstance().writePage(pageId, node.encode());
//   FileManager::getInstance().writePage(rightId, rightNode.encode());

//   return SplitProperty(promotedKey, rightId);
// }