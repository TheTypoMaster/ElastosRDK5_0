
#ifndef __LIBCORE_ICU_ALPHABETICINDEX_H__
#define __LIBCORE_ICU_ALPHABETICINDEX_H__

#include "Object.h"

using Elastos::Core::Object;
using Elastos::Utility::ILocale;

namespace Libcore {
namespace ICU {

class AlphabeticIndex
    : public Object
    , public IAlphabeticIndex
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ ILocale* locale);

protected:
    ~AlphabeticIndex();

public:
    CARAPI GetMaxLabelCount(
        /* [out] */ Int32* count);

    CARAPI SetMaxLabelCount(
        /* [in] */ Int32 count);

    CARAPI AddLabels(
        /* [in] */ ILocale* locale);

    CARAPI AddLabelRange(
        /* [in] */ Int32 codePointStart,
        /* [in] */ Int32 codePointEnd);

    CARAPI GetBucketCount(
        /* [out] */ Int32* count);

    CARAPI GetBucketIndex(
        /* [in] */ const String& s,
        /* [out] */ Int32* index);

    CARAPI GetBucketLabel(
        /* [in] */ Int32 index,
        /* [out] */ String* label);

    CARAPI GetImmutableIndex(
        /* [out] */ IImmutableIndex** index);

private:
    static CARAPI_(Int64) Create(const String& locale);

    static CARAPI_(void) Destroy(Int64 peer);

    static CARAPI_(Int32) GetMaxLabelCount(Int64 peer);

    static CARAPI_(void) SetMaxLabelCount(Int64 peer, Int32 count);

    static CARAPI_(void) AddLabels(Int64 peer, const String& locale);

    static CARAPI_(void) AddLabelRange(Int64 peer, Int32 codePointStart, Int32 codePointEnd);

    static CARAPI_(Int32) GetBucketCount(Int64 peer);

    static CARAPI_(Int32) GetBucketIndex(Int64 peer, const String& s);

    static CARAPI_(String) GetBucketLabel(Int64 peer, Int32 index);

    static CARAPI_(Int64) BuildImmutableIndex(Int64 peer);

    Int64 mPeer;

friend class ImmutableIndex;

};

} // namespace ICU
} // namespace Libcore

#endif // __LIBCORE_ICU_ALPHABETICINDEX_H__