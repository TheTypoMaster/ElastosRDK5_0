
#include "remote/RemoteUtils.h"
#include "remote/ResultMsg.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>
#include <elastos/Slogger.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Remote {

const String RemoteUtils::TAG = String("RemoteUtils");
const Boolean RemoteUtils::DBG = TRUE;
const String RemoteUtils::SPLITER = String("94:GM");

/**
 * composeResult: Add result number in the front of result
 * format: [rltNum][SPLITER][result]
 */
String RemoteUtils::ComposeResult(
    /* [in] */ Int32 rltNum,
    /* [in] */ String result)
{
    StringBuilder sb(StringUtils::Int32ToString(rltNum));
    sb.AppendString(SPLITER);

    /** append result if there is no error */
    if(rltNum == ResultMsg::RESULTMSG_NOERROR->ToInt32()) {
        sb.AppendString(result);
    }

    if (DBG) Slogger::D(TAG, "ComposeResult: %s", sb.ToString().string());
    return sb.ToString();
}

Boolean RemoteUtils::IsDebug()
{
    return TRUE;
}

} // Remote
} // Server
} // Droid
} // Elastos
