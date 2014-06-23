#ifndef JSON_WRITER_H
#define JSON_WRITER_H

// for the library
#include <JsonDataTree/JsonForwards.h>
#include <JsonDataTree/LibraryMacros.h>
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
	 */
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
			 */
			JsonWriter();

			/**
			 * \brief Construct a JSON writer to
			 *			write the given data.
			 *
			 * \param[in] data The data to write out.
			 */
			JsonWriter(JsonValue data);

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The writer to copy.
			 */
			JsonWriter(const JsonWriter& other);

			/**
			 * \brief Destroy this object.
			 */
			~JsonWriter();

			/**
			 * \brief Assign the data of `other` to this
			 *			object.
			 *
			 * \param[in] other The writer to copy.
			 */
			auto operator= (const JsonWriter& other) -> JsonWriter&;

			/**
			 * \brief Get the data that this writer writes out.
			 *
			 * \returns The data to be written by this writer.
			 */
			auto getData() const -> JsonValue;

			/**
			 * \brief Set the data that this writer writes out.
			 *
			 * \param[in] data The new data to write.
			 */
			auto setData(JsonValue data) -> void;

			/**
			 * \brief Get the data as a string.
			 *
			 * \returns The data as a string.
			 */
			auto string() const -> QString;

			/**
			 * \brief Write the data to a string.
			 *
			 * \param[out] str The string to write to.
			 */
			auto writeTo(QString* str) const -> void;

			/**
			 * \brief Write the data to an IO device.
			 *
			 * \param[out] io The device to write to.
			 */
			auto writeTo(QIODevice* io) const -> void;

			/**
			 * \brief Write the data to a text stream.
			 *
			 * \param[out] stream The stream to write to.
			 */
			auto writeTo(QTextStream& stream) const -> void;

		private:
			QSharedDataPointer<JsonWriterPrivate> d;
	};
}

#endif // JSON_WRITER_H
