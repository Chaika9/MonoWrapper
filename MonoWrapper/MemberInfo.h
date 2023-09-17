#pragma once

#include "Type.h"
#include <string>

namespace Mono {
    class Type;

    class Object;

    class MemberInfo {
    public:
        MemberInfo() = default;

        explicit MemberInfo(const Type &declaringType, const std::string &name);

        /**
         * @brief Get the name of the member.
         * @return Name of the member.
         */
        virtual std::string getName() const;

        /**
         * @brief Get the type that this member belongs to.
         * @return Type that this member belongs to.
         */
        virtual const Type &getDeclaringType() const;

        /**
         * @brief Check if this member is static.
         * @return True if this member is static, false otherwise.
         */
        virtual bool isStatic() const = 0;

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        virtual Object getCustomAttribute(const Type &attributeType) = 0;

        /**
         * @brief Check if this member has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this member has a specific custom attribute with specified type, false otherwise.
         */
        virtual bool hasCustomAttribute(const Type &attributeType) = 0;

        virtual bool operator==(const MemberInfo &other) const;

        virtual bool operator!=(const MemberInfo &other) const;

    protected:
        Type _declaringType;
        std::string _name;
    };
}