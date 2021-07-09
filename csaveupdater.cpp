#include "csaveupdater.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <QDebug>

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
    if (0 == count) {
        item->id = 0;
        item->count = 0;
    }
    else {
        item->id = id;
        item->count = count;
    }

    unsigned char* flag_addr = buf + ADDR_ITEM_FLAG_BEGIN + (id / 8);
    unsigned char bits = static_cast<unsigned char>(1) << (id % 8);
    if (0 == count) {
        *flag_addr = *flag_addr & ~bits;
    }
    else {
        *flag_addr = *flag_addr | bits;
    }
}

CSaveUpdater::CSaveUpdater(QObject *parent) : QObject(parent)
{

}

CSaveUpdater::~CSaveUpdater() {

}

void CSaveUpdater::save_update(QString filename) {
    std::string fname = reinterpret_cast<char*>(filename.toUtf8().data());
    fs::path p(fname.c_str());
    if (!fs::exists(p)) {
        std::stringstream s;
        s << "file: " << fname << "dose not exist!";
        emit sigErrorMessage(s.str().c_str());
        return;
    }

    std::string szBak = p.filename().string();
    szBak.append(".super.bak");

    std::ifstream f(fname.c_str(), std::ios::binary);
    std::vector<unsigned char> fbuf(std::istreambuf_iterator<char>(f), {});
    f.close();

    std::ofstream bkfile(szBak.c_str(), std::ios::binary);
    bkfile.write(reinterpret_cast<char*>(&fbuf[0]), fbuf.size());
    bkfile.close();

    uint32_t *pMoney = reinterpret_cast<uint32_t*>(&fbuf[ADDR_MONEY]);
    *pMoney = 0x7fffffff;

    for (int i = 1; i < 0x3e8; i++) {
        update_item(&fbuf[0], static_cast<uint16_t>(i), i);
    }
    /*
    for (int i = 999; i < 0x500; i++) {
        update_item(&fbuf[0], static_cast<uint16_t>(i), i-998);
    }
    /*
    for (int i = 1; i < 0x4D0; i++) {
        update_item(&fbuf[0], static_cast<uint16_t>(i), 999);
    }
    */

    std::ofstream ofile(fname.c_str(), std::ios::binary);
    ofile.write(reinterpret_cast<char*>(&fbuf[0]), fbuf.size());
    ofile.close();
}

void CSaveUpdater::save_adjust(QString filename, QString id, QString count) {
    int nID = 0;
    if (0 == id.toLower().left(2).compare("0x"))
        nID = id.toInt(nullptr, 16);
    else
        nID = id.toInt(nullptr, 10);
    int nCount = count.toInt();
    std::string fname = reinterpret_cast<char*>(filename.toUtf8().data());
    fs::path p(fname.c_str());
    if (!fs::exists(p)) {
        std::stringstream s;
        s << "file: " << fname << "dose not exist!";
        emit sigErrorMessage(s.str().c_str());
        return;
    }

    qDebug("id: 0x%x, %s, count: %d", nID, id.toUtf8().data(), nCount);
    std::ifstream f(fname.c_str(), std::ios::binary);
    std::vector<unsigned char> fbuf(std::istreambuf_iterator<char>(f), {});
    f.close();

    uint32_t *pMoney = reinterpret_cast<uint32_t*>(&fbuf[ADDR_MONEY]);
    *pMoney = 0x7fffffff;

    update_item(&fbuf[0], static_cast<uint16_t>(nID), static_cast<uint16_t>(nCount));

    std::ofstream ofile(fname.c_str(), std::ios::binary);
    ofile.write(reinterpret_cast<char*>(&fbuf[0]), fbuf.size());
    ofile.close();
}
