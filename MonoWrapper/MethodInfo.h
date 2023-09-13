#pragma once

#include "MemberInfo.h"
#include "Object.h"
#include <mono/metadata/object.h>

namespace Mono {
    class Object;

    class MethodInfo : public MemberInfo {
    public:
        explicit MethodInfo(const Type& type, non_owning_ptr<MonoMethod> method);

        /**
         * @brief Get raw MonoMethod pointer.
         * @return Raw MonoMethod pointer.
         */
        non_owning_ptr<MonoMethod> get() const;

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        Object getCustomAttribute(const Type &attributeType) override;

        /**
         * @brief Check if this method has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this method has a specific custom attribute with specified type, false otherwise.
         */
        bool hasCustomAttribute(const Type &attributeType) override;

        bool operator==(const MethodInfo &other) const;

        bool operator!=(const MethodInfo &other) const;

    protected:
        non_owning_ptr<MonoMethod> _method = nullptr;
    };
}