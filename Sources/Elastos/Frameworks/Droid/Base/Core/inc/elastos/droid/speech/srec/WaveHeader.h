#ifndef __ELASTOS_DROID_SPEECH_SREC_WaveHeader_H__
#define __ELASTOS_DROID_SPEECH_SREC_WaveHeader_H__

#include <elastos/io/InputStream.h>
#include <elastos/io/OutputStream.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

/**
 * This class represents the header of a WAVE format audio file, which usually
 * have a .wav suffix.  The following integer valued fields are contained:
 * <ul>
 * <li> format - usually PCM, ALAW or ULAW.
 * <li> numChannels - 1 for mono, 2 for stereo.
 * <li> sampleRate - usually 8000, 11025, 16000, 22050, or 44100 hz.
 * <li> bitsPerSample - usually 16 for PCM, 8 for ALAW, or 8 for ULAW.
 * <li> numBytes - size of audio data after this header, in bytes.
 * </ul>
 *
 * Not yet ready to be supported, so
 * @hide
 */
class WaveHeader
{
public:
    /**
     * Construct a WaveHeader, with all fields defaulting to zero.
     */
    WaveHeader();

    /**
     * Construct a WaveHeader, with fields initialized.
     * @param format format of audio data,
     * one of {@link #FORMAT_PCM}, {@link #FORMAT_ULAW}, or {@link #FORMAT_ALAW}.
     * @param numChannels 1 for mono, 2 for stereo.
     * @param sampleRate typically 8000, 11025, 16000, 22050, or 44100 hz.
     * @param bitsPerSample usually 16 for PCM, 8 for ULAW or 8 for ALAW.
     * @param numBytes size of audio data after this header, in bytes.
     */
    WaveHeader(
        /* [in] */ Int16 format,
        /* [in] */ Int16 numChannels,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int16 bitsPerSample,
        /* [in] */ Int32 numBytes);

    void Init(
        /* [in] */ Int16 format,
        /* [in] */ Int16 numChannels,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int16 bitsPerSample,
        /* [in] */ Int32 numBytes);

    /**
     * Get the format field.
     * @return format field,
     * one of {@link #FORMAT_PCM}, {@link #FORMAT_ULAW}, or {@link #FORMAT_ALAW}.
     */
    CARAPI_(Int16) GetFormat();

    /**
     * Set the format field.
     * @param format
     * one of {@link #FORMAT_PCM}, {@link #FORMAT_ULAW}, or {@link #FORMAT_ALAW}.
     * @return reference to this WaveHeader instance.
     */
    CARAPI SetFormat(
        /* [in] */ Int16 format);

    /**
     * Get the number of channels.
     * @return number of channels, 1 for mono, 2 for stereo.
     */
    CARAPI_(Int16) GetNumChannels();

    /**
     * Set the number of channels.
     * @param numChannels 1 for mono, 2 for stereo.
     * @return reference to this WaveHeader instance.
     */
    CARAPI SetNumChannels(
        /* [in] */ Int16 numChannels);

    /**
     * Get the sample rate.
     * @return sample rate, typically 8000, 11025, 16000, 22050, or 44100 hz.
     */
    CARAPI_(Int32) GetSampleRate();

    /**
     * Set the sample rate.
     * @param sampleRate sample rate, typically 8000, 11025, 16000, 22050, or 44100 hz.
     * @return reference to this WaveHeader instance.
     */
    CARAPI SetSampleRate(
        /* [in] */ Int32 sampleRate);

    /**
     * Get the number of bits per sample.
     * @return number of bits per sample,
     * usually 16 for PCM, 8 for ULAW or 8 for ALAW.
     */
    CARAPI_(Int16) GetBitsPerSample();

    /**
     * Set the number of bits per sample.
     * @param bitsPerSample number of bits per sample,
     * usually 16 for PCM, 8 for ULAW or 8 for ALAW.
     * @return reference to this WaveHeader instance.
     */
    CARAPI SetBitsPerSample(
        /* [in] */ Int16 bitsPerSample);

    /**
     * Get the size of audio data after this header, in bytes.
     * @return size of audio data after this header, in bytes.
     */
    CARAPI_(Int32) GetNumBytes();

    /**
     * Set the size of audio data after this header, in bytes.
     * @param numBytes size of audio data after this header, in bytes.
     * @return reference to this WaveHeader instance.
     */
    CARAPI SetNumBytes(
        /* [in] */ Int32 numBytes);

    /**
     * Read and initialize a WaveHeader.
     * @param in {@link java.io.InputStream} to read from.
     * @return number of bytes consumed.
     * @throws IOException
     */
    CARAPI_(Int32) Read(
        /* [in] */ IInputStream* in);// throws IOException

    /**
     * Write a WAVE file header.
     * @param out {@link java.io.OutputStream} to receive the header.
     * @return number of bytes written.
     * @throws IOException
     */
    CARAPI_(Int32) Write(
        /* [in] */ IOutputStream* out);// throws IOException

    //@Override
    CARAPI_(String) ToString();

private:
    static CARAPI_(void) ReadId(
        /* [in] */ IInputStream* in,
        /* [in] */ const String& id);// throws IOException

    static CARAPI_(Int32) ReadInt(
        /* [in] */ IInputStream* in);// throws IOException

    static CARAPI_(Int16) ReadShort(
        /* [in] */ IInputStream* in);// throws IOException

    static CARAPI_(void) WriteId(
        /* [in] */ IOutputStream* out,
        /* [in] */ const String& id);// throws IOException

    static CARAPI_(void) WriteInt(
        /* [in] */ IOutputStream* out,
        /* [in] */ Int32 val);// throws IOException

    static CARAPI_(void) WriteShort(
        /* [in] */ IOutputStream* out,
        /* [in] */ Int16 val);// throws IOException

private:
    static const CString TAG;// = "WaveHeader";

    static const Int32 HEADER_LENGTH;// = 44;

private:
    Int16 mFormat;// = 0;
    Int16 mNumChannels;// = 0;
    Int32 mSampleRate;// = 0;
    Int16 mBitsPerSample;// = 0;
    Int32 mNumBytes;// = 0;

};

} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_SREC_WaveHeader_H__
