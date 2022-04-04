#include <tuple>
#include <optional>
#include <iostream>

namespace StrongTypes
{
    template <typename T, typename Parameter>
    class NamedType : public T
    {
    public:
        explicit NamedType(T const& value) : T(value) {}
        explicit NamedType(T&& value) : T(std::move(value)) {}
    };

    namespace
    {
        //Usage: auto tpl = StrongTypes::createTuple<std::tuple<ColPosition, ColMaxPosition>>(ColPosition(12), ColMaxPosition(101));
        template <class T> struct tplMake;
        template <class... args> struct tplMake<std::tuple<args...>>
        {
            operator std::tuple<args...>(){
                return std::make_tuple( args(std::nullopt)... );
            }
        };
    }    
    //MSMS::TODO - Requires some checks on whether the argtype is in tuple.
    template<typename tupleType, typename... argType> tupleType createTuple(argType... args)
    {
        tupleType tpl = tplMake<tupleType>();
        ((std::get<argType>(tpl) = args ), ...);    
        return tpl;
    }
}

using ArgA = StrongTypes::NamedType<std::optional<size_t>, struct _arga>;
using ArgB = StrongTypes::NamedType<std::optional<size_t>, struct _argb>;

using allowedTuple = std::tuple<ArgA, ArgB>;

template<class... T> void test(T... ts)
{
    auto tpl = StrongTypes::createTuple<allowedTuple>(ts...);
    std::cout << std::get<ArgA>(tpl).value_or(0) << std::endl;

}

int main()
{
    test(); //prints 0
    test(ArgB(100)); //prints 0
    test(ArgA(12)); //prints 12
    return 0;
}