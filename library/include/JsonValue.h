#ifndef JSON_VALUE_H
#define JSON_VALUE_H

// for the library
#include <JsonDataTree/JsonForwards.h>

// superclass
#include <QObject>

// for implicit sharing
#include <QSharedDataPointer>

// for data
#include <QString>

namespace JSON
{
    // internal data class
    class JsonValuePrivate;

    /**
     * \brief Describes a JSON value, i.e.
     *            array, object, string,
     *            integer, floating point number,
     *            or null.
     */
    class JSON_LIBRARY JsonValue : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        Q_PROPERTY(Type type
                   READ getType
                   WRITE setType)
        Q_PROPERTY(bool isNumber
                   READ isNumber
                   STORED false)
        Q_PROPERTY(int integer
                   READ toInteger
                   WRITE setInteger)
        Q_PROPERTY(double floating
                   READ toDouble
                   WRITE setDouble)
        Q_PROPERTY(bool isString
                   READ isString
                   STORED false)
        Q_PROPERTY(QString string
                   READ toString
                   WRITE setString)
        Q_PROPERTY(bool isBoolean
                   READ isBoolean
                   STORED false)
        Q_PROPERTY(bool boolean
                   READ toBoolean
                   WRITE setBoolean)
        Q_PROPERTY(bool isArray
                   READ isArray
                   STORED false)
        Q_PROPERTY(JsonArray array
                   READ toArray
                   WRITE setArray)
        Q_PROPERTY(bool isObject
                   READ isObject
                   STORED false)
        Q_PROPERTY(JsonObject object
                   READ toObject
                   WRITE setObject)

        public:
            /** 
             * \brief An `enum` for the type of value.
             */
            enum Type { Array = 0,
                        Object,
                        String,
                        Number,
                        Boolean,
                        Null };

            /**
             * \brief Destroys this object.
             */
            ~JsonValue();

            /**
             * \brief Construct a invalid reference.
             *
             * \see isValid()
             */
            JsonValue();

            /**
             * \brief Make a copy of `other`.
             *
             * \param[in] other The value to copy.
             */
            JsonValue(const JsonValue& other);

            /**
             * \brief Assign the value of `other` to this object.
             *
             * \param[in] other The value to copy.
             */
            auto operator= (const JsonValue& other) -> JsonValue&;

            /**
             * \brief Construct a value with the given type.
             *
             * If the type is `Array` or `Object`, the default
             * value is empty. If it is `String`, it is an
             * empty string. If it is `Integer` or `Double`,
             * it is `0`.
             *
             * \param[in] type The type of value to make.
             */
            JsonValue(Type type);

            /**
             * \brief Reset the data this object holds to the
             *            default for the given type.
             *
             * \param[in] type The type of value to become.
             */
            auto setType(Type type) -> void;

            /**
             * \brief Make from an integer value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(int val);

            /**
             * \brief Assign an integer value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setInteger(int val) -> void;

            /**
             * \brief Make from a floating point (`double`) value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(double val);

            /**
             * \brief Assign a floating point (`double`) value to
             *            this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setDouble(double val) -> void;

            /**
             * \brief Make from a string value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(QString val);

            /**
             * \brief Assign a string value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setString(QString val) -> void;

            /**
             * \brief Make from a string value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(const char* const val);

            /**
             * \brief Assign a string value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setString(const char* const val) -> void;

            /**
             * \brief Make from a boolean value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(bool val);

            /**
             * \brief Assign a boolean value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setBoolean(bool val) -> void;

            /**
             * \brief Make from an array value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(JsonArray val);

            /**
             * \brief Assign an array value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setArray(JsonArray val) -> void;

            /**
             * \brief Make from an object value.
             *
             * \param[in] val The value to assign to the object.
             */
            JsonValue(JsonObject val);

            /**
             * \brief Assign an object value to this `JsonValue`.
             *
             * \param[in] val The value to assign to this object.
             */
            auto setObject(JsonObject val) -> void;

            /**
             * \brief Get the type of value this object holds.
             *
             * \returns The type of value this object holds.
             */
            auto getType() const -> Type;

            /**
             * \brief Determine if this is a null value.
             *
             * \returns `true` if this is a null value,
             *            `false` otherwise.
             */
            auto isNull() const -> bool;

            /**
             * \brief Determine if this is a numeric value.
             *
             * \see toInteger(bool*)
             * \see toDouble(bool*)
             *
             * \returns `true` if this is a numeric value,
             *            `false` otherwise.
             */
            auto isNumber() const -> bool;

            /**
             * \brief Convert this value to an integer.
             *
             * If `isNumber()`, this casts the value
             * to an `int` and returns that value. This
             * also sets `*ok` to `true`.
             *
             * If this is not a numeric value, this
             * returns `0` and sets `*ok` to `false`.
             *
             * \see isNumber()
             * \see toDouble(bool*)
             *
             * \param[out] ok A flag set to `true` if `isNumber()`,
             *                `false` otherwise.
             *
             * \returns This value as an integer.
             */
            auto toInteger(bool* ok = nullptr) const -> int;

            /**
             * \brief Convert this value to a `double`.
             *
             * If `isNumber()`, this simply returns the
             * proper value and sets `*ok` to `true`.
             *
             * If this is not a numeric value, this
             * returns `0` and sets `*ok` to `false`.
             *
             * \see isNumber()
             * \see toInteger(bool*)
             *
             * \param[out] ok A flag set to `true` if `isNumber()`,
             *                `false` otherwise.
             *
             * \returns This value as a `double`.
             */
            auto toDouble(bool* ok = nullptr) const -> double;

            /**
             * \brief Determine if this is a string value.
             *
             * \see toString(bool*)
             *
             * \returns `true` if this is a string value,
             *            `false` otherwise.
             */
            auto isString() const -> bool;

            /**
             * \brief Convert this value to a string.
             *
             * If `isString()`, this simply returns the
             * proper value.
             *
             * If this is not a string value, this
             * returns `QString()` and sets `*ok` to `false`.
             *
             * \see isString()
             *
             * \param[out] ok A flag set to `true` if `isString()`,
             *                `false` otherwise.
             *
             * \returns This value as a string.
             */
            auto toString(bool* ok = nullptr) const -> QString;

            /**
             * \brief Determine if this is a boolean value.
             *
             * \see toBoolean(bool*)
             *
             * \returns `true` if this is a boolean value,
             *            `false` otherwise.
             */
            auto isBoolean() const -> bool;

            /**
             * \brief Convert this value to a boolean.
             *
             * If `isBoolean()`, this simply returns the
             * proper value.
             *
             * If this is anything else, this returns `false`
             * and sets `*ok` to `false`.
             *
             * \see isBoolean()
             *
             * \param[out] ok A flag set to `true` if `isBoolean()`,
             *                `false` otherwise.
             *
             * \returns This value as a boolean.
             */
            auto toBoolean(bool* ok = nullptr) const -> bool;

            /**
             * \brief Determine if this value is an array.
             *
             * \see toArray(bool*)
             *
             * \returns `true` if this value is an array,
             *            `false` otherwise.
             */
            auto isArray() const -> bool;

            /**
             * \brief Convert this value to an array.
             *
             * If this value is not an array, this returns
             * an empty `JsonArray` and sets `*ok` to
             * `false`.
             *
             * \see isArray().
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an array, `false` otherwise.
             *
             * \returns This value as an array.
             */
            auto toArray(bool* ok = nullptr) -> JsonArray&;

            /**
             * \brief Convert this value to an array.
             *
             * If this value is not an array, this returns
             * an empty `JsonArray` and sets `*ok` to
             * `false`.
             *
             * \see isArray().
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an array, `false` otherwise.
             *
             * \returns This value as an array.
             */
            auto toArray(bool* ok = nullptr) const -> JsonArray;

            /**
             * \brief Convert this value to an array.
             *
             * If this value is not an array, this returns
             * an empty `JsonArray` and sets `*ok` to
             * `false`.
             *
             * \see isArray().
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an array, `false` otherwise.
             *
             * \returns This value as an array.
             */
            auto constToArray(bool* ok = nullptr) const -> JsonArray;

            /**
             * \brief Determine if this value is an object.
             *
             * \see toObject(bool*)
             *
             * \returns `true` if this value is an object,
             *            `false` otherwise.
             */
            auto isObject() const -> bool;

            /**
             * \brief Convert this value to an object.
             *
             * If this value is not an object, this returns
             * an empty `JsonObject` and sets `*ok` to `false`.
             *
             * \see isObject()
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an object, `false` otherwise.
             *
             * \returns This value as an object.
             */
            auto toObject(bool* ok = nullptr) -> JsonObject&;

            /**
             * \brief Convert this value to an object.
             *
             * If this value is not an object, this returns
             * an empty `JsonObject` and sets `*ok` to `false`.
             *
             * \see isObject()
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an object, `false` otherwise.
             *
             * \returns This value as an object.
             */
            auto toObject(bool* ok = nullptr) const -> JsonObject;

            /**
             * \brief Convert this value to an object.
             *
             * If this value is not an object, this returns
             * an empty `JsonObject` and sets `*ok` to `false`.
             *
             * \see isObject()
             *
             * \param[out] ok A flag set to `true` if this value is
             *                an object, `false` otherwise.
             *
             * \returns This value as an object.
             */
            auto constToObject(bool* ok = nullptr) const -> JsonObject;

            /**
             * \brief Get the value at the end of a path.
             *
             * If the path is not valid for this value, a `Null`
             * value is returned.
             *
             * \param[in] path The path to follow to the desired value.
             * \param[out] ok A flag set to `true` if `path` is valid
             *                    for this value, `false` otherwise.
             *
             * \returns The value at the end of `path`.
             */
            auto follow(JsonPath path, bool* ok = nullptr) -> JsonValue&;

            /**
             * \brief Get the value at the end of a path.
             *
             * If the path is not valid for this value, a `Null`
             * value is returned.
             *
             * \param[in] path The path to follow to the desired value.
             * \param[out] ok A flag set to `true` if `path` is valid
             *                    for this value, `false` otherwise.
             *
             * \returns The value at the end of `path`.
             */
            auto follow(JsonPath path, bool* ok = nullptr) const -> JsonValue;

            /**
             * \brief Get the value at the end of a path.
             *
             * If the path is not valid for this value, a `Null`
             * value is returned.
             *
             * \param[in] path The path to follow to the desired value.
             * \param[out] ok A flag set to `true` if `path` is valid
             *                    for this value, `false` otherwise.
             *
             * \returns The value at the end of `path`.
             */
            auto constFollow(JsonPath path, bool* ok = nullptr) const -> JsonValue;

            /**
             * \brief Create the given path for this value.
             *
             * This creates all necessary children to make the
             * path viable, and returns a reference to the value
             * at the end of the path (which is initialized to `Null`,
             * if it did not already exist).
             *
             * Adding to a child object involves adding a key-value pair
             * to the object. Adding to an array involves adding to the
             * beggining or end, depending on the value of the array index
             * in the path. This creates an object if there is a choice
             * between an object key and an array index.
             *
             * `Null` values are treated in a special way: unlike `Number`,
             * `String`, `Boolean`, `Object`, and `Array`, `Null` values
             * are converted into the appropriate type. So if you have the
             * JSON object:
             *
             *        {
             *            "A": null
             *        }
             *
             * And you ask to create the path `{ "A", 0 }`, it will change
             * the tree to:
             *
             *        {
             *            "A": [
             *                null
             *            ]
             *        }
             * 
             * If this value (or one of the requested children)
             * is not the right type (e.g., the path starts
             * with an array index but this is an object, or this value
             * is an integer and thus has no children at all), the path is
             * not created and a `Null` value is returned.
             *
             * \param[in] path The path to create.
             * \param[out] ok A flag set to `true` if `path` is valid
             *                    for this value, `false` otherwise.
             *
             * \returns The value at the end of the newly created path.
             */
            auto create(JsonPath path, bool* ok = nullptr) -> JsonValue&;

        private:
            /** \brief The *d-pointer* for this object. */
            QSharedDataPointer<JsonValuePrivate> d;
    };
}

Q_DECLARE_METATYPE(JSON::JsonValue)

#endif // JSON_VALUE_H
