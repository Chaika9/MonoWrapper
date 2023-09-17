#include "PropertyInfo.h"
#include "Type.h"
#include "MethodInfo.h"
#include "Object.h"
#include "Exceptions/AccessViolationException.h"
#include "Exceptions/MissingAttributeException.h"
#include <mono/metadata/reflection.h>
#include <stdexcept>

using namespace Mono;

PropertyInfo::PropertyInfo(const Type &type, non_owning_ptr<MonoProperty> property) : MemberInfo(type,
                                                                                                 mono_property_get_name(
                                                                                                         property)),
                                                                                      _property(property) {
    if (property == nullptr) {
        throw std::invalid_argument("property cannot be null");
    }
}

non_owning_ptr<MonoProperty> PropertyInfo::get() const {
    return _property;
}

bool PropertyInfo::isStatic() const {
    return getGetMethod().isStatic();
}

Object PropertyInfo::getCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_property(_declaringType.get(), _property);
    if (attributeInfo == nullptr) {
        throw MissingAttributeException(
                "Attribute '" + attributeType.getFullName() + "' not found for property '" + _name + "' in class '" +
                _declaringType.getFullName() + "'");
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return Object(attributeType, attributePtr);
}

bool PropertyInfo::hasCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_property(_declaringType.get(), _property);
    if (attributeInfo == nullptr) {
        return false;
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return attributePtr != nullptr;
}

Type PropertyInfo::getType() const {
    return getGetMethod().getReturnType();
}

MethodInfo PropertyInfo::getGetMethod() const {
    auto methodPtr = mono_property_get_get_method(_property);
    return MethodInfo(_declaringType, methodPtr);
}

MethodInfo PropertyInfo::getSetMethod() const {
    auto methodPtr = mono_property_get_set_method(_property);
    if (methodPtr == nullptr) {
        throw AccessViolationException(
                "Property '" + _name + "' in class '" + _declaringType.getFullName() + "' is read-only");
    }
    return MethodInfo(_declaringType, methodPtr);
}