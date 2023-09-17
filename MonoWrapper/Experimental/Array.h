#pragma once

#include "../Type.h"
#include "../NonOwningPtr.h"
#include <mono/metadata/appdomain.h>
#include <stdexcept>

namespace Mono {
    template<typename T>
    struct ConvertType;
}

namespace Mono::Experimental {
    class Array {
    public:
        Array() = default;

        explicit Array(non_owning_ptr<MonoArray> array);

        /**
         * @brief Create a new array with specified type and size.
         * @param type Type of array.
         * @param size Size of array.
         * @return New array.
         * @throws TargetInvocationException If failed to create array.
         */
        static Array newArray(const Type &type, std::size_t size);

        /**
         * @brief Create a new array with specified size. (Type is System.Object)
         * @param size Size of array.
         * @return New array.
         * @throws TargetInvocationException If failed to create array.
         */
        static Array newArray(std::size_t size);

        /**
         * @brief Get raw MonoArray pointer.
         * @return Raw MonoArray pointer.
         */
        non_owning_ptr<MonoArray> get() const;

        /**
         * @brief Get length of this array.
         * @return Length of this array.
         */
        std::size_t length() const;

        /**
         * @brief Get element at specified index. (Only Object type)
         * @tparam T Type of element.
         * @param index Index of element.
         * @return Element at specified index.
         * @throws std::out_of_range If index is out of range.
         */
        template<typename T>
        T get(std::size_t index) const {
            if (index >= _length) {
                throw std::out_of_range("index out of range");
            }
            auto element = mono_array_get(_array, MonoObject *, index);
            return ConvertType<std::decay_t<T>>::fromMonoBoxed(element);
        }

        /**
         * @brief Set element at specified index. (Only Object type)
         * @tparam T Type of element.
         * @param index Index of element.
         * @param value Value to set.
         * @throws std::out_of_range If index is out of range.
         */
        template<typename T>
        void set(std::size_t index, const T &value) const {
            if (index >= _length) {
                throw std::out_of_range("index out of range");
            }
            auto element = ConvertType<std::decay_t<T>>::toMono(value);
            mono_array_setref(_array, index, element);
        }

    protected:
        non_owning_ptr<MonoArray> _array = nullptr;
        std::size_t _length = 0;
    };
}