#include "FieldInfo.h"
#include "Type.h"
#include "Object.h"
#include "Exceptions/MissingAttributeException.h"
#include <mono/metadata/reflection.h>
#include <stdexcept>

using namespace Mono;

FieldInfo::FieldInfo(const Type &type, non_owning_ptr<MonoClassField> field) : MemberInfo(type,
                                                                                          mono_field_get_name(field)),
                                                                               _field(field) {
    if (field == nullptr) {
        throw std::invalid_argument("field cannot be null");
    }
    _type = Type(mono_field_get_type(field));
}

non_owning_ptr<MonoClassField> FieldInfo::get() const {
    return _field;
}

Object FieldInfo::getCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_field(_declaringType.get(), _field);
    if (attributeInfo == nullptr) {
        throw MissingAttributeException(
                "Attribute '" + attributeType.getFullName() + "' not found for field '" + _name + "' in class '" +
                _declaringType.getFullName() + "'");
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return Object(attributeType, attributePtr);
}

bool FieldInfo::hasCustomAttribute(const Type &attributeType) {
    auto attributeInfo = mono_custom_attrs_from_field(_declaringType.get(), _field);
    if (attributeInfo == nullptr) {
        return false;
    }

    auto attributePtr = mono_custom_attrs_get_attr(attributeInfo, attributeType.get());
    return attributePtr != nullptr;
}

Type FieldInfo::getType() const {
    return _type;
}