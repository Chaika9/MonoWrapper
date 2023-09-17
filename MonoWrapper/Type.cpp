#include "Type.h"
#include "FieldInfo.h"
#include "PropertyInfo.h"
#include "MethodInfo.h"
#include "Exceptions/MissingTypeMemberException.h"
#include "Exceptions/MissingMethodException.h"
#include "Exceptions/MissingAttributeException.h"
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <stdexcept>

using namespace Mono;

Type::Type(non_owning_ptr<MonoClass> type) : _type(type) {
    if (_type == nullptr) {
        throw std::invalid_argument("type cannot be null");
    }

    _name = mono_class_get_name(_type);
    _namespace = mono_class_get_namespace(_type);
    _fullName = _namespace + "." + _name;
    _isValueType = mono_class_is_valuetype(_type);
}

Type::Type(non_owning_ptr<MonoType> type) {
    if (type == nullptr) {
        throw std::invalid_argument("type cannot be null");
    }

    _type = mono_class_from_mono_type(type);
    if (_type == nullptr) {
        throw std::runtime_error("Failed to get MonoClass from MonoType");
    }

    _name = mono_class_get_name(_type);
    _namespace = mono_class_get_namespace(_type);
    _fullName = _namespace + "." + _name;
    _isValueType = mono_class_is_valuetype(_type);
}

non_owning_ptr<MonoClass> Type::get() const {
    return _type;
}

std::string Type::getName() const {
    return _name;
}

std::string Type::getNamespace() const {
    return _namespace;
}

std::string Type::getFullName() const {
    return _fullName;
}

Object Type::getCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_class(_type);
    if (attributeInfo == nullptr) {
        throw MissingAttributeException(
                "Attribute '" + attributeType.getFullName() + "' not found in class '" + _fullName + "'");
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return Object(attributeType, attributePtr);
}

bool Type::hasCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_class(_type);
    if (attributeInfo == nullptr) {
        return false;
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return attributePtr != nullptr;
}

Type Type::getBaseType() {
    auto baseType = mono_class_get_parent(_type);
    if (baseType == nullptr) {
        throw MonoException("Base type not found");
    }
    return Type(baseType);
}

bool Type::isSubclassOf(const Type &type) {
    return mono_class_is_subclass_of(_type, type._type, true);
}

MethodInfo Type::getConstructor(int argc) {
    if (argc < 0) {
        throw std::runtime_error("argc cannot be negative");
    }

    auto constructorPtr = mono_class_get_method_from_name(_type, ".ctor", argc);
    if (constructorPtr == nullptr) {
        throw MissingMethodException(
                "Constructor with " + std::to_string(argc) + " arguments not found in class '" + _fullName + "'");
    }
    return MethodInfo(*this, constructorPtr);
}

bool Type::hasConstructor(int argc) {
    if (argc < 0) {
        throw std::runtime_error("argc cannot be negative");
    }

    auto constructorPtr = mono_class_get_method_from_name(_type, ".ctor", argc);
    return constructorPtr != nullptr;
}

MethodInfo Type::getMethod(const std::string &name, int argc) {
    if (argc < 0) {
        throw std::runtime_error("argc cannot be negative");
    }

    auto methodPtr = mono_class_get_method_from_name(_type, name.c_str(), argc);
    if (methodPtr == nullptr) {
        throw MissingMethodException("Method '" + name + "' not found in class '" + _fullName + "'");
    }
    return MethodInfo(*this, methodPtr);
}

bool Type::hasMethod(const std::string &name, int argc) {
    if (argc < 0) {
        throw std::runtime_error("argc cannot be negative");
    }

    auto methodPtr = mono_class_get_method_from_name(_type, name.c_str(), argc);
    return methodPtr != nullptr;
}

MethodInfo Type::getMethod(const std::string &nameWithArgs) {
    auto desc = mono_method_desc_new((":" + nameWithArgs).c_str(), 0);
    auto methodPtr = mono_method_desc_search_in_class(desc, _type);
    mono_method_desc_free(desc);

    if (methodPtr == nullptr) {
        throw MissingMethodException("Method '" + nameWithArgs + "' not found in class '" + _fullName + "'");
    }
    return MethodInfo(*this, methodPtr);
}

bool Type::hasMethod(const std::string &nameWithArgs) {
    auto desc = mono_method_desc_new((":" + nameWithArgs).c_str(), 0);
    auto methodPtr = mono_method_desc_search_in_class(desc, _type);
    mono_method_desc_free(desc);

    return methodPtr != nullptr;
}

std::vector<MethodInfo> Type::getMethods() {
    std::vector<MethodInfo> methods;

    void *iter = nullptr;
    auto iterMethod = mono_class_get_methods(_type, &iter);
    while (iterMethod != nullptr) {
        auto method = MethodInfo(*this, iterMethod);
        methods.push_back(method);

        iterMethod = mono_class_get_methods(_type, &iter);
    }
    return methods;
}

FieldInfo Type::getField(const std::string &name) {
    auto fieldPtr = mono_class_get_field_from_name(_type, name.c_str());
    if (fieldPtr == nullptr) {
        throw MissingTypeMemberException("Field '" + name + "' not found in class '" + _fullName + "'");
    }
    return FieldInfo(*this, fieldPtr);
}

bool Type::hasField(const std::string &name) {
    auto fieldPtr = mono_class_get_field_from_name(_type, name.c_str());
    return fieldPtr != nullptr;
}

std::vector<FieldInfo> Type::getFields() {
    std::vector<FieldInfo> fields;

    void *iter = nullptr;
    auto iterField = mono_class_get_fields(_type, &iter);
    while (iterField != nullptr) {
        auto field = FieldInfo(*this, iterField);
        fields.push_back(field);

        iterField = mono_class_get_fields(_type, &iter);
    }
    return fields;
}

PropertyInfo Type::getProperty(const std::string &name) {
    auto propertyPtr = mono_class_get_property_from_name(_type, name.c_str());
    if (propertyPtr == nullptr) {
        throw MissingTypeMemberException("Property '" + name + "' not found in class '" + _fullName + "'");
    }
    return PropertyInfo(*this, propertyPtr);
}

bool Type::hasProperty(const std::string &name) {
    auto propertyPtr = mono_class_get_property_from_name(_type, name.c_str());
    return propertyPtr != nullptr;
}

std::vector<PropertyInfo> Type::getProperties() {
    std::vector<PropertyInfo> properties;

    void *iter = nullptr;
    auto iterProperty = mono_class_get_properties(_type, &iter);
    while (iterProperty != nullptr) {
        auto property = PropertyInfo(*this, iterProperty);
        properties.push_back(property);

        iterProperty = mono_class_get_properties(_type, &iter);
    }
    return properties;
}

bool Type::isValueType() const {
    return _isValueType;
}

bool Type::operator==(const Type &other) const {
    return _type == other._type;
}

bool Type::operator!=(const Type &other) const {
    return !(*this == other);
}