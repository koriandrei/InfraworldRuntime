#pragma once

#include "OneOf.h"

template <class TOneOfStruct>
struct TOneOfProtoHelper0
{
    typedef void CaseEnumType;
};

template <class TProtoStruct, class TUnrealOneOfStruct>
struct TOneOfProtoHelper1
{
    typedef void ProtoCaseEnum;
};

template <class TProtoStruct, class TUnrealOneOfStruct>
struct TOneOfProtoHelper2
{
    typedef typename TOneOfProtoHelper1<TProtoStruct, TUnrealOneOfStruct>::ProtoCaseEnum ProtoCaseEnumT;
    static constexpr ProtoCaseEnumT TProtoStruct::* ProtoCaseMemberPtr = nullptr;
};

template <class TUnrealOneOfStruct, class TProtoStruct>
auto GetCurrentProtoCase(const TProtoStruct& ProtoStruct)
{
    typedef typename TOneOfProtoHelper0<TUnrealOneOfStruct>::CaseEnumType UeCaseEnumT;
    typedef typename TOneOfProtoHelper1<TProtoStruct, TUnrealOneOfStruct>::ProtoCaseEnum ProtoCaseEnumT;

    auto ProtoCaseMemberPtr = TOneOfProtoHelper2<TProtoStruct, TUnrealOneOfStruct>::ProtoCaseMemberPtr;
    ProtoCaseEnumT ProtoCase = ProtoStruct.*ProtoCaseMemberPtr;
    return ProtoCase;
}

template <class TProtoCaseEnum, TProtoCaseEnum Value>
struct TProtoOneOfCaseHelper
{
    struct FInternal
    {
        int ExampleValue;
    };

    static constexpr int FInternal::* MemberPointer = &FInternal::ExampleValue;
};

template <class TProtoCaseEnum>
struct TOneOfProtoHelper3
{
    static constexpr TProtoCaseEnum AllowedValues[] = {  };
};


template <class TEnum, template<TEnum> class Strategy, int CurrentCaseIndex, int PossibleCasesCount, const TEnum PossibleCases[PossibleCasesCount]>
struct TEnumAttemptHelper
{
    static void Attempt(const TEnum ActualCase) 
    {
        constexpr TEnum ConsideredCase = PossibleCases[CurrentCaseIndex];
        if (ActualCase != ConsideredCase)
        {
            TEnumAttemptHelper<TEnum, Strategy, CurrentCaseIndex + 1, PossibleCasesCount, PossibleCases>::Attempt(ActualCase);
            return;
        }

        Strategy<ConsideredCase>()();
    }
};

template <class TEnum, template<TEnum> class Strategy, int CasesCount, const TEnum PossibleCases[CasesCount]>
struct TEnumAttemptHelper<TEnum, Strategy, CasesCount, CasesCount, PossibleCases>
{
    static void Attempt(const TEnum ActualCase) {}
};

template <class T>
struct TDummyHandler
{
    template <T Value>
    struct THandlerImpl1
    {
        void operator()()
        {}
    };
};

template <class TProtoCaseEnum>
struct TOneOfProtoHelper4
{
    template <class T, int N>
    struct TArrayElementCountHelper
    {
        static constexpr int ElementsCount = N;
    };

    template <class T, int N>
    static constexpr int GetArrayLength(T(&)[N])
    {
        return TArrayElementCountHelper<T, N>::ElementsCount;
    }

    static void Attempt(const TProtoCaseEnum CurrentCase)
    {
typedef TDummyHandler<TProtoCaseEnum> HandlerOuterT;

        TEnumAttemptHelper<
            TProtoCaseEnum, 
            HandlerOuterT::template THandlerImpl1, 
            0, 
            GetArrayLength(TOneOfProtoHelper3<TProtoCaseEnum>::AllowedValues), 
            TOneOfProtoHelper3<TProtoCaseEnum>::AllowedValues
            >::Attempt(CurrentCase);
    }
};

template <class TProtoCaseEnum>
struct TOneOfProtoHelper5
{
    template <TProtoCaseEnum Case>
    void operator()()
    {
        // const auto& CurrentProtoValue  TProtoOneOfCaseHelper<TProtoCaseEnum, Case>::MemberPointer;
    }
};

template <class TProtoStruct, class TOneOfStruct>
struct TOneOfCastUtils
{
    static void CastProtoToUnreal(const TProtoStruct& Proto, TOneOfStruct& OutUnreal)
    {
        typedef typename TOneOfProtoHelper0<TOneOfStruct>::CaseEnumType UeCaseEnumT;
        typedef typename TOneOfProtoHelper1<TProtoStruct, TOneOfStruct>::ProtoCaseEnum ProtoCaseEnumT;

        const ProtoCaseEnumT CurrentProtoCase = GetCurrentProtoCase<TOneOfStruct>(Proto);

        TOneOfProtoHelper4<ProtoCaseEnumT>::Attempt(CurrentProtoCase); 
    }
};
