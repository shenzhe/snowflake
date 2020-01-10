#include "phpx.h"
#include <iostream>

using namespace php;
using namespace std; 

//meta begin{{{
static PHPX_METHOD(Meta, __construct)
{
    if(!args[0].isInt()) {
        throwException("RuntimeException", "dataCenterId must int", -1);
    }
    long dataCenterId = args[0].toInt();
    long workerId = args[1].toInt();
    long sequence = args[2].toInt();
    long beginTimestamp = args[3].toInt();
    _this.set("dataCenterId", dataCenterId);
    _this.set("workerId", workerId);
    _this.set("sequence", sequence);
    _this.set("beginTimestamp", beginTimestamp);
}
//meta end }}}

//snowflake start {{{

static PHPX_METHOD(Snowflake, __construct)
{
    Variant meta = args[0];
    _this.set("meta", meta);
}

static PHPX_METHOD(Snowflake, generate)
{
    Variant meta = _this.get("meta");
    Object *oMeta = new Object(meta);
    long workerId = oMeta->get("workerId").toInt();
    long dataCenterId = oMeta->get("dataCenterId").toInt();
    long sequence = oMeta->get("sequence").toInt();
    long beginTimestamp = oMeta->get("beginTimestamp").toInt();
    retval = beginTimestamp<<22 | dataCenterId<<17 | workerId << 12 | sequence;
    
}

static PHPX_METHOD(Snowflake, degenerate)
{
    long id = args[0].toInt();
    Variant meta = _this.get("meta");
    Object *oMeta = new Object(meta);
    long beginTimestamp = id >> 22;
    long dataCenterId = id >> 17;
    long workerId = id >> 12;
    oMeta->set("workerId", dataCenterId << 5 ^ workerId);
    oMeta->set("dataCenterId", beginTimestamp << 5 ^ dataCenterId);
    oMeta->set("beginTimestamp", beginTimestamp);
    retval = meta;
    
}

//}}}
PHPX_EXTENSION()
{
    

    Extension *extension = new Extension("snowflake", "0.0.1");

    extension->onStart = [extension]() noexcept {
        extension->registerConstant("SNOWFLAKE_VERSION", 10001);

        Class *meta = new Class("Meta");
        meta->addMethod(PHPX_ME(Meta, __construct), CONSTRUCT);
        meta->addProperty("dataCenterId", 0);
        meta->addProperty("workerId", 0);
        meta->addProperty("sequence", 0);
        meta->addProperty("beginTimestamp", 0);
        extension->registerClass(meta);

        Class *snowflake = new Class("Snowflake");
        snowflake->addMethod(PHPX_ME(Snowflake, __construct), CONSTRUCT);
        snowflake->addMethod(PHPX_ME(Snowflake, generate));
        snowflake->addMethod(PHPX_ME(Snowflake, degenerate));
        extension->registerClass(snowflake);
    };

    //extension->onShutdown = [extension]() noexcept {
    //};

    //extension->onBeforeRequest = [extension]() noexcept {
    //    cout << extension->name << "beforeRequest" << endl;
    //};

    //extension->onAfterRequest = [extension]() noexcept {
    //    cout << extension->name << "afterRequest" << endl;
    //};

    extension->info({"snowflake support", "enabled"},
                    {
                        {"version", extension->version},
                        {"date", "2020-01-08"},
                    });

    return extension;
}