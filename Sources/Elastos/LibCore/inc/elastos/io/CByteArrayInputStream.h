
#ifndef __CBYTEARRAYINPUTSTREAM_H__
#define __CBYTEARRAYINPUTSTREAM_H__

#include "_Elastos_IO_CByteArrayInputStream.h"
#include "ByteArrayInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CByteArrayInputStream)
    , public ByteArrayInputStream
{
public:
    CAR_OBJECT_DECL()

};

} // namespace IO
} // namespace Elastos

#endif // __CBYTEARRAYINPUTSTREAM_H__
