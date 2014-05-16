#ifndef JSON_WRITER_H
#define JSON_WRITER_H

// for the library
#include <JsonDataTree/JsonForwards.h>
#include <JsonDataTree/LibraryMacros.h?
#include <JsonDataTree/JsonValue.h>

// superclass
#include <QObject>

// for implicit sharing
#include <QSharedDataPointer>

// for writing out the data
#include <QTextStream>
#include <QIODevice>
#include <QString>

namespace JSON
{
	// internal data class
	class JsonWriterPrivate;

	/**
	 * \brief Class used to write JSON data to
	 *			streams in text format.
	 **/
	class JSON_LIBRARY JsonWriter : public QObject
	{
		Q_OBJECT

		Q_PROPERTY(JsonValue data
					READ getData
					WRITE setData)
		Q_PROPERTY(QString string
					READ string
					STORED false)

		public:
			/**
			 * \brief Construct a JSON writer
			 *			with null data.
			 **/
			JsonWriter();

			/**
			 * \brief Construct a JSON writer to
			 *			write the given data.
			 *
			 * \param[in] data The data to write out.
			 **/
			JsonWriter(JsonValue data);

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The writer to copy.
			 **/
			JsonWriter(const JsonWriter& other);

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonWriter();

			/**
			 * \brief Assign the data of `other` to this
			 *			object.
			 *
			 * \param[in] other The writer to copy.
			 **/
			virtual JsonWriter& operator= (const JsonWriter& other);

			/**
			 * \brief Get the data that this writer writes out.
			 *
			 * \returns The data to be written by this writer.
			 **/
			virtual JsonValue getData() const;

			/**
			 * \brief Set the data that this writer writes out.
			 *
			 * \param[in] data The new data to write.
			 **/
			virtual void setData(JsonValue data);

			/**
			 * \brief Get the data as a string.
			 *
			 * \returns The data as a string.
			 **/
			virtual QString string() const;

			/**
			 * \brief Write the data to a string.
			 *
			 * \param[out] str The string to write to.
			 **/
			virtual void writeTo(QString* str) const;

			/**
			 * \brief Write the data to an IO device.
			 *
			 * \param[out] io The device to write to.
			 **/
			virtual void writeTo(QIODevice* io) const;

			/**
			 * \brief Write the data to a text stream.
			 *
			 * \param[out] stream The stream to write to.
			 **/
			virtual void writeTo(QTextStream& stream) const;

		private:
			QSharedDataPointer<JsonWriterPrivate> d;
	};
}

#endif // JSON_WRITER_H