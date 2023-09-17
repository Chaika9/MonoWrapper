#pragma once

#include "Type.h"
#include "NonOwningPtr.h"
#include <mono/metadata/assembly.h>
#include <string>
#include <vector>

namespace Mono {
    class Assembly {
    public:
        Assembly() = default;

        explicit Assembly(non_owning_ptr<MonoAssembly> assembly);

        /**
         * @brief Get raw MonoAssembly pointer.
         * @return Raw MonoAssembly pointer.
         */
        non_owning_ptr<MonoAssembly> get() const;

        /**
         * @brief Load specified assembly file into the current domain.
         * @param path Path to assembly file.
         * @return Assembly loaded.
         * @note If assembly is already loaded, it will return the loaded assembly.
         * @throws FileNotFoundException If assembly file not found.
         * @throws FileLoadException If assembly cannot be loaded. (e.g. invalid assembly file)
         */
        static Assembly loadFile(const std::string &path);

        /**
         * @brief Get specified type from this assembly.
         * @param name Name of type.
         * @return Type found.
         * @note You can use namespace qualified name. (e.g. "MyNamespace.MyClass")
         * @throws TypeLoadException If type not found.
         */
        Type getType(const std::string &name);

        /**
         * @brief Get specified type from this assembly.
         * @param nameSpace Namespace where type is located.
         * @param name Name of type.
         * @return Type found.
         * @throws TypeLoadException If type not found.
         */
        Type getType(const std::string &nameSpace, const std::string &name);

        /**
         * @brief Get all types in this assembly.
         * @return List of types in this assembly.
         */
        std::vector<Type> getTypes();

    protected:
        non_owning_ptr<MonoAssembly> _assembly = nullptr;
        non_owning_ptr<MonoImage> _image = nullptr;
    };
}