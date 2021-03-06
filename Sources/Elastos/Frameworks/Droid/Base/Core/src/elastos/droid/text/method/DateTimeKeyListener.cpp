#include "text/method/DateTimeKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Char32 DateTimeKeyListener::CHARACTERS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'm', 'p', ':', '/', '-', ' ' };

Int32 DateTimeKeyListener::GetInputType()
{
    return IInputType::TYPE_CLASS_DATETIME | IInputType::TYPE_DATETIME_VARIATION_NORMAL;
}

AutoPtr< ArrayOf<Char32> > DateTimeKeyListener::GetAcceptedChars()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(17);
    for(Int32 i=0; i<17; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

AutoPtr< ArrayOf<Char32> > DateTimeKeyListener::GetCHARACTERS()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(17);
    for(Int32 i=0; i<17; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos