#include <cmdef.h>
#include <stdio.h>
#include <stdlib.h>
#include <elastos.h>
#include <elautoptr.h>
#include <eltypes.h>
#include <stdio.h>
#include <Elastos.Core.h>
#include <elastos/StringBuilder.h>
#include <elastos/HashMap.h>
#include <elastos/StringUtils.h>

using Elastos::Utility::HashMap;

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Math;

using Elastos::Core::CObjectContainer;


class Base {
public:
    Base(Int32 foo = 0):mFoo(foo)
    {
        printf(" >>> Create: Base %p", this);
    }
    virtual ~Base() {}

    virtual void Print()
    {
        printf(" > %p: foo %d\n", this, mFoo);
    }
public:
    Int32 mFoo;
};

class Drived : public Base, public ElRefBase
{
public:
    Drived(Int32 foo = 0, Int32 bar = 1)
    :Base(foo), mBar(bar)
    {
        printf(", Drived %p: foo %d, bar %d\n", this, mFoo, mBar);
    }

    virtual ~Drived()
    {
        printf(" >>> Delete Drived %p...\n", this);
    }

    virtual void Print()
    {
        printf(" > %p: foo %d, bar %d\n", this, mFoo, mBar);
    }

public:
    Int32 mBar;
};

//=============================================================================

void doTestArrayOfFreeRelease()
{
    printf("\n==== enter doTestArrayOfFreeRelease ====\n");

    // Notes
    // Alloc: Create a new object with ref count zero.
    //
    {
        printf(" == Alloc\\Free ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);    // new obj with rc=0
        ArrayOf<Int32>::Free(v1);                           // free
    }

    {
        printf(" == Alloc\\Release ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);    // new obj with rc=0
        v1->Release();                                      // free
    }

    // Notes
    // AutoPtr<T>: increment ref count by one.
    //
    {
        printf(" == Alloc\\AutoPtr ==\n");
        AutoPtr<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10); // rc=1
        // v1 AutoPtr -> rc=0 -> free
    }

    // Notes
    // Clone: Clone a new object with ref count zero.
    //
    {
        printf(" == Alloc\\Clone\\Free ==\n");
        AutoPtr<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10);    // v1 rc=1
        ArrayOf<Int32>* v2 = v1->Clone();       // v2 new obj wih rc=0
        ArrayOf<Int32>::Free(v2);               // v2 free
        // v1 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Clone\\Release ==\n");
        AutoPtr<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10);    // v1 rc=1
        ArrayOf<Int32>* v2 = v1->Clone();   // v2 new obj wih rc=0
        v2->Release();                      // v2 free
        // v1 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Clone\\AutoPtr ==\n");
        AutoPtr<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10);    // v1 rc=1
        AutoPtr<ArrayOf<Int32> > v2 = v1->Clone();                  // v2 rc=1
        // v1 AutoPtr -> rc=0 -> free
        // v2 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Free ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);    // new obj with rc=0
        ArrayOf<Int32>::Free(v1);                           // free

        //v1->Release();                                    // crash!!!
    }

    printf("==== return from doTestArrayOfFreeRelease ====\n");
}

void doTestBufferOfFreeRelease()
{
    printf("\n==== enter doTestBufferOfFreeRelease ====\n");

    // Notes
    // Alloc: Create a new object with ref count zero.
    //
    {
        printf(" == Alloc\\Free ==\n");
        BufferOf<Int32> * v1 = BufferOf<Int32>::Alloc(10);  // new obj with rc=0
        BufferOf<Int32>::Free(v1);                          // free
    }

    {
        printf(" == Alloc\\Release ==\n");
        BufferOf<Int32> * v1 = BufferOf<Int32>::Alloc(10);  // new obj with rc=0
        v1->Release();                                      // free
    }

    // Notes
    // AutoPtr<T>: increment ref count by one.
    //
    {
        printf(" == Alloc\\AutoPtr ==\n");
        AutoPtr<BufferOf<Int32> > v1 = BufferOf<Int32>::Alloc(10); // rc=1
        // v1 AutoPtr -> rc=0 -> free
    }

    // Notes
    // Clone: Clone a new object with ref count zero.
    //
    {
        printf(" == Alloc\\Clone\\Free ==\n");
        AutoPtr<BufferOf<Int32> > v1 = BufferOf<Int32>::Alloc(10);  // v1 rc=1
        BufferOf<Int32>* v2 = v1->Clone();       // v2 new obj wih rc=0
        BufferOf<Int32>::Free(v2);               // v2 free
        // v1 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Clone\\Release ==\n");
        AutoPtr<BufferOf<Int32> > v1 = BufferOf<Int32>::Alloc(10);  // v1 rc=1
        BufferOf<Int32>* v2 = v1->Clone();  // v2 new obj wih rc=0
        v2->Release();                      // v2 free
        // v1 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Clone\\AutoPtr ==\n");
        AutoPtr<BufferOf<Int32> > v1 = BufferOf<Int32>::Alloc(10);  // v1 rc=1
        AutoPtr<BufferOf<Int32> > v2 = v1->Clone();                 // v2 rc=1
        // v1 AutoPtr -> rc=0 -> free
        // v2 AutoPtr -> rc=0 -> free
    }

    {
        printf(" == Alloc\\Free ==\n");
        BufferOf<Int32> * v1 = BufferOf<Int32>::Alloc(10);  // new obj with rc=0
        BufferOf<Int32>::Free(v1);                          // free

        //v1->Release();                                    // crash!!!
    }

    printf("==== return from doTestBufferOfFreeRelease ====\n");
}

void doTestAssignOnStack()
{
    printf("\n==== enter doTestAssignOnStack ====\n");

    {
        ArrayOf_<String, 10> v1;
        v1[0] = String("Stack string 1");
        v1[1] = String("Stack string 2");
        v1[2] = String("Stack string 3");
        v1[3] = v1[2];
    }
    printf("==== return from doTestAssignOnStack ====\n");
}

void doTestSelfAssign()
{
    printf("\n==== enter doTestSelfAssign ====\n");
    {
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);
        AutoPtr<ArrayOf<Int32> > ptr1 = v1;
        AutoPtr<ArrayOf<Int32> > ptr2 = ptr1;
        ptr2 = v1;
    }

    printf("==== return from doTestSelfAssign ====\n");
}

//=============================================================================

void printArray(ArrayOf<Int32> * v, const char* info)
{
    printf("  >------Start print %s ------<\n", info);
    Int32 len = v->GetLength();
    for (Int32 i = 0; i < len; ++i) {
        printf("    > %d: %d\n", i, (*v)[i]);
    }
    printf("  >------End print %s ------<\n", info);
}

void testBasicType()
{
    printf("\n==== enter testBasicType ====\n");

    {
        printf(" == Alloc\\Free\\Set ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(4);
        (*v1)[0] = 1111;
        (*v1)[1] = 2222;
        v1->Set(0, 1111111);
        v1->Set(2, 3333333);

        printArray(v1, "Alloc\\Free\\Set result");
        ArrayOf<Int32>::Free(v1);
    }

    {
        printf("\n == AutoPtr ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(4);
        (*v1)[0] = 1111;
        (*v1)[1] = 2222;
        AutoPtr<ArrayOf<Int32> > ptr1 = v1;
    }

    {
        printf("\n == Copy ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(7);
        (*v1)[0] = 0000;
        (*v1)[1] = 1111;
        (*v1)[2] = 2222;
        (*v1)[3] = 3333;
        (*v1)[4] = 4444;
        (*v1)[5] = 5555;
        (*v1)[6] = 6666;

        printArray(v1, "Copy source");

        ArrayOf<Int32> * v2 = ArrayOf<Int32>::Alloc(4);
        v2->Copy(v1);
        printArray(v2, "Copy result");

        v2->Copy(v1, 1, 7);
        printArray(v2, "Copy 02 result");

        v2->Copy(2, v1, 4, 2);
        printArray(v2, "Copy 03 result");

        {
            AutoPtr<ArrayOf<Int32> > ptr1 = v1;
            AutoPtr<ArrayOf<Int32> > ptr2 = v2;
        }
    }

    {
        printf("\n == CopyEx ==\n");
        Int32 array[5] = { 1111, 2222, 3333, 4444, 5555};
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(4);
        (*v1)[0] = 100001;
        v1->Copy(array, 2);
        printArray(v1, "CopyEx 01 result");
        AutoPtr<ArrayOf<Int32> > ptr1 = v1;
    }

    {
        printf("\n == Copy Buffer==\n");
        Int32 array[2] = { 1111, 2222};
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(4);
        (*v1)[0] = 201111;
        (*v1)[1] = 202222;
        (*v1)[2] = 203333;

        v1->Copy(2, array, 2);

        printArray(v1, "Copy result");
        AutoPtr<ArrayOf<Int32> > ptr1 = v1;
    }

    {
        printf("\n == Clone ==\n");
        ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(4);
        (*v1)[0] = 201111;
        (*v1)[1] = 202222;
        (*v1)[2] = 203333;

        ArrayOf<Int32> * v2 = v1->Clone();
        {
            AutoPtr<ArrayOf<Int32> > ptr1 = v1;
        }

        printArray(v2, "Clone result");
        AutoPtr<ArrayOf<Int32> > ptr2 = v2;
    }

    printf("==== return from testBasicType ====\n");
}

//=============================================================================

void printArray(ArrayOf<String> * v, const char* info)
{
    printf("  >------Start print %s ------<\n", info);
    Int32 len = v->GetLength();
    for (Int32 i = 0; i < len; ++i) {
        if ((*v)[i])
            printf("    > %d: %s\n", i, (const char *)((*v)[i]));
        else
            printf("    > %d: NULL\n", i);
    }
    printf("  >------End print %s ------<\n", info);
}

void testString()
{
    printf("\n==== enter testString ====\n");

    {
        printf(" == Alloc\\Free\\Set ==\n");
        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = String("Hello World 1!");
        (*v1)[1] = String("Hello World 2!");
        v1->Set(0, String("String 1 by set."));
        v1->Set(2, String("String 3 by set."));

        printArray(v1, "Alloc\\Free\\Set result");
        ArrayOf<String>::Free(v1);
    }

    {
        printf("\n == AutoPtr ==\n");
        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = String("String 1");
        (*v1)[1] = String("String 2");

        AutoPtr<ArrayOf<String> > ptr1 = v1;
    }

    {
        printf("\n == Copy ==\n");
        String sameString("<Same string>");
        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = sameString;
        (*v1)[1] = String("Copy 2");

        ArrayOf<String> * v2 = ArrayOf<String>::Alloc(4);
        (*v2)[0] = sameString;
        v2->Copy(v1);

        {
            AutoPtr<ArrayOf<String> > ptr1 = v1;
        }

        printArray(v2, " Copy result");
        AutoPtr<ArrayOf<String> > ptr2 = v2;
    }

    {
        printf("\n == CopyEx ==\n");
        String strArray[2];
        strArray[0] = String("CopyEx 1");
        strArray[1] = String("CopyEx 2");

        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = String("String 3");
        v1->Copy(strArray, 2);

        printArray(v1, " CopyEx result");
        AutoPtr<ArrayOf<String> > ptr1 = v1;
    }

    {
        printf("\n == Copy Buffer==\n");
        String strArray[2];
        strArray[0] = String("Copy 1");
        strArray[1] = String("Copy 2");

        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = String("Original 1");
        (*v1)[1] = String("Original 2");
        (*v1)[2] = String("Original 3");
        v1->Copy(2, strArray, 2);

        printArray(v1, " Copy result");
        AutoPtr<ArrayOf<String> > ptr1 = v1;
    }

    {
        printf("\n == Clone ==\n");
        ArrayOf<String> * v1 = ArrayOf<String>::Alloc(4);
        (*v1)[0] = String("Clone 1");
        (*v1)[1] = String("Clone 2");
        (*v1)[2] = String("Clone 3");

        ArrayOf<String> * v2 = v1->Clone();
        {
            AutoPtr<ArrayOf<String> > ptr1 = v1;
        }

        printArray(v2, " Clone result");
        AutoPtr<ArrayOf<String> > ptr2 = v2;
    }

    printf("==== return from testString ====\n");
}

//=============================================================================
void printArray(ArrayOf<IBigInteger *> * v, const char* info)
{
    printf("  >------Start print %s ------<\n", info);
    Int32 len = v->GetLength();
    for (Int32 i = 0; i < len; ++i) {
        if ((*v)[i]) {
            IBigInteger* bi = (*v)[i];
            Int32 sign = 0;
            bi->GetSignum(&sign);
            printf("    > %d: %p, sign %d\n", i, bi, sign);
        }
        else {
            printf("    > %d: NULL\n", i);
        }
    }
    printf("  >------End print %s ------<\n", info);
}

void printArray(ArrayOf<IInterface *> * v, const char* info)
{
    printf("  >------Start print %s ------<\n", info);
    Int32 len = v->GetLength();
    for (Int32 i = 0; i < len; ++i) {
        if ((*v)[i]) {
            printf("    > %d: %p\n", i, (*v)[i]);
        }
        else {
            printf("    > %d: NULL\n", i);
        }
    }
    printf("  >------End print %s ------<\n", info);
}

void testIInterface()
{
    printf("\n==== enter testIInterface ====\n");

    {
        printf(" == Alloc\\Free\\Set ==\n");
        IBigInteger* i1;
        ECode result = CBigInteger::New(1, 1, (IBigInteger**)&i1);
        if (NOERROR != result || i1 == NULL) {
            printf(" Failed to create CBigInteger. Error %08X\n", result);
        }

        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        v1->Set(0, i1);

        printArray(v1, " Set v1");

        Int32 sign = 0;
        i1->GetSignum(&sign);
        printf("    sign of v1: %d\n", sign);
        ArrayOf<IBigInteger*>::Free(v1);
    }

    {
        printf("\n == AutoPtr ==\n");
        AutoPtr<IBigInteger> i1;
        CBigInteger::New(-1, 1, (IBigInteger**)&i1);
        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        (*v1)[0] = i1;
        (*v1)[0]->AddRef();

        printArray(v1, " AutoPtr v1");
        AutoPtr<ArrayOf<IBigInteger*> > ptr1 = v1;
    }

    {
        printf("\n == Copy ==\n");
        AutoPtr<IBigInteger> i1, i2, i3, i4;
        CBigInteger::New(1, 1, (IBigInteger**)&i1);
        CBigInteger::New(1, 2, (IBigInteger**)&i2);
        CBigInteger::New(-1, 3, (IBigInteger**)&i3);
        CBigInteger::New(-1, 4, (IBigInteger**)&i4);

        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        v1->Set(0, i1);
        v1->Set(1, i2);

        printArray(v1, " Copy source");

        ArrayOf<IBigInteger*> * v2 = ArrayOf<IBigInteger*>::Alloc(4);
        v2->Set(0, i3);
        v2->Set(2, i4);
        v2->Copy(v1);

        {
            AutoPtr<ArrayOf<IBigInteger*> > ptr1 = v1;
        }

        printArray(v2, " Copy result");
        AutoPtr<ArrayOf<IBigInteger*> > ptr2 = v2;
    }

    {
        printf("\n == CopyEx ==\n");
        AutoPtr<IBigInteger> i1, i2, i3, i4;
        CBigInteger::New(1, 1, (IBigInteger**)&i1);
        CBigInteger::New(1, 2, (IBigInteger**)&i2);
        CBigInteger::New(-1, 3, (IBigInteger**)&i3);
        CBigInteger::New(-1, 4, (IBigInteger**)&i4);
        IBigInteger* array[2];
        array[0] = i1;
        array[1] = i2;

        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        v1->Set(0, i3);
        v1->Set(2, i4);
        v1->Copy(array, 2);

        printArray(v1, " CopyEx result");
        AutoPtr<ArrayOf<IBigInteger*> > ptr1 = v1;
    }

    {
        printf("\n == Copy Buffer==\n");
        AutoPtr<IBigInteger> i1, i2, i21, i22, i23;
        CBigInteger::New(1, 1, (IBigInteger**)&i1);
        CBigInteger::New(1, 2, (IBigInteger**)&i2);
        CBigInteger::New(-1, 1, (IBigInteger**)&i21);
        CBigInteger::New(-1, 2, (IBigInteger**)&i22);
        CBigInteger::New(-1, 3, (IBigInteger**)&i23);

        IBigInteger* array[2];
        array[0] = i1;
        array[1] = i2;

        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        v1->Set(0, i21);
        v1->Set(1, i22);
        v1->Set(2, i23);
        v1->Copy(2, array, 2);

        printArray(v1, " Copy result");
        AutoPtr<ArrayOf<IBigInteger*> > ptr1 = v1;
    }

    {
        printf("\n == Copy IInterface==\n");
        AutoPtr<IBigInteger> i1, i2, i21, i22, i23;
        CBigInteger::New(1, 1, (IBigInteger**)&i1);
        CBigInteger::New(1, 2, (IBigInteger**)&i2);
        CBigInteger::New(-1, 1, (IBigInteger**)&i21);
        CBigInteger::New(-1, 2, (IBigInteger**)&i22);
        CBigInteger::New(-1, 3, (IBigInteger**)&i23);

        AutoPtr<ArrayOf<IInterface*> > v1 = ArrayOf<IInterface*>::Alloc(4);
        v1->Set(0, i21->Probe(EIID_IInterface));
        v1->Set(1, i22->Probe(EIID_IInterface));
        v1->Set(2, i23->Probe(EIID_IInterface));
        AutoPtr<ArrayOf<IInterface*> > v2 = ArrayOf<IInterface*>::Alloc(8);
        v2->Copy(v1);

        printArray(v2, " Copy result");
    }

    {
        printf("\n == Clone ==\n");
        AutoPtr<IBigInteger> i1, i2, i3;
        CBigInteger::New(1, 1, (IBigInteger**)&i1);
        CBigInteger::New(1, 2, (IBigInteger**)&i2);
        CBigInteger::New(1, 3, (IBigInteger**)&i3);

        ArrayOf<IBigInteger*> * v1 = ArrayOf<IBigInteger*>::Alloc(4);
        v1->Set(0, i1);
        v1->Set(1, i2);
        v1->Set(2, i3);

        ArrayOf<IBigInteger*> * v2 = v1->Clone();
        {
            AutoPtr<ArrayOf<IBigInteger*> > ptr1 = v1;
        }

        printArray(v2, " Clone result");
        AutoPtr<ArrayOf<IBigInteger*> > ptr2 = v2;
    }

     printf("==== return from testIInterface ====\n");
}


void printArray(ArrayOf<Drived *> * v, const char* info)
{
    printf("  >------Start print %p, %s ------<\n", v, info);
    Int32 len = v->GetLength();
    for (Int32 i = 0; i < len; ++i) {
        printf("    > %d", i);
        if ((*v)[i]) {
            (*v)[i]->Print();
        }
        else {
            printf("    NULL\n");
        }
    }
    printf("  >------End print %s ------<\n", info);
}

void testElRefBase()
{
    printf("\n==== enter testElRefBase ====\n");

    {
        AutoPtr<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(2);
        v1->Set(0, 1);
        v1->Set(1, 2);

        AutoPtr<ArrayOf<Int32> > v2 = ArrayOf<Int32>::Alloc(2);
        v2->Copy(v1);
    }

    {
        AutoPtr<ArrayOf<Double> > v1 = ArrayOf<Double>::Alloc(2);
        v1->Set(0, 1.0);
        v1->Set(1, 2.0);

        AutoPtr<ArrayOf<Double> > v2 = ArrayOf<Double>::Alloc(2);
        v2->Copy(v1);
    }

    {
        printf("\n == Copy ==\n");
        Drived* c1 = new Drived(1, 1);
        Drived* c2 = new Drived(2, 2);
        Drived* c3 = new Drived(3, 3);
        Drived* c4 = new Drived(4, 4);

        AutoPtr<ArrayOf<Drived*> > v1 = ArrayOf<Drived*>::Alloc(4);
        v1->Set(0, c1);
        v1->Set(1, c2);

        printArray(v1.Get(), " Copy source");

        AutoPtr<ArrayOf<Drived*> > v2 = ArrayOf<Drived*>::Alloc(4);
        v2->Set(0, c3);
        v2->Set(1, c4);
        v2->Copy(1, v1, 0, 2);

        printArray(v2, " Copy result");

    }

    printf("==== return from testElRefBase ====\n");
}

//=============================================================================

void doTestBasicType()
{
    printf("\n==== call DoTestBasicType ====\n");
    testBasicType();
    printf("==== call DoTestBasicType end ====\n");
}

void doTestString()
{
    printf("\n==== call DoTestString ====\n");
    testString();
    printf("==== call DoTestString end ====\n");
}

void doTestIInterface()
{
    printf("\n==== call doTestIInterface ====\n");
    testIInterface();
    printf("==== call doTestIInterface end ====\n");
}

void doTestElRefBase()
{
    printf("\n==== call doTestElRefBase ====\n");
    testElRefBase();
    printf("==== call doTestElRefBase end ====\n");
}

template <typename T>
AutoPtr<IObjectContainer> ArrayOfToObjectContainer(
    /* [in] */ ArrayOf<T>* array)
{
    assert(SUPERSUBCLASS_EX(IInterface*, T) && "Error: element type must extends IInterface.");
    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        container->Add((*array)[i]->Probe(EIID_IInterface));
    }
    return container;
}

template <typename T>
AutoPtr<IObjectContainer> ArrayOfToObjectContainer(
    /* [in] */ AutoPtr<ArrayOf<T> >& array)
{
    return ArrayOfToObjectContainer(array.Get());
}

void ConvertArrayOfToObjectContainer()
{
    AutoPtr<IBigInteger> i1, i2, i3;
    CBigInteger::New(1, 1, (IBigInteger**)&i1);
    CBigInteger::New(1, 2, (IBigInteger**)&i2);
    CBigInteger::New(1, 3, (IBigInteger**)&i3);

    AutoPtr< ArrayOf<IBigInteger*> > array = ArrayOf<IBigInteger*>::Alloc(3);
    array->Set(0, i1);
    array->Set(1, i2);
    array->Set(2, i3);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);

    for (Int32 i = 0; i < array->GetLength(); ++i) {
        if ((*array)[i] != NULL) {
            container->Add((*array)[i]);
        }
    }

    AutoPtr<IObjectContainer> container2 = ArrayOfToObjectContainer(array);
    Int32 count;
    container2->GetObjectCount(&count);
    printf(" container2 length %d", count);
}

void GetArrayOf(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    *result = ArrayOf<Int32>::Alloc(5);
    (*result)->AddRef();
    for (Int32 i = 0; i < (*result)->GetLength(); ++i) {
        (**result)[i] = 10 + i;
    }
}

void SetObject(
    /* [in] */ ArrayOf<Drived*>* array,
    /* [in] */ Int32 index,
    /* [in] */ Drived* obj)
{
    assert(array != NULL);
    array->Set(index, obj);
}

void doTestBasicUsages()
{
    {
        AutoPtr<ArrayOf<Int32> > array;
        GetArrayOf((ArrayOf<Int32>**)&array);

        printf("testBasicUsages(): out, callee: ");
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            printf("%d ", (*array)[i]);
        }
        printf("\n");
    }

    {
        {
            AutoPtr<ArrayOf<Drived*> > array = ArrayOf<Drived*>::Alloc(1);
            SetObject(array, 0, new Drived());
        }

        {

            AutoPtr<Drived> d = new Drived();
            {
                AutoPtr<ArrayOf<Drived*> > array = ArrayOf<Drived*>::Alloc(1);
                SetObject(array, 0, d);
                printf("After SetObject\n");
            }
        }
    }

    {
        printf("\n=======One callee=========\n");
        AutoPtr<ArrayOf<Int32> > array;
        GetArrayOf((ArrayOf<Int32>**)&array);
        printf("1, RefCount: %d\n", array->GetRefCount());
    }

    {
        printf("\n=======Two callee=========\n");
        AutoPtr<ArrayOf<Int32> > array2 = ArrayOf<Int32>::Alloc(2);
        AutoPtr<ArrayOf<Int32> > array;
        GetArrayOf((ArrayOf<Int32>**)&array);
        printf("1, RefCount: %d\n", array->GetRefCount());

        {
            AutoPtr<ArrayOf<Int32> > a = array;
            printf("2, RefCount: %d\n", array->GetRefCount());

            AutoPtr<ArrayOf<Int32> > b = array;
            printf("3, RefCount: %d\n", array->GetRefCount());

            AutoPtr<ArrayOf<Int32> > c = array;
            printf("4, RefCount: %d\n", array->GetRefCount());
        }

        printf("5, RefCount: %d\n", array->GetRefCount());

        array = array2;
        printf("6, RefCount: %d\n", array->GetRefCount());
    }
}

void testArray2()
{
    {
        printf("\n================ Style 1 ================\n");
        AutoPtr<ArrayOf2<String>::Type > arrays = ArrayOf2<String>::Type::Alloc(10);
        for (Int32 i = 0; i < arrays->GetLength(); ++i) {
            ArrayOf2<String>::ElementType array = ArrayOf<String>::Alloc(1);

            StringBuilder sb("inner array: ");
            sb += i;
            array->Set(0, sb.ToString());
            arrays->Set(i, array);
        }

        for (Int32 i = 0; i < arrays->GetLength(); ++i) {
            printArray((*arrays)[i], "ArrayOf2 style 1");
        }
    }

    {
        printf("\n================ Style 2 ================\n");
        AutoPtr<ArrayOf<StringArray> > arrays = ArrayOf<StringArray>::Alloc(10);

        for (Int32 i = 0; i < arrays->GetLength(); ++i) {
            StringArray array = ArrayOf<String>::Alloc(1);

            StringBuilder sb("inner array: ");
            sb += i;
            array->Set(0, sb.ToString());
            arrays->Set(i, array);
        }

        for (Int32 i = 0; i < arrays->GetLength(); ++i) {
            printArray((*arrays)[i], "ArrayOf2 style 2");
        }
    }

    {
        printf("\n================ test Alloc big size ================\n");
        AutoPtr<ArrayOf<char> > array = ArrayOf<char>::Alloc(1024 * 1024 * 10); // 10 M
        printf("Step 1\n");
        array->Set(0, 1);
        printf("Step 2\n");
    }
}

void testStringBuf()
{
    StringBuf_<11> buf;
    StringBuf * strBuf = buf.Clone();
    Int32 len = strBuf->GetLength();
    Int32 ncap = strBuf->GetCapacity();
    if (len != 0 || ncap != 11 || strcmp((char *)(*strBuf), "")) {
        printf("strBuf = %s, len = %d, ncap = %d\n", (char *)strBuf, len, ncap);
        goto Exit;
    }

Exit:
    StringBuf::Free(strBuf);
}

void PrintArrayOfString(
    const ArrayOf<String>& array)
{
    printf("======ENTER PrintArrayOfString ==========\n");
    for (int i = 0; i < array.GetLength(); ++i)
    {
        printf(" > %d, value: [%s]\n", i, array[i].string());
    }
    printf("======LEAVE PrintArrayOfString ==========\n");
}
//=============================================================================

void testArrayOfString()
{
    printf("======ENTER testArrayOfString ==========\n");
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    array->Set(0, String("ONE"));
    array->Set(1, String("TWO"));

    PrintArrayOfString(*array);
    printf("======LEAVE testArrayOfString ==========\n");
}

void testSelfCopy()
{
    static Int32 ARRAY_LENGTH = 10;
    static Int32 SMALL_OFFSET = 1;
    static Int32 LARGE_OFFSET = 4;
    static Int32 COPY_COUNT = 5;

    printf("====================Self Copy T After======================\n");
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(ARRAY_LENGTH);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        array->Set(i, i);
    }

    array->Copy(LARGE_OFFSET, array, SMALL_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        printf("%d ", (*array)[i]);
    }

    printf("\n====================Self Copy T Before======================\n");
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        array->Set(i, i);
    }

    array->Copy(SMALL_OFFSET, array, LARGE_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        printf("%d ", (*array)[i]);
    }
    printf("\n");

    printf("\n====================Self Copy String After======================\n");
    AutoPtr<ArrayOf<String> > stringArray = ArrayOf<String>::Alloc(ARRAY_LENGTH);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        stringArray->Set(i, str);
    }

    stringArray->Copy(LARGE_OFFSET, stringArray, SMALL_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < stringArray->GetLength(); ++i) {
        printf("%s ", (*stringArray)[i].string());
    }

    printf("\n====================Self Copy String Before======================\n");
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        stringArray->Set(i, str);
    }

    stringArray->Copy(SMALL_OFFSET, stringArray, LARGE_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < stringArray->GetLength(); ++i) {
        printf("%s ", (*stringArray)[i].string());
    }
    printf("\n");

    printf("\n====================Self Copy IInterface After======================\n");
    AutoPtr<ArrayOf<ICharSequence* > > objArray = ArrayOf<ICharSequence*>::Alloc(ARRAY_LENGTH);
    for (Int32 i = 0; i < objArray->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        objArray->Set(i, seq);
    }

    objArray->Copy(LARGE_OFFSET, objArray, SMALL_OFFSET, COPY_COUNT);
    String info;
    for (Int32 i = 0; i < objArray->GetLength(); ++i) {
        (*objArray)[i]->ToString(&info);
        printf("%s ", info.string());
    }

    printf("\n====================Self Copy IInterface Before======================\n");
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        objArray->Set(i, seq);
    }

    objArray->Copy(SMALL_OFFSET, objArray, LARGE_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < objArray->GetLength(); ++i) {
        (*objArray)[i]->ToString(&info);
        printf("%s ", info.string());
    }
    printf("\n");

    printf("\n====================Self Copy AutoPtr After======================\n");
    AutoPtr<ArrayOf<AutoPtr<ICharSequence> > > autoPtrArray = ArrayOf<AutoPtr<ICharSequence> >::Alloc(ARRAY_LENGTH);
    for (Int32 i = 0; i < autoPtrArray->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        autoPtrArray->Set(i, seq);
    }

    autoPtrArray->Copy(LARGE_OFFSET, autoPtrArray, SMALL_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < autoPtrArray->GetLength(); ++i) {
        (*autoPtrArray)[i]->ToString(&info);
        printf("%s ", info.string());
    }

    printf("\n====================Self Copy AutoPtr Before======================\n");
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        String str =  StringUtils::Int32ToString(i);
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        autoPtrArray->Set(i, seq);
    }

    autoPtrArray->Copy(SMALL_OFFSET, autoPtrArray, LARGE_OFFSET, COPY_COUNT);
    for (Int32 i = 0; i < autoPtrArray->GetLength(); ++i) {
        (*autoPtrArray)[i]->ToString(&info);
        printf("%s ", info.string());
    }
    printf("\n");
}

void testQuintet()
{
    // doTestArrayOfFreeRelease();
    // doTestBufferOfFreeRelease();
    // doTestAssignOnStack();
    // doTestSelfAssign();
    // doTestBasicType();
    // doTestString();
    // doTestIInterface();
    // doTestElRefBase();
    // doTestBasicUsages();

    // ConvertArrayOfToObjectContainer();

    // testArray2();
    // testStringBuf();

    // testArrayOfString();
    testSelfCopy();
}

int main(int argc, char *argv[])
{
    printf("==== call testQuintet ====\n");
    testQuintet();
    printf("\n==== call testQuintet end ====\n");

    return 0;
}
