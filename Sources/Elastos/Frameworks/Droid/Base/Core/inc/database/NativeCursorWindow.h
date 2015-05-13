
#ifndef __NATIVECURSORWINDOW_H__
#define __NATIVECURSORWINDOW_H__

#include <elastos.h>
#include <cutils/log.h>
#include <utils/Errors.h>
#include <stddef.h>
#include <stdint.h>

#if LOG_NDEBUG

#define IF_LOG_WINDOW() if (false)
#define LOG_WINDOW(...)

#else

#define IF_LOG_WINDOW() IF_ALOG(LOG_DEBUG, "NativeCursorWindow")
#define LOG_WINDOW(...) ALOG(LOG_DEBUG, "NativeCursorWindow", __VA_ARGS__)

#endif

namespace Elastos {
namespace Droid {
namespace Database {

/**
 * This class stores a set of rows from a database in a buffer. The begining of the
 * window has first chunk of RowSlots, which are offsets to the row directory, followed by
 * an offset to the next chunk in a linked-list of additional chunk of RowSlots in case
 * the pre-allocated chunk isn't big enough to refer to all rows. Each row directory has a
 * FieldSlot per column, which has the size, offset, and type of the data for that field.
 * Note that the data types come from sqlite3.h.
 *
 * Strings are stored in UTF-8.
 */
class NativeCursorWindow
{
    NativeCursorWindow(const String& name, int ashmemFd,
            void* data, size_t size, bool readOnly);

public:
    /* Field types. */
    enum {
        FIELD_TYPE_NULL = 0,
        FIELD_TYPE_INTEGER = 1,
        FIELD_TYPE_FLOAT = 2,
        FIELD_TYPE_STRING = 3,
        FIELD_TYPE_BLOB = 4,
    };

    /* Opaque type that describes a field slot. */
    struct FieldSlot {
    private:
        int32_t type;
        union {
            double d;
            int64_t l;
            struct {
                uint32_t offset;
                uint32_t size;
            } buffer;
        } data;

        friend class NativeCursorWindow;
    } __attribute((packed));

    ~NativeCursorWindow();

    static android::status_t create(const String& name, size_t size, NativeCursorWindow** outCursorWindow);
    static android::status_t createFromParcel(IParcel* IParcel, NativeCursorWindow** outCursorWindow);

    android::status_t writeToParcel(IParcel* IParcel);

    inline String name() { return mName; }
    inline size_t size() { return mSize; }
    inline size_t freeSpace() { return mSize - mHeader->freeOffset; }
    inline uint32_t getNumRows() { return mHeader->numRows; }
    inline uint32_t getNumColumns() { return mHeader->numColumns; }

    android::status_t clear();
    android::status_t setNumColumns(uint32_t numColumns);

    /**
     * Allocate a row slot and its directory.
     * The row is initialized will null entries for each field.
     */
    android::status_t allocRow();
    android::status_t freeLastRow();

    android::status_t putBlob(uint32_t row, uint32_t column, const void* value, size_t size);
    android::status_t putString(uint32_t row, uint32_t column, const char* value, size_t sizeIncludingNull);
    android::status_t putLong(uint32_t row, uint32_t column, int64_t value);
    android::status_t putDouble(uint32_t row, uint32_t column, double value);
    android::status_t putNull(uint32_t row, uint32_t column);

    /**
     * Gets the field slot at the specified row and column.
     * Returns null if the requested row or column is not in the window.
     */
    FieldSlot* getFieldSlot(uint32_t row, uint32_t column);

    inline int32_t getFieldSlotType(FieldSlot* fieldSlot) {
        return fieldSlot->type;
    }

    inline int64_t getFieldSlotValueLong(FieldSlot* fieldSlot) {
        return fieldSlot->data.l;
    }

    inline double getFieldSlotValueDouble(FieldSlot* fieldSlot) {
        return fieldSlot->data.d;
    }

    inline const char* getFieldSlotValueString(FieldSlot* fieldSlot,
            size_t* outSizeIncludingNull) {
        *outSizeIncludingNull = fieldSlot->data.buffer.size;
        return static_cast<char*>(offsetToPtr(fieldSlot->data.buffer.offset));
    }

    inline const void* getFieldSlotValueBlob(FieldSlot* fieldSlot, size_t* outSize) {
        *outSize = fieldSlot->data.buffer.size;
        return offsetToPtr(fieldSlot->data.buffer.offset);
    }

private:
    static const size_t ROW_SLOT_CHUNK_NUM_ROWS = 100;

    struct Header {
        // Offset of the lowest unused byte in the window.
        uint32_t freeOffset;

        // Offset of the first row slot chunk.
        uint32_t firstChunkOffset;

        uint32_t numRows;
        uint32_t numColumns;
    };

    struct RowSlot {
        uint32_t offset;
    };

    struct RowSlotChunk {
        RowSlot slots[ROW_SLOT_CHUNK_NUM_ROWS];
        uint32_t nextChunkOffset;
    };

    String mName;
    int mAshmemFd;
    void* mData;
    size_t mSize;
    bool mReadOnly;
    Header* mHeader;

    inline void* offsetToPtr(uint32_t offset) {
        return static_cast<uint8_t*>(mData) + offset;
    }

    inline uint32_t offsetFromPtr(void* ptr) {
        return static_cast<uint8_t*>(ptr) - static_cast<uint8_t*>(mData);
    }

    /**
     * Allocate a portion of the window. Returns the offset
     * of the allocation, or 0 if there isn't enough space.
     * If aligned is true, the allocation gets 4 byte alignment.
     */
    uint32_t alloc(size_t size, bool aligned = false);

    RowSlot* getRowSlot(uint32_t row);
    RowSlot* allocRowSlot();

    android::status_t putBlobOrString(uint32_t row, uint32_t column,
            const void* value, size_t size, int32_t type);
};

} //Database
} //Droid
} //Elastos

#endif //__NATIVECURSORWINDOW_H__
