#pragma once

#include "MemberInfo.h"
#include "MethodInvoker.h"
#include <mono/metadata/object.h>

namespace Mono {
    class PropertyInfo : public MemberInfo {
    public:
        PropertyInfo() = default;

        explicit PropertyInfo(const Type &type, non_owning_ptr<MonoProperty> property);

        /**
         * @brief Get raw MonoProperty pointer.
         * @return Raw MonoProperty pointer.
         */
        non_owning_ptr<MonoProperty> get() const;

        /**
         * @brief Check if this property is static.
         * @return True if this property is static, false otherwise.
         */
        bool isStatic() const override;

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        Object getCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Check if this property has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this property has a specific custom attribute with specified type, false otherwise.
         */
        bool hasCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Get get method of this property. (e.g. "get_MyProperty")
         * @return Get method info.
         */
        MethodInfo getGetMethod() const;

        /**
         * @brief Get set method of this property. (e.g. "set_MyProperty")
         * @return Set method info.
         * @throws AccessViolationException If property is read-only.
         */
        MethodInfo getSetMethod() const;

        /**
         * @brief Get value of this property.
         * @tparam RetType Return type of property.
         * @param object Object to get value from.
         * @return Value of this property.
         */
        template<typename RetType>
        RetType getValue(const Object &object) {
            auto methodInvoker = Mono::makeMethodInvoker<RetType()>(getGetMethod());
            return methodInvoker(object);
        }

        /**
         * @brief Set value of this property.
         * @tparam T Type of value to set.
         * @param object Object to set value to.
         * @param value Value to set.
         * @throws AccessViolationException If property is read-only.
         */
        template<typename T>
        void setValue(const Object &object, T value) {
            auto methodInvoker = Mono::makeMethodInvoker<void(T)>(getSetMethod());
            methodInvoker(object, std::forward<T>(value));
        }

    protected:
        non_owning_ptr<MonoProperty> _property;
    };
}