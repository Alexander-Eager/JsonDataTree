#ifndef JSON_VALUE_H
#define JSON_VALUE_H

// for the library
#include "JsonForwards.h"
#include "LibraryMacros.h"

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
	 *			array, object, string,
	 *			integer, floating point number,
	 *			or null.
	 **/
	class JSON_LIBRARY JsonValue : public QObject
	{
		Q_OBJECT

		Q_ENUMS(Type)

		Q_PROPERTY(Type type
					READ getType
					WRITE setType)
		Q_PROPERTY(bool isInteger
					READ isInteger
					STORED false)
		Q_PROPERTY(int integer
					READ toInteger
					WRITE setInteger)
		Q_PROPERTY(bool isDouble
					READ isDouble
					STORED false)
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
		Q_PROPERTY(bool isObject
					READ isObject
					STORED false)

		public:
			/** 
			 * \brief An `enum` for the type of value.
			 **/
			enum Type { Array = 0,
						Object,
						String,
						Integer,
						Double,
						Boolean,
						Null };

			/**
			 * \brief Destroys this object.
			 **/
			virtual ~JsonValue();

			/**
			 * \brief Construct a invalid reference.
			 *
			 * \see isValid()
			 **/
			JsonValue();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param other The value to copy.
			 **/
			JsonValue(const JsonValue& other);

			/**
			 * \brief Assign the value of `other` to this object.
			 *
			 * \param other The value to copy.
			 **/
			JsonValue& operator= (const JsonValue& other);

			/**
			 * \brief Construct a value with the given type.
			 *
			 * If the type is `Array` or `Object`, the default
			 * value is empty. If it is `String`, it is an
			 * empty string. If it is `Integer` or `Double`,
			 * it is `0`.
			 *
			 * \param[in] type The type of value to make.
			 **/
			JsonValue(Type type);

			/**
			 * \brief Reset the data this object holds to the
			 *			default for the given type.
			 *
			 * \param[in] type The type of value to become.
			 **/
			virtual void setType(Type type);

			/**
			 * \brief Make from an integer value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(int val);

			/**
			 * \brief Assign an integer value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setInteger(int val);

			/**
			 * \brief Make from a floating point (`double`) value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(double val);

			/**
			 * \brief Assign a floating point (`double`) value to
			 *			this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setDouble(double val);

			/**
			 * \brief Make from a string value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(QString val);

			/**
			 * \brief Assign a string value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setString(QString val);

			/**
			 * \brief Make from a string value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(const char* const val);

			/**
			 * \brief Assign a string value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setString(const char* const val);

			/**
			 * \brief Make from a boolean value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(bool val);

			/**
			 * \brief Assign a boolean value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setBoolean(bool val);

			/**
			 * \brief Make from an array value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(JsonArray val);

			/**
			 * \brief Assign an array value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setArray(JsonArray val);

			/**
			 * \brief Make from an object value.
			 *
			 * \param[in] val The value to assign to the object.
			 **/
			JsonValue(JsonObject val);

			/**
			 * \brief Assign an object value to this `JsonValue`.
			 *
			 * \param[in] val The value to assign to this object.
			 **/
			virtual void setObject(JsonObject val);

			/**
			 * \brief Get the type of value this object holds.
			 *
			 * \returns The type of value this object holds.
			 **/
			virtual Type getType() const;

			/**
			 * \brief Determine if this is a null value.
			 *
			 * \returns `true` if this is a null value,
			 *			`false` otherwise.
			 **/
			virtual bool isNull() const;

			/**
			 * \brief Determine if this is an integer value.
			 *
			 * \see toInteger(bool*)
			 *
			 * \returns `true` if this is an integer value,
			 *			`false` otherwise.
			 **/
			virtual bool isInteger() const;

			/**
			 * \brief Convert this value to an integer.
			 *
			 * If `isInteger()`, this simply returns the
			 * proper value.
			 *
			 * If `isDouble()`, this casts from `double` to
			 * `int` and returns the result, but sets
			 * `*ok` to `false`.
			 *
			 * If this is not a numeric value, this
			 * returns `0` and sets `*ok` to `false`.
			 *
			 * \see isInteger()
			 *
			 * \param[out] ok A flag set to `true` if `isInteger()`,
			 *				`false` otherwise.
			 *
			 * \returns This value as an integer.
			 **/
			virtual int toInteger(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this is a floating point
			 *			(`double`) value.
			 *
			 * \see toDouble(bool*)
			 *
			 * \returns `true` if this is a `double` value,
			 *			`false` otherwise.
			 **/
			virtual bool isDouble() const;

			/**
			 * \brief Convert this value to a `double`.
			 *
			 * If `isDouble()`, this simply returns the
			 * proper value.
			 *
			 * If `isInteger()`, this casts from `int` to
			 * `double` and returns the result, but sets
			 * `*ok` to `false`.
			 *
			 * If this is not a numeric value, this
			 * returns `0` and sets `*ok` to `false`.
			 *
			 * \see isDouble()
			 *
			 * \param[out] ok A flag set to `true` if `isDouble()`,
			 *				`false` otherwise.
			 *
			 * \returns This value as a `double`.
			 **/
			virtual double toDouble(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this is a string value.
			 *
			 * \see toString(bool*)
			 *
			 * \returns `true` if this is a string value,
			 *			`false` otherwise.
			 **/
			virtual bool isString() const;

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
			 *				`false` otherwise.
			 *
			 * \returns This value as a string.
			 **/
			virtual QString toString(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this is a boolean value.
			 *
			 * \see toBoolean(bool*)
			 *
			 * \returns `true` if this is a boolean value,
			 *			`false` otherwise.
			 **/
			virtual bool isBoolean() const;

			/**
			 * \brief Convert this value to a boolean.
			 *
			 * If `isBoolean()`, this simply returns the
			 * proper value.
			 *
			 * If `isInteger()` or `isDouble()`, this returns
			 * `true` if non-zero and `false` if zero, and
			 * sets `*ok` to `false`.
			 *
			 * If this is anything else, this returns `false`
			 * and sets `*ok` to `false`.
			 *
			 * \see isBoolean()
			 *
			 * \param[out] ok A flag set to `true` if `isBoolean()`,
			 *				`false` otherwise.
			 *
			 * \returns This value as a boolean.
			 **/
			virtual bool toBoolean(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this value is an array.
			 *
			 * \see toArray(bool*)
			 *
			 * \returns `true` if this value is an array,
			 *			`false` otherwise.
			 **/
			virtual bool isArray() const;

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
			 *				an array, `false` otherwise.
			 *
			 * \returns This value as an array.
			 **/
			virtual JsonArray& toArray(bool* ok = nullptr);

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
			 *				an array, `false` otherwise.
			 *
			 * \returns This value as an array.
			 **/
			virtual JsonArray toArray(bool* ok = nullptr) const;

			/**
			 * \brief Determine if this value is an object.
			 *
			 * \see toObject(bool*)
			 *
			 * \returns `true` if this value is an object,
			 *			`false` otherwise.
			 **/
			virtual bool isObject() const;

			/**
			 * \brief Convert this value to an object.
			 *
			 * If this value is not an object, this returns
			 * an empty `JsonObject` and sets `*ok` to `false`.
			 *
			 * \see isObject()
			 *
			 * \param[out] ok A flag set to `true` if this value is
			 *				an object, `false` otherwise.
			 *
			 * \returns This value as an object.
			 **/
			virtual JsonObject& toObject(bool* ok = nullptr);

			/**
			 * \brief Convert this value to an object.
			 *
			 * If this value is not an object, this returns
			 * an empty `JsonObject` and sets `*ok` to `false`.
			 *
			 * \see isObject()
			 *
			 * \param[out] ok A flag set to `true` if this value is
			 *				an object, `false` otherwise.
			 *
			 * \returns This value as an object.
			 **/
			virtual JsonObject toObject(bool* ok = nullptr) const;

		private:
			/** \brief The *d-pointer* for this object. **/
			QSharedDataPointer<JsonValuePrivate> d;
	};
}

Q_DECLARE_METATYPE(JSON::JsonValue)

#endif // JSON_VALUE_H