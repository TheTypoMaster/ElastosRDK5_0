
#ifndef __CHTTPDATEHELPER_H__
#define __CHTTPDATEHELPER_H__

#include "_CHttpDate.h"
#include "Elastos.CoreLibrary_server.h"

using Elastos::Utility::IDate;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CHttpDate)
{
public:
    CARAPI Parse(
        /* [in] */ const String& value,
        /* [out] */ IDate** adate);

    CARAPI Format(
        /* [in] */ IDate* value,
        /* [out] */ String* str);

public:
    static CARAPI _Parse(
        /* [in] */ const String& value,
        /* [out] */ IDate** adate);

    static CARAPI _Format(
        /* [in] */ IDate* value,
        /* [out] */ String* str);

private:
    /**
     * Most websites serve cookies in the blessed format. Eagerly create the parser to ensure such
     * cookies are on the fast path.
     */
    // private static final ThreadLocal<DateFormat> STANDARD_DATE_FORMAT
    //         = new ThreadLocal<DateFormat>() {
    //     @Override protected DateFormat initialValue() {
    //         DateFormat rfc1123 = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
    //         rfc1123.setTimeZone(TimeZone.getTimeZone("UTC"));
    //         return rfc1123;
    //     }
    // };

    /**
     * If we fail to parse a date in a non-standard format, try each of these formats in sequence.
     */
    static const String BROWSER_COMPATIBLE_DATE_FORMATS[14];
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CHTTPDATEHELPER_H__
