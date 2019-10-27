#include <c-lo/detail/type_traits.hpp>

#include <array>

struct TestStructure {
    void execute() {}
    void execute_with_arg(int) {}
    void execute_const() const {}
};

void execute(TestStructure) {}

template <typename T>
using execute_member = decltype(std::declval<T>().execute());

static_assert(
    c_lo::detail::is_detected_v<
        execute_member,
        TestStructure
    >,
    "non-const member should be detected"
);

static_assert(
    !c_lo::detail::is_detected_v<
        execute_member,
        const TestStructure
    >,
    "non-const member should not be detected for const TestStructure"
);

template <typename T>
using non_existant_member = decltype(std::declval<T>().non_existant());

static_assert(
    !c_lo::detail::is_detected_v<
        non_existant_member,
        TestStructure
    >,
    "non-existant member should not be detected"
);

template <typename T, typename Arg>
using execute_with_arg_member = decltype(std::declval<T>().execute_with_arg(std::declval<Arg>()));

static_assert(
    c_lo::detail::is_detected_v<
        execute_with_arg_member,
        TestStructure,
        int
    >,
    "non-const member with arg should be detected"
);

static_assert(
    !c_lo::detail::is_detected_v<
        execute_with_arg_member,
        const TestStructure,
        int
    >,
    "non-const member with arg should not be detected if TestStructure is const"
);

static_assert(
    !c_lo::detail::is_detected_v<
        execute_with_arg_member,
        TestStructure
    >,
    "non-const member with arg should not be detected if args not correct"
);

template <typename T>
using execute_const_member = decltype(std::declval<T>().execute_const());

static_assert(
    c_lo::detail::is_detected_v<
        execute_const_member,
        TestStructure
    >,
    "const member should be detected"
);

static_assert(
    c_lo::detail::is_detected_v<
        execute_const_member,
        const TestStructure
    >,
    "const member should be detected with const TestStructure"
);

template <typename T>
using execute_free = decltype(execute(std::declval<T>()));

static_assert(
    c_lo::detail::is_detected_v<
        execute_free,
        TestStructure
    >,
    "free function should be detected"
);

template <typename T>
using non_existant_free = decltype(non_existant(std::declval<T>()));

static_assert(
    !c_lo::detail::is_detected_v<
        non_existant_free,
        TestStructure
    >,
    "non-existant free function should not be detected"
);

int main() {}