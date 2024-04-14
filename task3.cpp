#include <type_traits>
#include <string>
#include <vector>
#include <any>
#include <iostream>

namespace TypeListDetail {
    // Определение базового шаблона
    template <typename... Types>
    struct TypeList {};



    template <typename... Types>
    struct Size;

    template <typename... Types>
    struct Size<TypeList<Types...>> {
        static constexpr std::size_t value = sizeof...(Types);
    };



    template <std::size_t N, typename TypeList>
    struct TypeAt;

    template <std::size_t N, typename Head, typename... Tail>
    struct TypeAt<N, TypeList<Head, Tail...>> {
        using type = typename TypeAt<N - 1, TypeList<Tail...>>::type;
    };

    template <typename Head, typename... Tail>
    struct TypeAt<0, TypeList<Head, Tail...>> {
        using type = Head;
    };



    template <typename T, typename TypeList>
    struct Contains;

    template <typename T, typename... Types>
    struct Contains<T, TypeList<Types...>> {
        static constexpr bool value = (std::is_same_v<T, Types> || ...);
    };



    template <typename T, typename TypeList, std::size_t Index = 0>
    struct IndexOf;

    template <typename T, typename Head, typename... Tail, std::size_t Index>
    struct IndexOf<T, TypeList<Head, Tail...>, Index> {
        static constexpr std::size_t value = std::is_same_v<T, Head> ? Index : IndexOf<T, TypeList<Tail...>, Index + 1>::value;
    };

    template <typename T, std::size_t Index>
    struct IndexOf<T, TypeList<>, Index> {
        static constexpr std::size_t value = static_cast<std::size_t>(-1);
    };



    template <typename T, typename TypeList>
    struct PushBack;

    template <typename T, typename... Types>
    struct PushBack<T, TypeList<Types...>> {
        using type = TypeList<Types..., T>;
    };



    template <typename T, typename TypeList>
    struct PushFront;

    template <typename T, typename... Types>
    struct PushFront<T, TypeList<Types...>> {
        using type = TypeList<T, Types...>;
    };
}

template <typename... Types>
using TypeList = TypeListDetail::TypeList<Types...>;



//typemap: {typelist, vector<any>}
template<typename... Args>
struct TypeMap;

template<typename Head, typename... Tail>
struct TypeMap<Head, Tail...>
{
    TypeMap()
    {
        using keys = TypeList<Head, Tail...>;
        values.resize(sizeof...(Tail)+1);
    };

    template<typename T> void addValue(T value)
    {
        constexpr int i = TypeListDetail::IndexOf<T, keys>::value;
        values[i] = std::any(value);
    };

    template<typename T> T getValue()
    {
        int i = TypeListDetail::IndexOf<T, keys>::value;
        T from_any = std::any_cast<T>(values[i]);
        return from_any;
    };

    template<typename T> bool Contains()
    {
        return TypeListDetail::Contains<T, keys>::value;
    }

    typedef TypeList<Head, Tail...>     keys;
    std::vector<std::any>               values;

    template<typename T> void removeValue()
    {
        constexpr int i = TypeListDetail::IndexOf<T, keys>::value;
        values[i] = std::any(nullptr);
    };
};

int main()
{
    struct DataA {
    std::string value;
    };
    struct DataB {
    int value;
    };
    TypeMap<int, DataA, double, DataB> myTypeMap;
    // Добавление элементов в контейнер
    myTypeMap.addValue<int>(42);
    myTypeMap.addValue<double>(3.14);
    myTypeMap.addValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.addValue<DataB>({10});

    // Получение и вывод значений по типам ключей
    std::cout << "Value for int: "<<myTypeMap.getValue<int>() << std::endl; // Вывод: 42
    std::cout << "Value for double: " <<
    myTypeMap.getValue<double>() << std::endl; // Вывод: 3.14
    std::cout << "Value for DataA: " <<
    myTypeMap.getValue<DataA>().value << std::endl; // Вывод: Hello, TypeMap!
    std::cout << "Value for DataB: " <<
    myTypeMap.getValue<DataB>().value << std::endl; // Вывод: 10

    // Проверка наличия элемента
    std::cout << "Contains int? " <<
    (myTypeMap.Contains<int>() ? "Yes" : "No") << std::endl; //Вывод: Yes

    // Удаление элемента
    myTypeMap.removeValue<double>();
    // Попытка получения удаленного элемента
    std::cout << "Value for double after removal: " <<
    myTypeMap.getValue<double>() << std::endl; // Вывод: (некорректное значение)
    return 0;
};