#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xRedisClient.h"

xRedisClient xClient;

// AP Hash Function
unsigned int APHash(const char *str)
{
    unsigned int hash = 0;
    int i;

    for (i = 0; *str; i++) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
}

#define CACHE_TYPE_1 0

void test_set(const char *strkey, const char *strValue)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(strkey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.set(dbi, strkey, strValue)) {
            printf("%s success \n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_append()
{
    test_set("test", "hello");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.append(dbi, szKey, " xsky")) {
            printf("%s success \n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_decr()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.decr(dbi, szKey, res)) {
            if (res == 99) {
                printf("%s success %ld \n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_decrby()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.decrby(dbi, szKey, 11, res)) {
            if (res == 89) {
                printf("%s success %ld \n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_incr()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.incr(dbi, szKey, res)) {
            if (res == 101) {
                printf("%s success %ld \n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_incrby()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.incrby(dbi, szKey, 11, res)) {
            if (res == 111) {
                printf("%s success %ld \n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_get()
{

    char szKey[256] = {0};
    {
        strcpy(szKey, "kenan");
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        if (bRet) {
            string strData;
            if (xClient.get(dbi, szKey, strData)) {
                printf("%s success data:%s \n", __PRETTY_FUNCTION__, strData.c_str());
            } else {
                printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
    }

    {
        sprintf(szKey, "test_%u", (unsigned int)time(NULL));
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        if (bRet) {
            string strData;
            if (xClient.get(dbi, szKey, strData)) {
                printf("%s error data:%s \n", __PRETTY_FUNCTION__, strData.c_str());
            } else {
                printf("%s success [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
    }
}

void test_getrange()
{
    test_set("test", "01234567890123456789");
    char szKey[256] = {0};

    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        string strData;
        if (xClient.getrange(dbi, szKey, 2, 6, strData)) {
            printf("%s success data:%s \n", __PRETTY_FUNCTION__, strData.c_str());
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_exists()
{
    char szKey[256] = {0};
    strcpy(szKey, "kenan");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.exists(dbi, szKey)) {
            printf("%s success \n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_del()
{
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.del(dbi, szKey)) {
            printf("%s success \n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_mset()
{
    char szKey[256] = {0};
    DBIArray vdbi;
    VDATA    kvData;

    for (int i = 0; i < 10; i++) {
        RedisDBIdx dbi(&xClient);
        sprintf(szKey, "mset_key_%d", i);
        dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        vdbi.push_back(dbi);
        kvData.push_back(szKey);
        sprintf(szKey, "mset_value_%d", i);
        kvData.push_back(szKey);
    }

    if (xClient.mset(vdbi, kvData)) {
        printf("%s success \n", __PRETTY_FUNCTION__);
    } else {
        printf("%s error [%s] \n", __PRETTY_FUNCTION__, "mset error");
    }
}

void test_mget()
{
    char szKey[256] = {0};
    DBIArray vdbi;
    KEYS     kData;
    ReplyData Reply;
    for (int i = 0; i < 15; i++) {
        RedisDBIdx dbi(&xClient);
        sprintf(szKey, "mset_key_%d", i);
        dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        vdbi.push_back(dbi);
        kData.push_back(szKey);
    }

    if (xClient.mget(vdbi, kData, Reply)) {
        printf("%s success %d \n", __PRETTY_FUNCTION__, Reply.size());
        ReplyData::iterator iter = Reply.begin();
        for (; iter != Reply.end(); iter++) {
            printf("%d\t%s\n", (*iter).type, (*iter).str.c_str());
        }
    } else {
        printf("%s error [%s] \n", __PRETTY_FUNCTION__, "mset error");
    }
}

void test_hset(const char *szHKey, const char *field, const char *value)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count = 0;
        if (xClient.hset(dbi, szHKey, field, value, count)) {
            printf("%s success \n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_hget(const char *szHKey)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        string strData;
        if (xClient.hget(dbi, szHKey, "BILLURL", strData)) {
            printf("%s success data:%s\n", __PRETTY_FUNCTION__, strData.c_str());
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_expire(const char *szHKey, unsigned int second)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.expire(dbi, szHKey, second)) {
            printf("%s success\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void test_ttl(const char *szHKey)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t second;
        if (xClient.ttl(dbi, szHKey, second)) {
            printf("%s success data:%ld\n", __PRETTY_FUNCTION__, second);
        } else {
            printf("%s error [%s] \n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

int main(int argc, char **argv)
{

    xClient.Init(1);

    RedisNode RedisList1[1] = {
        {0, "10.44.144.77", 6379, "", 2, 5}
    };

    xClient.ConnectRedisCache(RedisList1, 1, CACHE_TYPE_1);

    test_getrange();
    test_hset("pvid1_ip1", "BILLURL", "http://www.baidu.com/cpro.php?abcdefg");
    test_hget("pvid1_ip1");
    test_hset("pvid2_ip2", "BILLURL", "http://www.baidu.com/cpro.php?ABCDEFG");
    test_hget("pvid2_ip2");
    test_expire("pvid1_ip1", 9999);
    test_ttl("pvid1_ip1");
    test_ttl("pvid2_ip2");

    xClient.release();

    return 0;
}


