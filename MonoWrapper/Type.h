#pragma once

#include "NonOwningPtr.h"
#include <mono/metadata/object.h>
#include <string>
#include <vector>

namespace Mono {
    class MethodInfo;

    class FieldInfo;

    class PropertyInfo;

    class Object;

    class Type {
    public:
        Type() = default;

        explicit Type(non_owning_ptr<MonoClass> type);

        explicit Type(non_owning_ptr<MonoType> type);

        /**
         * @brief Get raw MonoClass pointer.
         * @return Raw MonoClass pointer.
         */
        non_owning_ptr<MonoClass> get() const;

        /**
         * @brief Get type name.
         * @return Type name.
         */
        std::string getName() const;

        /**
         * @brief Get namespace where type is located.
         * @return Namespace where type is located.
         */
        std::string getNamespace() const;

        /**
         * @brief Get full name of type. (e.g. "MyNamespace.MyClass")
         * @return Full name of type.
         */
        std::string getFullName() const;

        /**
         * @brief Get a specific custom attribute with specified type.
         * @param attributeType Type of attribute to get.
         * @return Custom attribute object.
         * @throws MissingAttributeException If attribute not found.
         */
        Object getCustomAttribute(const Type &attributeType);

        /**
         * @brief Check if this type has a specific custom attribute with specified type.
         * @param attributeType Type of attribute to check.
         * @return True if this type has a specific custom attribute with specified type, false otherwise.
         */
        bool hasCustomAttribute(const Type &attributeType);

        /**
         * @brief Get base type of this type.
         * @return Base type of this type.
         * @throws MonoException If does not have base type.
         */
        Type getBaseType();

        /**
         * @brief Check if this type is subclass of specified type. (e.g. "MyClass" is subclass of "MyBaseClass")
         * @param type Type to check.
         * @return True if this type is subclass of specified type, false otherwise.
         * @note This method will check recursively.
         */
        bool isSubclassOf(const Type &type);

        /**
         * @brief Get constructor with specified argument count.
         * @param argc Argument count of constructor.
         * @return Constructor method info.
         * @throws MissingMethodException If constructor not found.
         */
        MethodInfo getConstructor(int argc);

        /**
         * @brief Check if this type has constructor with specified argument count.
         * @param argc Argument count of constructor.
         * @return True if this type has constructor with specified argument count, false otherwise.
         */
        bool hasConstructor(int argc);

        /**
         * @brief Get method with specified name and argument count. (e.g. "MyMethod" with 2 arguments)
         * @param name Name of method.
         * @param argc Argument count of method.
         * @return Method info.
         * @throws MissingMethodException If method not found.
         */
        MethodInfo getMethod(const std::string &name, int argc);

        /**
         * @brief Check if this type has method with specified name and argument count. (e.g. "MyMethod" with 2 arguments)
         * @param name Name of method.
         * @param argc Argument count of method.
         * @return True if this type has method with specified name and argument count, false otherwise.
         */
        bool hasMethod(const std::string &name, int argc);

        /**
         * @brief Get method with specified name and argument types.
         * @param nameWithArgs Name of method with argument types. (e.g. "MyMethod(string)")
         * @return Method info.
         * @throws MissingMethodException If method not found.
         */
        MethodInfo getMethod(const std::string &nameWithArgs);

        /**
         * @brief Check if this type has method with specified name and argument types.
         * @param nameWithArgs Name of method with argument types. (e.g. "MyMethod(string)")
         * @return True if this type has method with specified name and argument types, false otherwise.
         */
        bool hasMethod(const std::string &nameWithArgs);

        /**
         * @brief Get all methods in this type.
         * @return List of methods in this type.
         */
        std::vector<MethodInfo> getMethods();

        /**
         * @brief Get field with specified name.
         * @param name Name of field.
         * @return Field info.
         * @throws MissingTypeMemberException If field not found.
         */
        FieldInfo getField(const std::string &name);

        /**
         * @brief Check if this type has field with specified name.
         * @param name Name of field.
         * @return True if this type has field with specified name, false otherwise.
         */
        bool hasField(const std::string &name);

        /**
         * @brief Get all fields in this type.
         * @return List of fields in this type.
         */
        std::vector<FieldInfo> getFields();

        /**
         * @brief Get property with specified name.
         * @param name Name of property.
         * @return Property info.
         * @throws MissingTypeMemberException If property not found.
         */
        PropertyInfo getProperty(const std::string &name);

        /**
         * @brief Check if this type has property with specified name.
         * @param name Name of property.
         * @return True if this type has property with specified name, false otherwise.
         */
        bool hasProperty(const std::string &name);

        /**
         * @brief Get all properties in this type.
         * @return List of properties in this type.
         */
        std::vector<PropertyInfo> getProperties();

        /**
         * @brief Check if this type is value type.
         * @return True if this type is value type, false otherwise.
         */
        bool isValueType() const;

        bool operator==(const Type &other) const;

        bool operator!=(const Type &other) const;

    protected:
        non_owning_ptr<MonoClass> _type = nullptr;
        std::string _name;
        std::string _namespace;
        std::string _fullName;
        bool _isValueType;
    };
}