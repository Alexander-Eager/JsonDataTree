#ifndef JSON_PATH_H
#define JSON_PATH_H

// for the library
#include "JsonForwards.h"
#include "LibraryMacros.h"

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
	 *			to a child value.
	 *
	 * In JSON files, there are two container types:
	 *	- Arrays, based on numbered indices `> 0`
	 *	- Objects, which are basically maps with strings
	 *		as keys.
	 *
	 * A path is a list of indices/keys to delve down a
	 * JSON structure.
	 **/
	class JSON_LIBRARY JsonPath : public QObject
	{
		Q_OBJECT

		Q_PROPERTY(bool startsWithObject
					READ startsWithObject
					STORED false)
		Q_PROPERTY(bool startsWithArray
					READ startsWithArray
					STORED false)
		Q_PROPERTY(int length
					READ length
					STORED false)

		public:
			/**
			 * \brief The iterator type.
			 **/
			using iterator = QList<JsonKey>::iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using const_iterator = QList<JsonKey>::const_iterator;

			/**
			 * \brief For STL compatibility.
			 **/
			using const_pointer = QList<JsonKey>::const_pointer;

			/**
			 * \brief For STL compatibility.
			 **/
			using const_reference = QList<JsonKey>::const_reference;

			/**
			 * \brief For STL compatibility.
			 **/
			using difference_type = QList<JsonKey>::difference_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using pointer = QList<JsonKey>::pointer;

			/**
			 * \brief For STL compatibility.
			 **/
			using reference = QList<JsonKey>::reference;

			/**
			 * \brief For STL compatibility.
			 **/
			using size_type = QList<JsonKey>::size_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using value_type = QList<JsonKey>::value_type;

			/**
			 * \brief The iterator type.
			 **/
			using Iterator = QList<JsonKey>::Iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using ConstIterator = QList<JsonKey>::ConstIterator;

			/**
			 * \brief Construct an empty path.
			 *
			 * Following this path takes you nowhere.
			 **/
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
			 **/
			JsonPath(std::initializer_list<JsonKey> keys);

			/**
			 * \brief Construct a path from a formatted string.
			 *
			 * The string must be separated by some specific
			 * character. By default, that character is `'.'`.
			 *
			 * \param[in] keys The formatted string of keys.
			 * \param[in] sep The separating character.
			 **/
			JsonPath(const char* const keys, char sep = '.');

			/**
			 * \brief Construct a path from a formatted string.
			 *
			 * The string must be separated by some specific
			 * character. By default, that character is `'.'`.
			 *
			 * \param[in] keys The formatted string of keys.
			 * \param[in] sep The separating character.
			 **/
			JsonPath(QString keys, QChar sep = '.');

			/**
			 * \brief Construct a path from a list of keys.
			 *
			 * \param[in] keys A list of string keys.
			 **/
			JsonPath(QStringList keys);

			/**
			 * \brief Destroy this path.
			 **/
			virtual ~JsonPath();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The path to copy.
			 **/
			JsonPath(const JsonPath& other);

			/**
			 * \brief Assign the contents of `other`
			 *			to this path.
			 *
			 * \param[in] other The path to copy.
			 **/
			JsonPath& operator= (const JsonPath& other);

			/**
			 * \brief Compare two paths for equality.
			 *
			 * \param other The path to compare to.
			 *
			 * \returns `true` if both paths are equal,
			 *			`false` otherwise.
			 **/
			bool operator== (const JsonPath& other) const;

			/**
			 * \brief Compare two paths for inequality.
			 *
			 * \param other The path to compare to.
			 *
			 * \returns `true` if the paths are not equal,
			 *			`false` otherwise.
			 **/
			bool operator!= (const JsonPath& other) const;

			/**
			 * \brief Determine if this starts with an
			 *			object key.
			 *
			 * Note that object keys and array indices are
			 * not mutually exclusive.
			 *
			 * \returns `true` if this starts with an
			 *			object key, `false` otherwise.
			 **/
			virtual bool startsWithObject() const;

			/**
			 * \brief Determine if this starts with an
			 *			array index.
			 *
			 * Note that object keys and array indices are
			 * not mutually exclusive.
			 *
			 * \returns `true` if this starts with an
			 *			array index, `false` otherwise.
			 **/
			virtual bool startsWithArray() const;

			/**
			 * \brief Get the number of keys in this path.
			 *
			 * \returns The length of this path.
			 **/
			virtual int length() const;

			/**
			 * \brief Get the `index`th key in this path.
			 *
			 * \returns The `index`th key in this path.
			 **/
			virtual JsonKey at(int index) const;

			/**
			 * \brief Get the `index`th key in this path.
			 *
			 * \returns The `index`th key in this path.
			 **/
			virtual JsonKey& at(int index);

			/**
			 * \brief Get the `index`th key in this path.
			 *
			 * \returns The `index`th key in this path.
			 **/
			virtual JsonKey operator[] (int index) const;

			/**
			 * \brief Get the `index`th key in this path.
			 *
			 * \returns The `index`th key in this path.
			 **/
			virtual JsonKey& operator[] (int index);

			/**
			 * \brief Set the `index`th key in this path to
			 *			a new value.
			 *
			 * \param[in] index The location of the key to change.
			 * \param[in] key The value to place at `index`.
			 *
			 * \returns `true` if `index` was within the bounds
			 *			of the path, `false` otherwise.
			 **/
			virtual bool set(int index, JsonKey key);

			/**
			 * \brief Remove the `index`th key from this path.
			 *
			 * \param[in] index The location of the key to remove.
			 *
			 * \returns `true` if `index` was within the bounds
			 *			of the path, `false` otherwise.
			 **/
			virtual bool remove(int index);

			/**
			 * \brief Insert a key before `index` in this path.
			 *
			 * If `index < 0`, the key is inserted at the beginning
			 * of the path (i.e. at `0`).
			 *
			 * If `index >= length()`, the key is inserted at the end
			 * of the path (i.e. at `length()`).
			 *
			 * \param[in] index The location to insert the new key.
			 * \param[in] key The new key to insert.
			 **/
			virtual void insert(int index, JsonKey key);

			/**
			 * \brief Append a key to this path.
			 *
			 * \param[in] key The key to append.
			 **/
			virtual void append(JsonKey key);

			/**
			 * \brief Append another path to this path.
			 *
			 * \param[in] path The path to append.
			 **/
			virtual void append(JsonPath path);

			/**
			 * \brief Remove the last key from this path.
			 *
			 * \returns `true` if this path was not empty,
			 *			`false` otherwise.
			 **/
			virtual bool removeLast();

			/**
			 * \brief Get the last key in this path.
			 *
			 * \returns The last key in this path,
			 *			or `JsonKey(QString())` if
			 *			the path is empty.
			 **/
			virtual JsonKey last() const;

			/**
			 * \brief Get the last key in this path.
			 *
			 * \returns The last key in this path,
			 *			or `JsonKey(QString())` if
			 *			the path is empty.
			 **/
			virtual JsonKey& last();

			/**
			 * \brief Add a key to the start of this path.
			 *
			 * \param[in] key The key to prepend.
			 **/
			virtual void prepend(JsonKey key);

			/**
			 * \brief Add a path to the start of this path.
			 *
			 * \param[in] path The path to prepend.
			 **/
			virtual void prepend(JsonPath path);

			/**
			 * \brief Remove the first key from this path.
			 *
			 * \returns `true` if this path was not empty,
			 *			`false` otherwise.
			 **/
			virtual bool removeFirst();

			/**
			 * \brief Get the first key in this path.
			 *
			 * \returns The first key in this path,
			 *			or `JsonKey(QString())` if
			 *			the path is empty.
			 **/
			virtual JsonKey first() const;

			/**
			 * \brief Get the first key in this path.
			 *
			 * \returns The last key in this path,
			 *			or `JsonKey(QString())` if
			 *			the path is empty.
			 **/
			virtual JsonKey& first();

			/**
			 * \brief Determine if the given key is in
			 *			this path.
			 *
			 * \param[in] key The key to look for.
			 *
			 * \returns `true` if this path contains `key`,
			 *			`false` otherwise.
			 **/
			virtual bool contains(JsonKey key) const;

			/**
			 * \brief Get the first index of the given key
			 *			in this path.
			 *
			 * If the key is not in this path, this
			 * returns `-1`.
			 *
			 * \param[in] key The key to look for.
			 *
			 * \returns The first index of the given key, or
			 *			`-1` if it does not exist.
			 **/
			virtual int indexOf(JsonKey key) const;

			/**
			 * \brief Get the last index of the given key
			 *			in this path.
			 *
			 * If the key is not in this path, this
			 * returns `-1`.
			 *
			 * \param[in] key The key to look for.
			 *
			 * \returns The last index of the given key, or
			 *			`-1` if it does not exist.
			 **/
			virtual int lastIndexOf(JsonKey key) const;

			/**
			 * \brief Trim the path to include only the
			 *			keys between `first` (inclusive)
			 *			and `last` (exclusive).
			 *
			 * \param[in] first The first key to include in
			 *				the new path.
			 * \param[in] last The index after the last key
			 *				to include in the new path.
			 *
			 * \returns The trimmed path.
			 **/
			virtual JsonPath trimmed(int first, int last) const;

			/**
			 * \brief Get an iterator to the beginning of
			 *			this path.
			 *
			 * \returns An iterator to the beginning of this
			 *			path.
			 **/
			virtual iterator begin();

			/**
			 * \brief Get an iterator to the beginning of
			 *			this path.
			 *
			 * \returns An iterator to the beginning of this
			 *			path.
			 **/
			virtual const_iterator begin() const;

			/**
			 * \brief Get an iterator to the beginning of
			 *			this path.
			 *
			 * \returns An iterator to the beginning of this
			 *			path.
			 **/
			virtual const_iterator constBegin() const;

			/**
			 * \brief Get an iterator to the end of
			 *			this path.
			 *
			 * \returns An iterator to the end of this
			 *			path.
			 **/
			virtual iterator end();

			/**
			 * \brief Get an iterator to the end of
			 *			this path.
			 *
			 * \returns An iterator to the end of this
			 *			path.
			 **/
			virtual const_iterator end() const;

			/**
			 * \brief Get an iterator to the end of
			 *			this path.
			 *
			 * \returns An iterator to the end of this
			 *			path.
			 **/
			virtual const_iterator constEnd() const;

		private:
			QSharedDataPointer<JsonPathPrivate> d;
	};

	/**
	 * \brief Class that represents a JSON key.
	 *
	 * This can be an integer or a string. It is
	 * used to access an value in a `JsonArray`
	 * or `JsonValue`.
	 **/
	class JSON_LIBRARY JsonKey : public QObject
	{
		Q_OBJECT

		Q_PROPERTY(bool isInteger
					READ isInteger
					STORED true)
		Q_PROPERTY(int integer
					READ toInteger)
		Q_PROPERTY(bool isString
					READ isString
					STORED false)
		Q_PROPERTY(QString string
					READ toString)
		Q_PROPERTY(bool isArrayIndex
					READ isArrayIndex
					STORED true)
		Q_PROPERTY(int arrayIndex
					READ toArrayIndex
					STORED false)
		Q_PROPERTY(bool isObjectKey
					READ isObjectKey
					STORED false)
		Q_PROPERTY(QString objectKey
					READ toObjectKey
					STORED false)

		public:
			/**
			 * \brief Construct a key from a string.
			 *
			 * All string keys can be object keys.
			 * If the string represents an integer,
			 * it can also be an array index.
			 *
			 * \param[in] key The string key.
			 **/
			JsonKey(QString key);

			/**
			 * \brief Construct a key from a string.
			 *
			 * All string keys can be object keys.
			 * If the string represents an integer,
			 * it can also be an array index.
			 *
			 * \param[in] key The string key.
			 **/
			JsonKey(const char* const key);

			/**
			 * \brief Construct a key from an integer.
			 *
			 * Integer keys can be array indices,
			 * but they cannot be object keys.
			 *
			 * \param[in] key The array index.
			 **/
			JsonKey(int key);

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonKey();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The key to copy.
			 **/
			JsonKey(const JsonKey& other);

			/**
			 * \brief Assign the value of `other` to
			 *			this key.
			 *
			 * \param[in] other The key to copy.
			 **/
			JsonKey& operator= (const JsonKey& other);

			/**
			 * \brief Determine if two keys are equal.
			 *
			 * \param other The key to compare to.
			 *
			 * \returns `true` if both this key and
			 *			`other` are stored in the
			 *			same way and have the same
			 *			value, `false` otherwise.
			 **/
			bool operator== (const JsonKey& other) const;

			/**
			 * \brief Compare to keys for inequality.
			 *
			 * \param other The key to compare to.
			 *
			 * \returns `true` if the keys are not equal,
			 *			`false` otherwise.
			 **/
			bool operator!= (const JsonKey& other) const;

			/**
			 * \brief Determine if this key can be
			 *			used as an object key.
			 *
			 * All string keys can be used as object
			 * keys, while no integer keys can be
			 * used as object keys, so this is
			 * essentially the same as `isString()`.
			 *
			 * \returns `true` if this can be used as
			 *			an object key, `false` otherwise.
			 **/
			virtual bool isObjectKey() const;

			/**
			 * \brief Convert this key to an object key.
			 *
			 * If `!isObjectKey()`, this returns `QString()`.
			 *
			 * \param[out] ok Flag set to `isObjectKey()`.
			 *
			 * \returns This key as an object key.
			 **/
			virtual QString toObjectKey(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this key can be
			 *			used as an array index.
			 *
			 * All integer keys can be used as array
			 * indices, while only some strings can.
			 *
			 * \returns `true` if this can be used as
			 *			an array index, `false` otherwise.
			 **/
			virtual bool isArrayIndex() const;

			/**
			 * \brief Convert this key to an array index.
			 *
			 * If `!isArrayIndex()`, this returns `-1`.
			 *
			 * \param[out] ok Flag set to `isArrayIndex()`.
			 *
			 * \returns This key as an array index.
			 **/
			virtual int toArrayIndex(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this key is stored
			 *			as a string.
			 *
			 * \returns `true` if this key is stored as
			 *			a string, `false` otherwise.
			 **/
			virtual bool isString() const;

			/**
			 * \brief Get this key as a string.
			 *
			 * If `!isString()`, this returns `QString()`.
			 *
			 * \param[out] ok Flag set to `isString()`.
			 *
			 * \returns This key as a string.
			 **/
			virtual QString toString(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this key is stored
			 *			as an integer.
			 *
			 * \returns `true` if this key is stored as
			 *			an integer, `false` otherwise.
			 **/
			virtual bool isInteger() const;

			/**
			 * \brief Convert this key to an integer.
			 *
			 * If `!isInteger()`, this returns `-1`.
			 *
			 * \param[out] ok Flag set to `isInteger()`.
			 *
			 * \returns This key as an integer.
			 **/
			virtual int toInteger(bool* ok = nullptr) const;

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
 **/
JSON_LIBRARY JSON::JsonPath& operator<< (JSON::JsonPath& path,
											JSON::JsonKey key);

/**
 * \brief Append another path to the given path.
 *
 * \param path The path to append to.
 * \param other The path to append.
 *
 * \returns The parameter `path`.
 **/
JSON_LIBRARY JSON::JsonPath& operator<< (JSON::JsonPath& path,
											JSON::JsonPath other);

#endif // JSON_PATH_H