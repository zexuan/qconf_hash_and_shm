#include "shm.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <string>
#include <iostream>

#define PATH "/etc/hosts"
extern int maxSlotsNum;

int main() {
    maxSlotsNum = 8000000;
    static qhasharr_t *_shm_tbl = NULL; //share memory table
    int proj_id = 1234;
    key_t shm_key;
    if ((shm_key = ftok(PATH, proj_id)) == -1) {
        std::cerr << "ftok err" << std::endl;
        return -1;
    }
    std::cout << "shm key: 0x" << std::hex << shm_key << std::endl;
    int ret = create_hash_tbl(_shm_tbl, shm_key, 0644);
    if (ret == QCONF_OK) {
        bool initRet = LRU::getInstance()->initLruMem(_shm_tbl);
        if (!initRet) {
            LOG_ERR("Init LRU memory failed");
            ret = QCONF_ERR_MEM; 
        }
    }

    for(int i = 0; i < 100000; i++) {
        hash_tbl_set(_shm_tbl, std::to_string(i), std::to_string(i));
        if (i %100 == 0) std::cout << i << std::endl;
    }

    int max_slots = 0, used_slots = 0;
    hash_tbl_get_count(_shm_tbl, max_slots, used_slots);
    std::cout << "max: " << max_slots << " used " << used_slots << std::endl;

    std::string _key = "9999";
    std::string _val = "";
    hash_tbl_get(_shm_tbl, _key, _val);

    std::cout << "key " << _key << " value " << _val << std::endl;

    return 0;
}
