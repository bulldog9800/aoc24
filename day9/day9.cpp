#include "../aoc_utils.hpp"

class DiskFragmenter {
    struct FileIdAndSize {
        FileIdAndSize(int id, int size) : m_id(id), m_size(size){};

        int m_id{-1};

        int m_size{-1};
    };

public:
    void initFromLine(vector<string>& rLines) {
        assert(rLines.size() == 1);
        string diskMap = rLines[0];

        for (int i = 0; i < diskMap.size(); i++) {
            int numOfBlocks = diskMap[i] - '0';
            int startingIdx = m_blocks.size();
            /* Free blocks. */
            if (i % 2 == 1) {
                m_freeBlocks.insert(ii(startingIdx, numOfBlocks));

                for (int j = 0; j < numOfBlocks; j++) {
                    m_blocks.push_back(Block(BlockType::Free));
                }
                continue;
            }

            /* File blocks */
            int fileId = i / 2;
            m_files.insert({startingIdx, FileIdAndSize(fileId, numOfBlocks)});
            for (int j = 0; j < numOfBlocks; j++) {
                m_blocks.push_back(Block(BlockType::File, fileId));
            }
        }
    }

    void tryToMoveFile(map<int, FileIdAndSize>::reverse_iterator& file) {
        int fileStartIdx = file->first;
        int fileSize = file->second.m_size;
        int fileId = file->second.m_id;
        int freeBlockStartIdx = -1;
        int freeBlockSize = -1;
        bool movedFile = false;

        for (auto& freeBlock : m_freeBlocks) {
            freeBlockStartIdx = freeBlock.first;
            freeBlockSize = freeBlock.second;

            if (freeBlockStartIdx > fileStartIdx) {
                /* We are now searching for blocks that are to the right of
                 * the file, stop searching. */
                break;
            }

            if (freeBlockSize < fileSize) {
                /* The free block is smaller than the file, continue
                 * searching. */
                continue;
            }

            /* The file fits inside the free block, insert it. */
            for (int i = 0; i < fileSize; i++) {
                m_blocks[fileStartIdx + i].m_type = BlockType::Free;
                m_blocks[freeBlockStartIdx + i].m_type = BlockType::File;
                m_blocks[freeBlockStartIdx + i].m_id = file->second.m_id;
            }
            movedFile = true;
            break;
        }

        if (movedFile) {
            /* Now update the free blocks list by removing the old entry and
             * adding a one if there are free blocks left. */
            m_freeBlocks.erase(freeBlockStartIdx);
            if (freeBlockSize - fileSize > 0) {
                m_freeBlocks.insert(
                    ii(freeBlockStartIdx + fileSize, freeBlockSize - fileSize));
            }
        }
    }

    void defragment() {
        /* In the defragmentation process we go over all of the files from the
         * end to the beginning, and try to find a fit for each file, if we
         * don't find any we skip. */
        for (auto file = m_files.rbegin(); file != m_files.rend(); ++file) {
            tryToMoveFile(file);
        }
    }

    UINT64 calcChecksum() {
        UINT64 checksum = 0;
        for (int i = 0; i < m_blocks.size(); i++) {
            if (m_blocks[i].m_type == BlockType::Free) {
                continue;
            }

            checksum += i * m_blocks[i].m_id;
        }

        return checksum;
    }

private:
    enum class BlockType { File, Free };

    struct Block {
        Block(BlockType type) : m_type(type){};
        Block(BlockType type, int id) : m_type(type), m_id(id){};

        BlockType m_type;
        int m_id{-1};
    };

    void moveLeftIt(int* pLeftIt) {
        /* Move the left iterator to the next free block. */
        while (*pLeftIt < m_blocks.size() &&
               m_blocks[*pLeftIt].m_type == BlockType::File) {
            (*pLeftIt)++;
        }
    }

    void moveRightIt(int* pRightIt) {
        /* Move the right iterator backwards to the last file block. */
        while (*pRightIt >= 0 &&
               m_blocks[*pRightIt].m_type == BlockType::Free) {
            (*pRightIt)--;
        }
    }

    vector<Block> m_blocks;

    /** A map that will contain entries (startIdx, numOfBlocks) which represent
     * numOfBlocks of continuous free blocks. */
    map<int, int> m_freeBlocks;

    /** A map that will contain entries (startIdx, (fileId, numOfBlocks)) which
     * represent a file that starts at startIdx with a fileId and how many
     * blocks it contains. */
    map<int, FileIdAndSize> m_files;
};

int main() {
    InputReader inputReader;
    inputReader.readFile("/Users/whysoserious/repos/aoc24/day9/input.txt");
    vector<string>& rLines = inputReader.getLines();

    DiskFragmenter diskFragmenter;
    diskFragmenter.initFromLine(rLines);

    diskFragmenter.defragment();

    cout << diskFragmenter.calcChecksum() << endl;

    return 0;
}
