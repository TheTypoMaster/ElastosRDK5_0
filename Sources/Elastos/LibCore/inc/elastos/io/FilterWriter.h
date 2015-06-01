
#ifndef __ELASTOS_IO_FILTERWRITER_H__
#define __ELASTOS_IO_FILTERWRITER_H__

#include "elastos/io/Writer.h"

namespace Elastos {
namespace IO {

class FilterWriter
    : public Writer
{
protected:
    FilterWriter();

    ~FilterWriter();

    /**
     * Constructs a new FilterWriter on the Writer {@code out}. All writes are
     * now filtered through this writer.
     *
     * @param out
     *            the target Writer to filter writes on.
     */
    CARAPI Init(
        /* [in] */ IWriter* out);

    /**
     * Closes this writer. This implementation closes the target writer.
     *
     * @throws IOException
     *             if an error occurs attempting to close this writer.
     */
    CARAPI Close();

    /**
     * Flushes this writer to ensure all pending data is sent out to the target
     * writer. This implementation flushes the target writer.
     *
     * @throws IOException
     *             if an error occurs attempting to flush this writer.
     */
    CARAPI Flush();

    /**
     * Writes {@code count} characters from the char array {@code buffer}
     * starting at position {@code offset} to the target writer.
     *
     * @param buffer
     *            the buffer to write.
     * @param offset
     *            the index of the first character in {@code buffer} to write.
     * @param count
     *            the number of characters in {@code buffer} to write.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    /**
     * Writes the specified character {@code oneChar} to the target writer. Only the
     * two least significant bytes of the integer {@code oneChar} are written.
     *
     * @param oneChar
     *            the char to write to the target writer.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    /**
     * Writes {@code count} characters from the string {@code str} starting at
     * position {@code index} to this writer. This implementation writes
     * {@code str} to the target writer.
     *
     * @param str
     *            the string to be written.
     * @param offset
     *            the index of the first character in {@code str} to write.
     * @param count
     *            the number of chars in {@code str} to write.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI WriteString(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

protected:
     /**
     * The Writer being filtered.
     */
     AutoPtr<IWriter> mOut;
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_FILTERWRITER_H__
