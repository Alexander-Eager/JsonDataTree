#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

// for the library
#include <JsonDataTree/JsonForwards.h>
#include <JsonDataTree/LibraryMacros.h>

// superclass
#include <QObject>

// for implicit sharing
#include <QSharedDataPointer>

// for data
#include <QString>
#include <QHash>

namespace JSON
{
	// private internal data
	class JsonObjectPrivate;

	/**
	 * \brief Represents a JSON object (i.e., a map).
	 **/
	class JSON_LIBRARY JsonObject : public QObject
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
		Q_PROPERTY(QList<QString> keys
					READ keys
					STORED false)

		public:
			/**
			 * \brief The iterator type.
			 **/
			using iterator = QHash<QString, JsonValue>::iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using const_iterator = QHash<QString, JsonValue>::const_iterator;

			/**
			 * \brief For STL compatibility.
			 **/
			using difference_type = QHash<QString, JsonValue>::difference_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using key_type = QHash<QString, JsonValue>::key_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using mapped_type = QHash<QString, JsonValue>::mapped_type;

			/**
			 * \brief For STL compatibility.
			 **/
			using size_type = QHash<QString, JsonValue>::size_type;

			/**
			 * \brief The iterator type.
			 **/
			using Iterator = QHash<QString, JsonValue>::Iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using ConstIterator = QHash<QString, JsonValue>::ConstIterator;

			/**
			 * \brief Make an empty object.
			 **/
			JsonObject();

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonObject();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The object to copy.
			 **/
			JsonObject(const JsonObject& other);

			/**
			 * \brief Assign the contents of `other` to this
			 *			object.
			 *
			 * \param[in] other The object to copy.
			 **/
			JsonObject& operator= (const JsonObject& other);

			/**
			 * \brief Get the number of key-value pairs.
			 *
			 * \returns The number of key-value pairs.
			 **/
			virtual int size() const;

			/**
			 * \brief Get the number of key-value pairs.
			 *
			 * \see size()
			 *
			 * \returns The number of key-value pairs.
			 **/
			virtual int count() const;

			/**
			 * \brief Determine if this object has no
			 *			key-value pairs.
			 *
			 * \returns `true` if this object is empty,
			 *			`false` otherwise.
			 **/
			virtual bool isEmpty() const;

			/**
			 * \brief Remove all key-value pairs from this
			 *			object.
			 **/
			virtual void clear();

			/**
			 * \brief Get the keys in this object.
			 *
			 * \returns The keys in this object.
			 **/
			virtual QList<QString> keys() const;

			/**
			 * \brief Insert key-value pair.
			 *
			 * If `key` already exists in this object,
			 * the associated value is replaced with
			 * `value`.
			 *
			 * \param[in] key The key for the pair.
			 * \param[in] value The value for the pair.
			 *
			 * \returns An iterator to the position with
			 *			the key-value pair.
			 **/
			virtual iterator insert(QString key, JsonValue value);

			/**
			 * \brief Get the value for a specific key.
			 *
			 * If the key does not exist, this returns a
			 * `Null` `JsonValue`.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns The associated value.
			 **/
			virtual JsonValue get(QString key) const;

			/**
			 * \brief Get the value for a specific key.
			 *
			 * If the key does not exist, this adds the
			 * key and associates it with a null value.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns The associated value.
			 **/
			virtual JsonValue& get(QString key);

			/**
			 * \brief Get the value for a specific key.
			 *
			 * If the key does not exist, this returns a
			 * `Null` `JsonValue`.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns The associated value.
			 **/
			virtual JsonValue operator[] (QString key) const;

			/**
			 * \brief Get the value for a specific key.
			 *
			 * If the key does not exist, this adds the
			 * key and associates it with a null value.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns The associated value.
			 **/
			virtual JsonValue& operator[] (QString key);

			/**
			 * \brief Get an iterator to the specific key.
			 *
			 * If the key does not exist, this returns
			 * `end()`.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns An iterator to `key`.
			 **/
			virtual iterator find(QString key);

			/**
			 * \brief Get an iterator to the specific key.
			 *
			 * If the key does not exist, this returns
			 * `end()`.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns An iterator to `key`.
			 **/
			virtual const_iterator find(QString key) const;

			/**
			 * \brief Get an iterator to the specific key.
			 *
			 * If the key does not exist, this returns
			 * `end()`.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns An iterator to `key`.
			 **/
			virtual const_iterator constFind(QString key) const;

			/**
			 * \brief Determine if this object has the given
			 *			key.
			 *
			 * \param[in] key The key to lookup.
			 *
			 * \returns `true` if the key was found,
			 *			`false` otherwise.
			 **/
			virtual bool contains(QString key) const;

			/**
			 * \brief Get an iterator to the start of this
			 *			object.
			 *
			 * \returns An iterator to the start of this object.
			 **/
			virtual iterator begin();

			/**
			 * \brief Get an iterator to the start of this
			 *			object.
			 *
			 * \returns An iterator to the start of this object.
			 **/
			virtual const_iterator begin() const;

			/**
			 * \brief Get an iterator to the start of this
			 *			object.
			 *
			 * \returns An iterator to the start of this object.
			 **/
			virtual const_iterator constBegin() const;

			/**
			 * \brief Get an iterator to the end of this
			 *			object.
			 *
			 * \returns An iterator to the end of this object.
			 **/
			virtual iterator end();

			/**
			 * \brief Get an iterator to the end of this
			 *			object.
			 *
			 * \returns An iterator to the end of this object.
			 **/
			virtual const_iterator end() const;

			/**
			 * \brief Get an iterator to the end of this
			 *			object.
			 *
			 * \returns An iterator to the end of this object.
			 **/
			virtual const_iterator constEnd() const;

		private:
			QSharedDataPointer<JsonObjectPrivate> d;
	};
}

#endif // JSON_OBJECT_H