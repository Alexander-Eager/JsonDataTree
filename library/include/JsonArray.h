#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

// superclass
#include <QObject>

// for the JSON library
#include "JsonForwards.h"
#include "LibraryMacros.h"

// for holding data
#include <QSharedDataPointer>

namespace JSON
{
	// private JsonArray data
	class JsonArrayPrivate;

	/**
	 * \brief Represents an array of JSON values.
	 **/
	class JSON_LIBRARY JsonArray : public QObject
	{
		Q_OBJECT

		Q_PROPERTY(int size
					READ size
					STORED false)
		Q_PROPERTY(int count
					READ count
					STORED false)
		Q_PROPERTY(bool empty
					READ isEmpty
					STORED false)

		public:
			/**
			 * \brief The iterator type.
			 **/
			using iterator = QList<JsonValue>::iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using const_iterator = QList<JsonValue>::const_iterator;

			/**
			 * \brief For STL compatibility.
			 **/
			using const_pointer = QList<JsonArray>::const_pointer;

			/**
			 * \brief For STL compatibility.
			 **/
			using const_reference = QList<JsonArray>::const_reference;

			/**
			 * \brief For STL compatibility.
			 **/
			using difference_type = QList<JsonArray>::difference_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using pointer = QList<JsonArray>::pointer;

			/**
			 * \brief For STL compatibility.
			 **/
			using reference = QList<JsonArray>::reference;

			/**
			 * \brief For STL compatibility.
			 **/
			using size_type = QList<JsonArray>::size_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using value_type = QList<JsonArray>::value_type;

			/**
			 * \brief The iterator type.
			 **/
			using Iterator = QList<JsonValue>::Iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using ConstIterator = QList<JsonValue>::ConstIterator;

			/**
			 * \brief Make an empty array.
			 **/
			JsonArray();

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonArray();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param other The array to copy.
			 **/
			JsonArray(const JsonArray& other);

			/**
			 * \brief Assign the contents of `other` to
			 *			this object.
			 *
			 * \param other The array to copy.
			 **/
			JsonArray& operator= (const JsonArray& other);

			/**
			 * \brief Get the size of the array.
			 *
			 * \returns The size of this array.
			 **/
			virtual int size() const;

			/**
			 * \brief Get the size of this array.
			 *
			 * \see size()
			 *
			 * \returns The size of this array.
			 **/
			virtual int count() const;

			/**
			 * \brief Determine if this array is empty.
			 *
			 * \returns `true` if there are no values in
			 *			this array, `false` otherwise.
			 **/
			virtual bool isEmpty() const;

			/**
			 * \brief Remove all values from this array.
			 **/
			virtual void clear();

			/**
			 * \brief Add a value to the end of this array.
			 *
			 * \param value The value to append.
			 **/
			virtual void append(JsonValue value);

			/**
			 * \brief Remove the last value from this array.
			 *
			 * \returns `true` if the list was not empty,
			 *			`false` otherwise.
			 **/
			virtual bool removeLast();

			/**
			 * \brief Get the last value in this array.
			 *
			 * \returns The last value in this array.
			 **/
			virtual JsonValue last() const;

			/**
			 * \brief Get the last value in this array.
			 *
			 * \returns The last value in this array.
			 **/
			virtual JsonValue& last();

			/**
			 * \brief Add a value to the front of this array.
			 *
			 * \param value The value to prepend.
			 **/
			virtual void prepend(JsonValue value);

			/**
			 * \brief Remove the first value from this array.
			 *
			 * \returns `true` if the list was not empty,
			 *			`false` otherwise.
			 **/
			virtual bool removeFirst();

			/**
			 * \brief Get the first value in this array.
			 *
			 * \returns The first value in this array.
			 **/
			virtual JsonValue first() const;

			/**
			 * \brief Get the first value in this array.
			 *
			 * \returns The first value in this array.
			 **/
			virtual JsonValue& first();

			/**
			 * \brief Insert a value at a specific index.
			 *
			 * Everything after `index` moves up one spot.
			 *
			 * If `index >= size()`, the value is placed
			 * at the end. If `index <= 0`, the value is
			 * placed at the beginning.
			 *
			 * \param index The location to place the new value.
			 * \param value The value to add.
			 **/
			virtual void insert(int index, JsonValue value);

			/**
			 * \brief Remove a value at a specific index.
			 *
			 * Everything after `index` moves down one spot.
			 *
			 * \param index The location of the removal.
			 *
			 * \returns `true` if `index` is in the bounds of
			 *			the array, `false` otherwise.
			 **/
			virtual bool remove(int index);

			/**
			 * \brief Get the value at `index`.
			 *
			 * \returns The value at `index`.
			 **/
			virtual JsonValue at(int index) const;

			/**
			 * \brief Get the value at `index`.
			 *
			 * \returns The value at `index`.
			 **/
			virtual JsonValue& at(int index);

			/**
			 * \brief Get the value at `index`.
			 *
			 * \returns The value at `index`.
			 **/
			virtual JsonValue operator[] (int index) const;

			/**
			 * \brief Get the value at `index`.
			 *
			 * \returns The value at `index`.
			 **/
			virtual JsonValue& operator[] (int index);

			/**
			 * \brief Set the value at `index`.
			 *
			 * \param index The location to change the value at.
			 * \param value The new value for that location.
			 *
			 * \returns `true` if `index` is within the bounds
			 *			of the array, `false` otherwise.
			 **/
			virtual bool set(int index, JsonValue value);

			/**
			 * \brief Get an iterator to the start of this
			 *			array.
			 *
			 * \returns An iterator to the start of this array.
			 **/
			virtual iterator begin();

			/**
			 * \brief Get an iterator to the start of this
			 *			array.
			 *
			 * \returns An iterator to the start of this array.
			 **/
			virtual const_iterator begin() const;

			/**
			 * \brief Get an iterator to the start of this
			 *			array.
			 *
			 * \returns An iterator to the start of this array.
			 **/
			virtual const_iterator constBegin() const;

			/**
			 * \brief Get an iterator to the end of this
			 *			array.
			 *
			 * \returns An iterator to the end of this array.
			 **/
			virtual iterator end();

			/**
			 * \brief Get an iterator to the end of this
			 *			array.
			 *
			 * \returns An iterator to the end of this array.
			 **/
			virtual const_iterator end() const;

			/**
			 * \brief Get an iterator to the end of this
			 *			array.
			 *
			 * \returns An iterator to the end of this array.
			 **/
			virtual const_iterator constEnd() const;

		private:
			QSharedDataPointer<JsonArrayPrivate> d;
	};
}

Q_DECLARE_METATYPE(JSON::JsonArray)

/**
 * \brief Append a value to the given array.
 *
 * \param array The array to append to.
 * \param value The value to append.
 **/
JSON_LIBRARY JSON::JsonArray& operator<< (JSON::JsonArray& array,
											JSON::JsonValue value);

#endif // JSON_ARRAY_H