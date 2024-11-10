#include "NKMessages.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include <json_spirit/json_spirit_writer.h>

// remove this undef stuff when the time comes
#undef ENFORCE_LINE
#define ENFORCE_LINE(expected)

#define ASSERT_OBJECT_HAS_MEMBER(it, error, line) \
if (it == obj.end()) \
{ \
std::string errorStr = error; \
LOG_ERROR("%s", errorStr.c_str()); ENFORCE_LINE(line);\
throw std::runtime_error(error); \
}

template<> std::string NKJSON::Serialise(const NKMessage& val)
{
    json_spirit::mObject obj;
    Serialise(val, obj);
    return json_spirit::write(obj);
}

void NKJSON::Serialise(const NKMessageAuth& val, json_spirit::mObject& obj)
{
    obj["session"] = val.session;
    obj["device"] = val.device;
    obj["appID"] = val.appID;
    obj["skuID"] = val.skuID;
}

void NKJSON::Serialise(const NKMessage& val, json_spirit::mObject& obj)
{
    json_spirit::mObject authObj;
    Serialise(val.auth, authObj);

    obj["auth"] = authObj;
    obj["sig"] = val.sig;
    obj["data"] = val.data;
    obj["nonce"] = val.nonce;
}

const bool NKJSON::TryParse(NKMessageErrorDetails& out, const json_spirit::mObject& obj)
{
    json_spirit::mObject::const_iterator reasonIt = obj.find("reason");

    if (reasonIt == obj.end())
    {
        std::string error = "object has no member called 'reason'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4807);
        throw std::runtime_error("object has no member called 'reason'");
    }

    out.reason = reasonIt->second.get_str();

    json_spirit::mObject::const_iterator fixIt = obj.find("fix");

    if (fixIt == obj.end())
    {
        std::string error = "object has no member called 'fix'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4818);
        throw std::runtime_error("object has no member called 'fix'");
    }

    out.fix = fixIt->second.get_str();
    return true;
}





const bool NKJSON::TryParse(NKMessageError& out, const json_spirit::mObject& obj)
{
    json_spirit::mObject::const_iterator typeIt = obj.find("type");

    if (typeIt == obj.end())
    {
        std::string error = "object has no member called 'type'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4836);
        throw std::runtime_error("object has no member called 'type'");
    }

    out.type = typeIt->second.get_str();

    json_spirit::mObject::const_iterator detailsIt = obj.find("details");

    if (detailsIt == obj.end())
    {
        std::string error = "object has no member called 'details'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4847);
        throw std::runtime_error("object has no member called 'details'");
    }

    return TryParse(out.details, detailsIt->second.get_obj());
}









const bool NKJSON::TryParse(NKMessageResponse& out, const json_spirit::mObject& obj)
{
    json_spirit::mObject::const_iterator errorIt = obj.find("error");

    if (errorIt == obj.end())
    {
        std::string error = "object has no member called 'error'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4869);
        throw std::runtime_error("object has no member called 'error'");
    }

    if (errorIt->second.type() != json_spirit::Value_type::null_type)
    {
        const json_spirit::mObject& errorObj = errorIt->second.get_obj();
        TryParse(out.error, errorObj);
    }

    if (!out.error.type.empty())
    {
        return true;
    }

    json_spirit::mObject::const_iterator dataIt = obj.find("data");
    json_spirit::mObject::const_iterator sigIt = obj.find("sig");

    ASSERT_OBJECT_HAS_MEMBER(dataIt, "object has no member called 'data'", 4887);
    ASSERT_OBJECT_HAS_MEMBER(sigIt, "object has no member called 'sig'", 4888);

    out.data = dataIt->second.get_str();
    out.sig = sigIt->second.get_str();

    return true;
}
