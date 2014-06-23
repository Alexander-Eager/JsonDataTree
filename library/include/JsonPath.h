#ifndef JSON_PATH_H
#define JSON_PATH_H

// for the library
#include <JsonDataTree/JsonForwards.h>

// superclass
#include <QObject>

// for implicit sharing
#include <QSharedDataPointer>

// for list stuff
#include <QString>
#include <QStringList>
#include <QList>
#include <initializer_list>

namespace JSON
{
    // internal data
    class JsonPathPrivate;
    class JsonKeyPrivate;

    /**
     * \brief Represents a path from a root value
     *          to a child value.
     *
     * In JSON files, there are two container types:
     *  - Arrays, based on numbered indices `> 0`
     *  - Objects, which are basically maps with strings
     *      as keys.
     *
     * A path is a list of indices/keys to delve down a
     * JSON structure.
     */
    class JSON_LIBRARY JsonPath : public QList<JsonKey>
    {
        public:
            /**
             * \brief Construct an empty path.
             *
             * Following this path takes you nowhere.
             */
            JsonPath();

            /**
             * \brief Construct a path from the given list.
             *
             * This is useful for predefined paths in
             * initializer lists. You can pass a list of
             * integers and strings, and it will work.
             *
             * This constructor works on mixed string/int lists.
             *
             * \param[in] keys An initializer list of keys.
             */
            JsonPath(std::initializer_list<JsonKey> keys);

            /**
             * \brief Construct a path from a formatted string.
             *
             * The string must be separated by some specific
             * character. By default, that character is `'.'`.
             *
             * \param[in] keys The formatted string of keys.
             * \param[in] sep The separating character.
             */
            JsonPath(const char* const keys, char sep = '.');

            /**
             * \brief Construct a path from a formatted string.
             *
             * The string must be separated by some specific
             * character. By default, that character is `'.'`.
             *
             * \param[in] keys The formatted string of keys.
             * \param[in] sep The separating character.
             */
            JsonPath(QString keys, QChar sep = '.');

            /**
             * \brief Construct a path from a list of keys.
             *
             * \param[in] keys A list of string keys.
             */
            JsonPath(QStringList keys);

            /**
             * \brief Construct a path from a list of keys.
             *
             * \param[in] path A list of keys.
             */
            JsonPath(QList<JsonKey> other);

            /**
             * \brief Make a copy of `other`.
             *
             * \param[in] other The path to copy.
             */
            JsonPath(const JsonPath& other);

            /**
             * \brief Destroy this path.
             */
            ~JsonPath();

            /**
             * \brief Assign the contents of `other`
             *          to this path.
             *
             * \param[in] other The path to copy.
             */
            auto operator= (const JsonPath& other) -> JsonPath&;

            /**
             * \brief Compare two paths for equality.
             *
             * \param other The path to compare to.
             *
             * \returns `true` if both paths are equal,
             *          `false` otherwise.
             */
            auto operator== (const JsonPath& other) const -> bool;

            /**
             * \brief Compare two paths for inequality.
             *
             * \param other The path to compare to.
             *
             * \returns `true` if the paths are not equal,
             *          `false` otherwise.
             */
            auto operator!= (const JsonPath& other) const -> bool;

            /**
             * \brief Determine if this starts with an
             *          object key.
             *
             * Note that object keys and array indices are
             * not mutually exclusive.
             *
             * \returns `true` if this starts with an
             *          object key, `false` otherwise.
             */
            auto startsWithObject() const -> bool;

            /**
             * \brief Determine if this starts with an
             *          array index.
             *
             * Note that object keys and array indices are
             * not mutually exclusive.
             *
             * \returns `true` if this starts with an
             *          array index, `false` otherwise.
             */
            auto startsWithArray() const -> bool;

            /**
             * \brief Get the number of keys in this path.
             *
             * \returns The length of this path.
             */
            auto length() const -> int;

            /**
             * \brief Trim the path to include only the
             *        keys between `first` (inclusive)
             *        and `last` (exclusive).
             *
             * \param[in] first The first key to include in
             *                  the new path.
             * \param[in] last The index after the last key
             *                 to include in the new path.
             *
             * \returns The trimmed path.
             */
            auto trimmed(int first, int last) const -> JsonPath;

        private:
            QSharedDataPointer<JsonPathPrivate> d;
    };

    /**
     * \brief Class that represents a JSON key.
     *
     * This can be an integer or a string. It is
     * used to access an value in a `JsonArray`
     * or `JsonValue`.
     */
    class JSON_LIBRARY JsonKey
    {
        public:
            /**
             * \brief An invalid key.
             *
             * This represents the array index `-1`.
             */
            static const JsonKey Null;

            /**
             * \brief Construct an invalid key.
             *
             * An invalid key is the array index `-1`.
             */
            JsonKey();

            /**
             * \brief Construct a key from a string.
             *
             * All string keys can be object keys.
             * If the string represents an integer,
             * it can also be an array index.
             *
             * \param[in] key The string key.
             */
            JsonKey(QString key);

            /**
             * \brief Construct a key from a string.
             *
             * All string keys can be object keys.
             * If the string represents an integer,
             * it can also be an array index.
             *
             * \param[in] key The string key.
             */
            JsonKey(const char* const key);

            /**
             * \brief Construct a key from an integer.
             *
             * Integer keys can be array indices,
             * but they cannot be object keys.
             *
             * \param[in] key The array index.
             */
            JsonKey(int key);

            /**
             * \brief Make a copy of `other`.
             *
             * \param[in] other The key to copy.
             */
            JsonKey(const JsonKey& other);

            /**
             * \brief Destroy this object.
             */
            ~JsonKey();

            /**
             * \brief Assign the value of `other` to
             *          this key.
             *
             * \param[in] other The key to copy.
             */
            auto operator= (const JsonKey& other) -> JsonKey&;

            /**
             * \brief Determine if two keys are equal.
             *
             * \param other The key to compare to.
             *
             * \returns `true` if both this key and
             *          `other` are stored in the
             *          same way and have the same
             *          value, `false` otherwise.
             */
            auto operator== (const JsonKey& other) const -> bool;

            /**
             * \brief Compare to keys for inequality.
             *
             * \param other The key to compare to.
             *
             * \returns `true` if the keys are not equal,
             *          `false` otherwise.
             */
            auto operator!= (const JsonKey& other) const -> bool;

            /**
             * \brief Determine if this key can be
             *          used as an object key.
             *
             * All string keys can be used as object
             * keys, while no integer keys can be
             * used as object keys, so this is
             * essentially the same as `isString()`.
             *
             * \returns `true` if this can be used as
             *          an object key, `false` otherwise.
             */
            auto isObjectKey() const -> bool;

            /**
             * \brief Convert this key to an object key.
             *
             * If `!isObjectKey()`, this returns `QString()`.
             *
             * \param[out] ok Flag set to `isObjectKey()`.
             *
             * \returns This key as an object key.
             */
            auto toObjectKey(bool* ok = nullptr) const -> QString;

            /**
             * \brief Determine if this key can be
             *          used as an array index.
             *
             * All integer keys can be used as array
             * indices, while only some strings can.
             *
             * \returns `true` if this can be used as
             *          an array index, `false` otherwise.
             */
            auto isArrayIndex() const -> bool;

            /**
             * \brief Convert this key to an array index.
             *
             * If `!isArrayIndex()`, this returns `-1`.
             *
             * \param[out] ok Flag set to `isArrayIndex()`.
             *
             * \returns This key as an array index.
             */
            auto toArrayIndex(bool* ok = nullptr) const -> int;

            /**
             * \brief Determine if this key is stored
             *          as a string.
             *
             * \returns `true` if this key is stored as
             *          a string, `false` otherwise.
             */
            auto isString() const -> bool;

            /**
             * \brief Get this key as a string.
             *
             * If `!isString()`, this returns `QString()`.
             *
             * \param[out] ok Flag set to `isString()`.
             *
             * \returns This key as a string.
             */
            auto toString(bool* ok = nullptr) const -> QString;

            /**
             * \brief Determine if this key is stored
             *          as an integer.
             *
             * \returns `true` if this key is stored as
             *          an integer, `false` otherwise.
             */
            auto isInteger() const -> bool;

            /**
             * \brief Convert this key to an integer.
             *
             * If `!isInteger()`, this returns `-1`.
             *
             * \param[out] ok Flag set to `isInteger()`.
             *
             * \returns This key as an integer.
             */
            auto toInteger(bool* ok = nullptr) const -> int;

        private:
            QSharedDataPointer<JsonKeyPrivate> d;
    };
}

/**
 * \brief Append a key to the given path.
 *
 * \param path The path to append to.
 * \param key The key to append.
 *
 * \returns The parameter `path`.
 */
JSON_LIBRARY auto operator<< (JSON::JsonPath& path,
                              JSON::JsonKey key) -> JSON::JsonPath&;

/**
 * \brief Append another path to the given path.
 *
 * \param path The path to append to.
 * \param other The path to append.
 *
 * \returns The parameter `path`.
 */
JSON_LIBRARY auto operator<< (JSON::JsonPath& path,
                              JSON::JsonPath other) -> JSON::JsonPath&;

#endif // JSON_PATH_H
