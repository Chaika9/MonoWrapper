#pragma once

#include "MemberInfo.h"
#include <mono/metadata/object.h>

namespace Mono {
    template<typename T>
    class FieldInvoker;

    template<typename T>
    FieldInvoker<T> makeFieldInvoker(const FieldInfo &field);

    class FieldInfo : public MemberInfo {
    public:
        FieldInfo() = default;

        explicit FieldInfo(const Type &type, non_owning_ptr<MonoClassField> field);

        /**
         * @brief Get raw MonoClassField pointer.
         * @return Raw MonoClassField pointer.
         */
        non_owning_ptr<MonoClassField> get() const;

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        Object getCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Check if this field has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this field has a specific custom attribute with specified type, false otherwise.
         */
        bool hasCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Get type of this field.
         * @return Type of this field.
         */
        Type getType() const;

        /**
         * @brief Get value of this field.
         * @tparam RetType Return type of field.
         * @param object Object to get value from.
         * @return Value of this field.
         */
        template<typename RetType>
        RetType getValue(const Object &object) {
            auto fieldInvoker = Mono::makeFieldInvoker<RetType>(*this);
            return fieldInvoker.getValue(object);
        }

        /**
         * @brief Set value of this field.
         * @tparam T Type of value to set.
         * @param object Object to set value to.
         * @param value Value to set.
         */
        template<typename T>
        void setValue(const Object &object, T value) {
            auto fieldInvoker = Mono::makeFieldInvoker<T>(*this);
            fieldInvoker.setValue(object, value);
        }

    protected:
        non_owning_ptr<MonoClassField> _field;
        Type _type;
    };
}