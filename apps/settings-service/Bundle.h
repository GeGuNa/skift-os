#pragma once

#include <libio/File.h>
#include <libutils/json/Json.h>
#include <libsettings/Path.h>

namespace Settings
{

struct Bundle
{
    HashMap<String, Json::Value> keys;

    static Optional<Bundle> Load(const String &path)
    {
        Bundle bundle;

        IO::File file{path, OPEN_READ};

        if (!file.exist())
        {
            return {};
        }

        auto value = Json::parse(file);

        if (!value.is(Json::OBJECT))
        {
            return {};
        }

        auto obj = value.as_object();

        obj.foreach ([&](auto &key, auto &value) {
            bundle.keys[key] = value;
            return Iteration::CONTINUE;
        });

        return bundle;
    }

    void write(const Path &path, const Json::Value &value)
    {
        keys[path.key] = value;
    }

    Json::Value read(const Path &path)
    {
        if (path.key == "*")
        {
            Json::Value::Object obj;

            keys.foreach ([&](auto &key, auto &value) {
                obj[key] = value;

                return Iteration::CONTINUE;
            });

            return obj;
        }
        else if (keys.has_key(path.key))
        {
            return keys[path.key];
        }
        else
        {
            return nullptr;
        }
    }
};

} // namespace Settings
