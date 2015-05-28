
#ifndef __CBYTEARRAYOUTPUTSTREAM_H__
#define __CBYTEARRAYOUTPUTSTREAM_H__

#include "_Elastos_IO_CByteArrayOutputStream.h"
#include "ByteArrayOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CByteArrayOutputStream)
    , public ByteArrayOutputStream
{
public:
    CAR_OBJECT_DECL()

};

} // namespace IO
} // namespace Elastos

#endif // __CBYTEARRAYOUTPUTSTREAM_H__
