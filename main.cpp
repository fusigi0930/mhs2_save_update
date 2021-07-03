#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>

#define ADDR_MONEY                  0x48
#define ADDR_ITEM_BEGIN             0x54
#define ADDR_ITEM_FLAG_BEGIN        0x12b68

namespace fs = std::experimental::filesystem;

struct __attribute__((__packed__)) SItem {
    uint16_t id;
    uint16_t count;
    uint32_t reseve;
};

void update_item(unsigned char* buf, uint16_t id, uint16_t count) {
    if (nullptr == buf) return;

    unsigned char* item_addr = buf + ADDR_ITEM_BEGIN + ((id - 1) * sizeof(SItem));
    SItem *item = reinterpret_cast<SItem *>(item_addr);
    item->id = id;
    item->count = count;

    unsigned char* flag_addr = buf + ADDR_ITEM_FLAG_BEGIN + (id / 8);
    unsigned char bits = static_cast<unsigned char>(1) << (id % 8);
    *flag_addr = *flag_addr | bits;
}

int main(int argc, char *argv[])
{

    if (argc != 2) {
        std::cout << "syntax: <command> <save file>" << std::endl;
        return 0;
    }

    fs::path p(argv[1]);
    if (!fs::exists(p)) {
        std::cout << "file: " << argv[1] << " is not exist!" << std::endl;
        return 0;
    }

    std::string szBak = argv[1];
    szBak.append(".super.bak");

    std::ifstream f(argv[1], std::ios::binary);
    std::vector<unsigned char> fbuf(std::istreambuf_iterator<char>(f), {});
    f.close();

    fs::copy(argv[1], szBak.c_str());

    uint32_t *pMoney = reinterpret_cast<uint32_t*>(&fbuf[ADDR_MONEY]);
    *pMoney = 0x7fffffff;

    for (int i = i; i < 0x4C0; i++) {
        update_item(&fbuf[0], i, 999);
    }

    std::ofstream ofile(argv[1], std::ios::binary);
    ofile.write(reinterpret_cast<char*>(&fbuf[0]), fbuf.size());
    ofile.close();
}
