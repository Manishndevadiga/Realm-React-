////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "js_types.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <optional>

#include <realm/object-store/object_schema.hpp>
#include <realm/object-store/shared_realm.hpp>

namespace realm {
namespace js {

enum class AggregateFunc { Min, Max, Sum, Avg };

static std::vector<std::string> const JSLogFunctionName(
    // do not reorder unless reordering JSLogFunction as well
    {
        "log",
        "debug",
        "info",
        "error",
        "warn",
        "trace",
    });

enum JSLogFunction {
    // do not reorder unless reordering JSLogFunctionName as well
    Log = 0,
    Debug,
    Info,
    Error,
    Warning,
    Trace
};

template <typename T>
class RealmDelegate;

template <typename T>
static inline RealmDelegate<T>* get_delegate(realm::Realm* realm)
{
    return static_cast<RealmDelegate<T>*>(realm->m_binding_context.get());
}

static const char* local_string_for_property_type(realm::PropertyType type)
{
    switch (type & ~realm::PropertyType::Flags) {
        // Override naming given by ObjectStore
        case realm::PropertyType::ObjectId:
            return "objectId";
        case realm::PropertyType::Decimal:
            return "decimal128";

        default:
            return string_for_property_type(type);
    }
}

template <typename T>
static inline T stot(const std::string& s)
{
    std::istringstream iss(s);
    T value;
    iss >> value;
    if (iss.fail()) {
        throw std::invalid_argument("Cannot convert string '" + s + "'");
    }
    return value;
}

static inline uint32_t validated_positive_index(std::string string)
{
    int64_t index = stot<int64_t>(string);
    if (index < 0) {
        throw std::out_of_range(std::string("Index ") + string + " cannot be less than zero.");
    }
    if (index > std::numeric_limits<uint32_t>::max()) {
        throw std::out_of_range(std::string("Index ") + string + " must be a 32-bit unsigned integer");
    }
    return static_cast<uint32_t>(index);
}

static inline void validate_argument_count(size_t count, size_t expected, const char* message = nullptr)
{
    if (count != expected) {
        throw std::invalid_argument(message ? message : "Invalid arguments");
    }
}

static inline void validate_argument_count(size_t count, size_t min, size_t max, const char* message = nullptr)
{
    if (count < min || count > max) {
        throw std::invalid_argument(message ? message : "Invalid arguments");
    }
}

static inline void validate_argument_count_at_least(size_t count, size_t expected, const char* message = nullptr)
{
    if (count < expected) {
        throw std::invalid_argument(message ? message : "Invalid arguments");
    }
}

template <typename T, AggregateFunc func>
void compute_aggregate_on_collection(typename T::ContextType ctx, typename T::ObjectType this_object,
                                     typename T::Arguments& args, typename T::ReturnValue& return_value)
{

    auto list = get_internal<typename T::Type, T>(ctx, this_object);

    ColKey column = {};
    if (list->get_type() == realm::PropertyType::Object) {
        const ObjectSchema& object_schema = list->get_object_schema();
        std::string property_name = T::Value::validated_to_string(ctx, args[0]);
        const Property* property = object_schema.property_for_name(property_name);
        if (!property) {
            throw std::invalid_argument(
                util::format("Property '%1' does not exist on object '%2'", property_name, object_schema.name));
        }
        column = property->column_key;
    }
    else {
        args.validate_maximum(0);
    }

    std::optional<Mixed> mixed;
    switch (func) {
        case AggregateFunc::Min:
            return_value.set(list->min(column));
            break;
        case AggregateFunc::Max:
            return_value.set(list->max(column));
            break;
        case AggregateFunc::Sum:
            return_value.set(list->sum(column));
            break;
        case AggregateFunc::Avg:
            return_value.set(list->average(column));
            break;
    }
}

/**
 * @brief
 *
 * @param ctx
 * @param message
 * @param console_log_cmd
 */


/**
 * @brief Log a message to JS's `console`
 *
 * @tparam T JS Engine
 * @param ctx The current JS context
 * @param message Message to pass along to `console`'s logger
 * @param log_function_id Optional logging function to invoke on `console`, e.g., Info, Warning.  Default is Log
 */

template <typename T>
void log_to_console(typename T::Context ctx, std::string const& message, JSLogFunction log_function_id)
{
    using ObjectType = typename T::Object;
    using Object = js::Object<T>;
    using ValueType = typename T::Value;
    using Value = js::Value<T>;
    using FunctionType = typename T::Function;

    std::string const console_log_cmd = JSLogFunctionName[static_cast<int>(log_function_id)];
    ObjectType console = Value::validated_to_object(ctx, Object::get_global(ctx, "console"), "console");
    ValueType function_obj = Object::get_property(ctx, console, console_log_cmd);
    FunctionType log_function =
        Value::validated_to_function(ctx, function_obj, std::string("console." + console_log_cmd).c_str());
    ValueType msg[1] = {Value::from_string(ctx, message)};
    Function<T>::call(ctx, log_function, 1, msg);
}


} // namespace js
} // namespace realm
