#pragma once
#include <type_traits>

template <class TFirstArg>
struct TArgumentHelper
{
    template <class TSecondArg>
    struct TFunctionPointer
    {
        typedef void (*FuncType)(TFirstArg, TSecondArg);
        typedef TSecondArg SecondArg; 
    };

    template <class TSecondArg>
    // static TFunctionPointer<TSecondArg> Create(typename TFunctionPointer<TSecondArg>::FuncType FuncPtr) { return TFunctionPointer<TSecondArg>(); }
    static TFunctionPointer<TSecondArg> CreateFreeFunction(void (*FuncPtr)(TFirstArg, TSecondArg)) { return TFunctionPointer<TSecondArg>(); }
    template <class TOwnerClass, class TSecondArg>
    static TFunctionPointer<TSecondArg> CreateMemberFunction(void (TOwnerClass::*FuncPtr)(TFirstArg, TSecondArg)) { return TFunctionPointer<TSecondArg>(); }
};

void F(int Arg1, int Arg2);
void F(char Arg1, char Arg2);

struct FMyTestClass
{
    void F(int Arg1, int Arg2);
    void F(char Arg1, char Arg2);
};

// int main(int argc, char** argv)
// {
//     typedef decltype(TArgumentHelper<char>::CreateFreeFunction(&F)) FreeFunctionPointerHelperT;
//     typedef typename FreeFunctionPointerHelperT::SecondArg FreeSecondArg;
//     static_assert(std::is_same_v<char, FreeSecondArg>);

//     typedef decltype(TArgumentHelper<char>::CreateMemberFunction(&FMyTestClass::F)) MemberFunctionPointerHelperT;
//     typedef typename MemberFunctionPointerHelperT::SecondArg MemberSecondArg;
//     static_assert(std::is_same_v<char, MemberSecondArg>);

//     return 0;
// }
