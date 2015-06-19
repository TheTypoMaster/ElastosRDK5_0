
#ifndef __CREMOTEPARCEL_H__
#define __CREMOTEPARCEL_H__

#include <marshal_rpc.h>

namespace Elastos {
namespace RPC {

class CRemoteParcel
    : public ElRefBase
    , public IParcel
{
public:
    CRemoteParcel(
        /* [in] */ Boolean writeMarshalHeader = TRUE);

    ~CRemoteParcel();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Marshall(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI Unmarshall(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI AppendFrom(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* result);

    CARAPI ReadByte(
        /* [out] */ Byte *pValue);

    CARAPI WriteByte(
        /* [in] */ Byte value);

    CARAPI ReadBoolean(
        /* [out] */ Boolean *pValue);

    CARAPI WriteBoolean(
        /* [in] */ Boolean value);

    CARAPI ReadChar16(
        /* [out] */ Char16 *pValue);

    CARAPI WriteChar16(
        /* [in] */ Char16 value);

    CARAPI ReadChar(
        /* [out] */ Char32 *pValue);

    CARAPI WriteChar(
        /* [in] */ Char32 value);

    CARAPI ReadInt16(
        /* [out] */ Int16 *pValue);

    CARAPI WriteInt16(
        /* [in] */ Int16 value);

    CARAPI ReadInt32(
        /* [out] */ Int32 *pValue);

    CARAPI WriteInt32(
        /* [in] */ Int32 value);

    CARAPI ReadInt64(
        /* [out] */ Int64 *pValue);

    CARAPI WriteInt64(
        /* [in] */ Int64 value);

    CARAPI ReadFloat(
        /* [out] */ Float *value);

    CARAPI WriteFloat(
        /* [in] */ Float value);

    CARAPI ReadDouble(
        /* [out] */ Double *value);

    CARAPI WriteDouble(
        /* [in] */ Double value);

    CARAPI ReadString(
        /* [out] */ String* pStr);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI ReadStruct(
        /* [out] */ Handle32 *pAddress);

    CARAPI WriteStruct(
        /* [in] */ Handle32 pValue,
        /* [in] */ Int32 size);

    CARAPI ReadEMuid(
        /* [out] */ EMuid *pId);

    CARAPI WriteEMuid(
        /* [in] */ const EMuid & id);

    CARAPI ReadEGuid(
        /* [out] */ EGuid *pId);

    CARAPI WriteEGuid(
        /* [in] */ const EGuid & id);

    CARAPI ReadArrayOf(
        /* [out] */ Handle32 *ppArray);

    CARAPI WriteArrayOf(
        /* [in] */ Handle32 pArray);

    CARAPI ReadArrayOfString(
        /* [out, callee] */ ArrayOf<String>** ppArray);

    CARAPI WriteArrayOfString(
        /* [in] */ ArrayOf<String>* array);

    CARAPI ReadInterfacePtr(
        /* [out] */ Handle32 *pItfPtr);

    CARAPI WriteInterfacePtr(
        /* [in] */ IInterface* pValue);

    // Place a file descriptor into the parcel.  The given fd must remain
    // valid for the lifetime of the parcel.
    CARAPI WriteFileDescriptor(
        /* [in] */ Int32 fd);

    // Place a file descriptor into the parcel.  A dup of the fd is made, which
    // will be closed once the parcel is destroyed.
    CARAPI WriteDupFileDescriptor(
        /* [in] */ Int32 fd);

    // Retrieve a file descriptor from the parcel.  This returns the raw fd
    // in the parcel, which you do not own -- use dup() to get your own copy.
    CARAPI ReadFileDescriptor(
        /* [out] */ Int32* fd);

    CARAPI Clone(
        /* [in] */ IParcel* pSrcParcel);

    CARAPI GetDataPosition(
        /* [out] */ Int32* position);

    CARAPI SetDataPosition(
        /* [in] */ Int32 position);

    CARAPI GetElementSize(
        /* [out] */ Int32* pSize);

    CARAPI GetElementPayload(
        /* [ouy] */ Handle32* pBuffer);

    CARAPI_(MarshalHeader*) GetMarshalHeader();

    CARAPI_(const UInt8*) Data() const;

    CARAPI_(UInt32) DataSize() const;

    CARAPI_(UInt32) DataAvail() const;

    CARAPI_(UInt32) DataPosition() const;

    CARAPI_(UInt32) DataCapacity() const;

    CARAPI SetDataSize(
        /* [in] */ UInt32 size);

    CARAPI_(void) SetDataPositionInner(
        /* [in] */ UInt32 pos) const;

    CARAPI SetDataCapacity(
        /* [in] */ UInt32 size);

    CARAPI SetData(
        /* [in] */ const UInt8* buffer,
        /* [in] */ UInt32 len);

    CARAPI_(void) FreeData();

    CARAPI_(UInt32) ObjectsCount() const;

    CARAPI Write(
        /* [in] */ const void* data,
        /* [in] */ UInt32 len);

    CARAPI_(void*) WriteInplace(
        /* [in] */ UInt32 len);

    CARAPI Read(
        /* [in] */ void* outData,
        /* [in] */ UInt32 len) const;

    CARAPI_(const void*) ReadInplace(
        /* [in] */ UInt32 len) const;

private:
    CARAPI FinishWrite(
        /* [in] */ UInt32 len);

    CARAPI_(void) ReleaseObjects();

    CARAPI GrowData(
        /* [in] */ UInt32 len);

    CARAPI RestartWrite(
        /* [in] */ UInt32 desired);

    CARAPI ContinueWrite(
        /* [in] */ UInt32 desired);

    CARAPI_(void) FreeDataNoInit();

    CARAPI_(void) InitState();

    template<class T>
    CARAPI ReadAligned(
        /* [in] */ T* value) const;

    template<class T> T ReadAligned() const;

    template<class T>
    CARAPI WriteAligned(
        /* [in] */ T value);

    CARAPI WriteCStringInner(
        /* [in] */ const char* str);

    CARAPI_(const char*) ReadCStringInner() const;

    CARAPI_(Boolean) IsParcelable(
        /* [in] */ InterfacePack *pInterfacePack,
        /* [out] */ CIClassInfo **ppClassInfo);

    CARAPI ReadArrayOfInner(
        /* [out] */ PCARQUINTET* pValue);

    CARAPI WriteArrayOfInner(
        /* [in] */ PCARQUINTET pValue);

public:
    typedef void (*release_func)(
        /* [in] */ CRemoteParcel* parcel,
        /* [in] */ const UInt8* data,
        /* [in] */ UInt32 dataSize,
        /* [in] */ const UInt32* objects,
        /* [in] */ UInt32 objectsSize,
        /* [in] */ void* cookie);

private:
    ECode mError;
    UInt8* mData;
    UInt32 mDataSize;
    UInt32 mDataCapacity;
    mutable UInt32 mDataPos;
    UInt32* mObjects;
    UInt32 mObjectsSize;
    UInt32 mObjectsCapacity;
    mutable UInt32 mNextObjectHint;

    release_func mOwner;
    void* mOwnerCookie;
};

} // namespace RPC
} // namespace Elastos

#endif //__CREMOTEPARCEL_H__
