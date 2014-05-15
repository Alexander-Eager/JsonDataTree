#ifndef JSON_READER_H
#define JSON_READER_H

// for the library
#include "JsonForwards.h"
#include "LibraryMacros.h"
#include "JsonValue.h"

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
	// internal data
	class JsonReaderPrivate;

	/**
	 * \brief A JSON text and file parser.
	 **/
	class JSON_LIBRARY JsonReader : public QObject
	{
		Q_OBJECT

		public:
			/**
			 * \brief Construct a reader.
			 **/
			JsonReader();

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonReader();

			/**
			 * \brief Parse the value from the
			 *			given string.
			 *
			 * \param[in] string The JSON string to
			 *			parse.
			 * \param[out] ok A flag set to `true` if
			 *				there were no errors,
			 *				`false` otherwise.
			 **/
			JsonValue parse(QString string, bool* ok = nullptr) const;

			/**
			 * \brief Read the data from the given
			 *			IO device.
			 *
			 * \param[in] io The IO device to read from.
			 * \param[out] ok A flag set to `true` if
			 *				there were no errors,
			 *				`false` otherwise.
			 **/
			JsonValue read(QIODevice* io, bool* ok = nullptr) const;

			/**
			 * \brief Read the data from the given
			 *			text stream.
			 *
			 * \param stream The text stream to read from.
			 * \param[out] ok A flag set to `true` if
			 *				there were no errors,
			 *				`false` otherwise.
			 **/
			JsonValue read(QTextStream& stream, bool* ok = nullptr) const;

		private:
			QSharedDataPointer<JsonReaderPrivate> d;
	};
}

#endif // JSON_READER_H