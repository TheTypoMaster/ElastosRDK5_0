
#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include "_Elastos_Sql_SQLite_CDatabase.h"
#include "Database.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDatabase) , public Database
{
public:
    CAR_OBJECT_DECL();
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDATABASE_H__
