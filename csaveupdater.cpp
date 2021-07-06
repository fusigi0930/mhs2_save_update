#include "csaveupdater.h"
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

static void update_item(unsigned char* buf, uint16_t id, uint16_t count) {
    if (nullptr == buf) return;

    unsigned char* item_addr = buf + ADDR_ITEM_BEGIN + ((id - 1) * sizeof(SItem));
    SItem *item = reinterpret_cast<SItem *>(item_addr);
    item->id = id;
    item->count = count;

    unsigned char* flag_addr = buf + ADDR_ITEM_FLAG_BEGIN + (id / 8);
    unsigned char bits = static_cast<unsigned char>(1) << (id % 8);
    *flag_addr = *flag_addr | bits;
}

CSaveUpdater::CSaveUpdater(QObject *parent) : QObject(parent)
{

}

CSaveUpdater::~CSaveUpdater() {

}

void CSaveUpdater::save_update(QString filename) {
    std::string fname = reinterpret_cast<char*>(filename.data());
    fs::path p(fname.c_str());
    if (!fs::exists(p)) {
        emit sigErrorMessage("file dose not exist!");
        return;
    }

    std::string szBak = p.filename().string();
    szBak.append(".super.bak");

    std::ifstream f(fname.c_str(), std::ios::binary);
    std::vector<unsigned char> fbuf(std::istreambuf_iterator<char>(f), {});
    f.close();

    fs::copy_file(fname.c_str(), szBak.c_str());

    uint32_t *pMoney = reinterpret_cast<uint32_t*>(&fbuf[ADDR_MONEY]);
    *pMoney = 0x7fffffff;

    for (int i = 1; i < 0x4C0; i++) {
        update_item(&fbuf[0], static_cast<uint16_t>(i), 999);
    }

    std::ofstream ofile(fname.c_str(), std::ios::binary);
    ofile.write(reinterpret_cast<char*>(&fbuf[0]), fbuf.size());
    ofile.close();
}
