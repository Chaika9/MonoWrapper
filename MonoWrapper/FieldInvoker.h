#pragma once

#include "TypeConversion.h"
#include "FieldInfo.h"
#include <mono/metadata/object.h>
#include <memory>

namespace Mono {
    class Object;

    class FieldInfo;

    template<typename T>
    class FieldInvoker {
    public:
        explicit FieldInvoker(const FieldInfo &field) : _field(field) {}

        void setValue(const Object &object, T value) const {
            setValueImpl(&object, std::forward<T>(value));
        }

        T getValue(const Object &object) const {
            return getValueImpl(&object);
        }

    private:
        void setValueImpl(const Object *object, T value) const {
            auto monoValue = ConvertType<std::decay_t<T>>::toMono(std::forward<T>(value));
            auto arg = toMonoArg(monoValue);
            mono_field_set_value(object->get(), _field.get(), arg);
        }

        T getValueImpl(const Object *object) const {
            T val{};

            auto arg = reinterpret_cast<void *>(&val);
            MonoObject *objValue = nullptr;
            if (!_field.getType().isValueType()) {
                arg = &objValue;
            }

            mono_field_get_value(object->get(), _field.get(), arg);
            if (!_field.getType().isValueType() && objValue != nullptr) {
                val = ConvertType<T>::fromMonoBoxed(objValue);
            }
            return val;
        }

    protected:
        const FieldInfo &_field;
    };

    /**
     * @brief Create a FieldInvoker Helper. This helper can be used to get/set value of a field.
     * @param field The mono field to get/set value.
     * @return The FieldInvoker Helper.
     */
    template<typename T>
    FieldInvoker<T> makeFieldInvoker(const FieldInfo &field) {
        return FieldInvoker<T>(field);
    }
}