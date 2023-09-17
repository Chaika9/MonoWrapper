#include "MethodInfo.h"
#include "Type.h"
#include "Exceptions/MissingAttributeException.h"
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>
#include <stdexcept>

using namespace Mono;

MethodInfo::MethodInfo(const Type &type, non_owning_ptr<MonoMethod> method) : MemberInfo(type,
                                                                                         mono_method_get_name(method)),
                                                                              _method(method) {
    if (_method == nullptr) {
        throw std::invalid_argument("method cannot be null");
    }
}

non_owning_ptr<MonoMethod> MethodInfo::get() const {
    return _method;
}

bool MethodInfo::isStatic() const {
    uint32_t flags = mono_method_get_flags(_method, nullptr);
    return (flags & MONO_METHOD_ATTR_STATIC) != 0;
}

Object MethodInfo::getCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_method(_method);
    if (attributeInfo == nullptr) {
        throw MissingAttributeException(
                "Attribute '" + attributeType.getFullName() + "' not found for method '" + _name + "' in class '" +
                _declaringType.getFullName() + "'");
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return Object(attributeType, attributePtr);
}

bool MethodInfo::hasCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_method(_method);
    if (attributeInfo == nullptr) {
        return false;
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return attributePtr != nullptr;
}

bool MethodInfo::operator==(const MethodInfo &other) const {
    return _method == other._method;
}

bool MethodInfo::operator!=(const MethodInfo &other) const {
    return !(*this == other);
}