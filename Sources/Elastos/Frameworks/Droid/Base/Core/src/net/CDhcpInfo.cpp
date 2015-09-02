
#include "net/CDhcpInfo.h"
#if 0 // TODO: Waiting for NetworkUtils
#include "net/NetworkUtils.h"
#endif

namespace Elastos {
namespace Droid {
namespace Net {

CAR_OBJECT_IMPL(CDhcpInfo)

CAR_INTERFACE_IMPL_2(CDhcpInfo, Object, IDhcpInfo, IParcelable)

ECode CDhcpInfo::constructor()
{
//    super();
    return NOERROR;
}

/** copy constructor {@hide} */
ECode CDhcpInfo::constructor(
    /* [in] */ IDhcpInfo* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: change translated codes below
    if (source != NULL)
    {
       source->GetIpAddress(&mIpAddress);
       source->GetGateway(&mGateway);
       source->GetNetmask(&mNetmask);
       source->GetDns1(&mDns1);
       source->GetDns2(&mDns2);
       source->GetServerAddress(&mServerAddress);
       source->GetLeaseDuration(&mLeaseDuration);
    }
    return NOERROR;
#endif
}

ECode CDhcpInfo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: change translated codes below
    VALIDATE_NOT_NULL(result);

    StringBuffer str("ipaddr"); PutAddress(&str, mIpAddress);
    str += String(" gateway "); PutAddress(&str, mGateway);
    str += String(" netmask "); PutAddress(&str, mNetmask);
    str += String(" dns1 "); PutAddress(&str, mDns1);
    str += String(" dns2 "); PutAddress(&str, mDns2);
    str += String(" DHCP server "); PutAddress(&str, mServerAddress);
    str += String(" lease ");
    str += mLeaseDuration;
    str += String(" seconds");

    *result = str.ToString();
    return NOERROR;
#endif
}

void CDhcpInfo::PutAddress(
    /* [in] */ StringBuffer* buff,
    /* [in] */ Int32 addr)
{
#if 0 // TODO: change translated codes below
    buff = new StringBuffer();
    AutoPtr<IInetAddress> inetAddr;
    NetworkUtils::Int32ToInetAddress(addr,(IInetAddress**)&inetAddr);
    String address;
    inetAddr->GetHostAddress(&address);
    buff->AppendString(address);
#endif
}

/** Implement the Parcelable interface {@hide} */
ECode CDhcpInfo::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

/** Implement the Parcelable interface {@hide} */
ECode CDhcpInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mIpAddress);
    dest->WriteInt32(mGateway);
    dest->WriteInt32(mNetmask);
    dest->WriteInt32(mDns1);
    dest->WriteInt32(mDns2);
    dest->WriteInt32(mServerAddress);
    dest->WriteInt32(mLeaseDuration);
    return NOERROR;
}

ECode CDhcpInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadInt32(&mIpAddress);
    source->ReadInt32(&mGateway);
    source->ReadInt32(&mNetmask);
    source->ReadInt32(&mDns1);
    source->ReadInt32(&mDns2);
    source->ReadInt32(&mServerAddress);
    source->ReadInt32(&mLeaseDuration);
    return NOERROR;
}

ECode CDhcpInfo::GetIpAddress(
    /* [out] */ Int32* ipaddress)
{
    VALIDATE_NOT_NULL(ipaddress);
    *ipaddress = mIpAddress;
    return NOERROR;
}

ECode CDhcpInfo::GetGateway(
    /* [out] */ Int32* gateway)
{
    VALIDATE_NOT_NULL(gateway);
    *gateway = mGateway;
    return NOERROR;
}

ECode CDhcpInfo::GetNetmask(
    /* [out] */Int32* netmask)
{
    VALIDATE_NOT_NULL(netmask);
    *netmask = mNetmask;
    return NOERROR;
}

ECode CDhcpInfo::GetDns1(
     /* [out] */ Int32* dns1)
{
    VALIDATE_NOT_NULL(dns1);
    *dns1 = mDns1;
    return NOERROR;
}

ECode CDhcpInfo::GetDns2(
    /* [out] */ Int32* dns2)
{
    VALIDATE_NOT_NULL(dns2);
    *dns2 = mDns2;
    return NOERROR;
}

ECode CDhcpInfo::GetServerAddress(
    /* [out] */ Int32* serverAddress)
{
    VALIDATE_NOT_NULL(serverAddress);
    *serverAddress = mServerAddress;
    return NOERROR;
}

ECode CDhcpInfo::GetLeaseDuration(
    /* [out] */ Int32* leaseDuration)
{
    VALIDATE_NOT_NULL(leaseDuration);
    *leaseDuration = mLeaseDuration;
    return NOERROR;
}

ECode CDhcpInfo::SetIpAddress(
    /* [in] */ Int32 ipaddress)
{
    mIpAddress = ipaddress;
    return NOERROR;
}

ECode CDhcpInfo::SetGateway(
    /* [in] */ Int32 gateway)
{
    mGateway = gateway;
    return NOERROR;
}

ECode CDhcpInfo::SetNetmask(
    /* [in] */ Int32 netmask)
{
    mNetmask = netmask;
    return NOERROR;
}

ECode CDhcpInfo::SetDns1(
    /* [in] */ Int32 dns1)
{
    mDns1 = dns1;
    return NOERROR;
}

ECode CDhcpInfo::SetDns2(
    /* [in] */ Int32 dns2)
{
    mDns2 = dns2;
    return NOERROR;
}

ECode CDhcpInfo::SetServerAddress(
    /* [in] */ Int32 serverAddress)
{
    mServerAddress = serverAddress;
    return NOERROR;
}

ECode CDhcpInfo::SetLeaseDuration(
    /* [in] */ Int32 leaseDuration)
{
    mLeaseDuration = leaseDuration;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
