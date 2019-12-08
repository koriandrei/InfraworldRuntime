#pragma once

#include <vector>

#include "FunctionArgumentHelper.h"

#include <type_traits>
#include <string>

template <class T>
static void WriteType(T Value);

template <class TOneOfCase, TOneOfCase Case>
struct TOneOfCaseHelper
{
    typedef void BoundType;
};

template <class TOneOfCase>
struct TOneOfWrapper
{
private:
    template <TOneOfCase Case>
    struct TOneOfCaseTypeStub
    {
        struct FCaseStub{};
    };

    template <bool Dummy, TOneOfCase... PossibleCases>
    class TOneOfTypeImpl;

    class FOneOfTypeImplBase
    {
    public:
        bool IsSet() const
        {
            return bIsSet;
        }

    protected:
        template <TOneOfCase Case, class T>
        void SetInternal(const T& Value)
        {
            CurrentCase = Case;
            bIsSet = true;
            bytes.resize(sizeof(T));

            unsigned char* DataStart = bytes.data();

            new (DataStart) T(Value);

            return;

            const unsigned char* const ValueStart = static_cast<const unsigned char*>(static_cast<const void*>(&Value));



            for (int ByteIndex = 0; ByteIndex < sizeof(T); ++ByteIndex)
            {
                const unsigned char* const SourcePointer = ValueStart + ByteIndex;
                
                bytes[ByteIndex] = *SourcePointer;
            }
        }

        template <TOneOfCase Case, class T>
        bool TryGetInternal(T& OutValue) const
        {
            if (!bIsSet || CurrentCase != Case)
            {
                return false;
            }

            const unsigned char* const DataStart = bytes.data();

            const T* TypedDataStart = static_cast<const T*>(static_cast<const void*>(DataStart));

            OutValue = *TypedDataStart;

            return true;

            unsigned char* const ValueStart = static_cast<unsigned char*>(static_cast<void*>(&OutValue));
            for (int ByteIndex = 0; ByteIndex < sizeof(T); ++ByteIndex)
            {
                unsigned char* DestinationPointer = ValueStart + ByteIndex;

                *DestinationPointer = bytes[ByteIndex];
            }

        }
        
    private:
        std::vector<unsigned char> bytes;
        bool bIsSet = false;
        TOneOfCase CurrentCase;
    };

    template <bool Dummy>
    class TOneOfTypeImpl<Dummy> : public FOneOfTypeImplBase
    {
    public:
        template <TOneOfCase DummyCase, class TDummyType>
        void Set(TOneOfCaseTypeStub<DummyCase>, const TDummyType& DummyValue);
        bool TryGet();
    };

    template <bool Dummy, TOneOfCase Case, TOneOfCase... RemainingCases>
    class TOneOfTypeImpl<Dummy, Case, RemainingCases...> : public TOneOfTypeImpl<Dummy, RemainingCases...>
    {
        typedef FOneOfTypeImplBase Base;
        typedef TOneOfCaseHelper<TOneOfCase, Case> FCaseHelper;
        typedef TOneOfTypeImpl<Dummy, RemainingCases...> Super;

        typedef typename FCaseHelper::BoundType BoundType;

    public:
        using Super::Set;
        using Super::TryGet;
        void Set(TOneOfCaseTypeStub<Case>, const BoundType& Value)
        {
            // WriteType(Value);
            this->template SetInternal<Case>(Value);
        }

        bool TryGet(TOneOfCaseTypeStub<Case>, BoundType& OutValue) const
        {
            return this->template TryGetInternal<Case>(OutValue);
        }
    };
public:
    template <TOneOfCase... PossibleCases>
    class TOneOfType
    {
    public:
        bool IsSet() const
        {
            return Impl.IsSet();
        }
        
        TOneOfCase GetCase();
        
        template <TOneOfCase Case, class T>
        void Set(const T& Value)
        {
            typedef TOneOfCaseTypeStub<Case> StubType;
            typedef TArgumentHelper<StubType> ArgumentTypeHelper;

            // typedef typename decltype(ArgumentTypeHelper::CreateMemberFunction(&OneOfImplT::Set))::SecondArg BoundSecondArgType;

            // BoundSecondArgType Converted = BoundSecondArgType(Value);

            // static_assert(std::is_same_v<std::string, BoundSecondArgType>);



            Impl.Set(StubType(), Value);
        }

        template <TOneOfCase Case, class T>
        bool TryGet(T& OutValue)
        {
            return Impl.TryGet(TOneOfCaseTypeStub<Case>(), OutValue);
        }
    private:
        typedef TOneOfTypeImpl</*Dummy =*/ true, PossibleCases...> OneOfImplT;
        OneOfImplT Impl;
    };
};
