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

// for errors
#include <QString>

namespace JSON
{
	// internal data
	class JsonReaderPrivate;
	class JsonReaderErrorPrivate;
	class JsonReaderErrorsPrivate;

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
			 * \brief Make a copy of other.
			 *
			 * \param[in] other The reader to copy.
			 **/
			JsonReader(const JsonReader& other);

			/**
			 * \brief Parse the value from the
			 *			given string.
			 *
			 * \param[in] string The JSON string to
			 *			parse.
			 * \param[out] errors A list of all errors
			 *						that occured.
			 **/
			JsonValue parse(QString string,
							JsonReaderErrors* errors = nullptr) const;

			/**
			 * \brief Read the data from the given
			 *			IO device.
			 *
			 * \param[in] io The IO device to read from.
			 * \param[out] errors A list of all errors
			 *						that occured.
			 **/
			JsonValue read(QIODevice* io,
							JsonReaderErrors* errors = nullptr) const;

			/**
			 * \brief Read the data from the given
			 *			text stream.
			 *
			 * \param[in] stream The text stream to read from.
			 * \param[out] errors A list of all errors
			 *						that occured.
			 **/
			JsonValue read(QTextStream& stream,
							JsonReaderErrors* errors = nullptr) const;

		private:
			QSharedDataPointer<JsonReaderPrivate> d;
	};

	/**
	 * \brief A class that describes errors
	 *			when reading the file.
	 **/
	class JSON_LIBRARY JsonReaderError : public QObject
	{
		Q_OBJECT

		Q_ENUMS(ErrorType)

		Q_PROPERTY(ErrorType type
					READ type)
		Q_PROPERTY(QString message
					READ message
					STORED false)
		Q_PROPERTY(int offset
					READ offset)

		public:
			/**
			 * \brief A type for describing the kind of error.
			 **/
			enum ErrorType {
				NoError = 0,
				NumberWithBadCharacter,
				StringWithBadEscape,
				StringWithBadHex,
				StringWithNoClosingQuote,
				UnknownLiteral,
				ObjectWithMissingColon,
				ObjectWithMissingComma,
				ObjectWithExtraComma,
				ObjectWithNoClosingBrace,
				ObjectWithNonStringKey,
				ArrayWithMissingComma,
				ArrayWithExtraComma,
				ArrayWithNoClosingBracket,
				CommentWithNoEnd,
				StreamFailure
			};

			/**
			 * \brief Make a reader error with the given type
			 *			and offset.
			 *
			 * \param[in] type The type of error.
			 * \param[in] offset The location of the error in the
			 *					file or string.
			 **/
			JsonReaderError(ErrorType type, int offset);

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonReaderError();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The error to copy.
			 **/
			JsonReaderError(const JsonReaderError& other);

			/**
			 * \brief Assign the error of `other` to
			 *			this error.
			 *
			 * \param[in] other The error to copy.
			 **/
			JsonReaderError& operator= (const JsonReaderError& other);

			/**
			 * \brief Get the error type.
			 *
			 * \returns The error type.
			 **/
			ErrorType type() const;

			/**
			 * \brief Get the error message.
			 *
			 * \returns The error message.
			 **/
			QString message() const;

			/**
			 * \brief Get the error offset.
			 *
			 * \returns The error offset.
			 **/
			int offset() const;

		private:
			QSharedDataPointer<JsonReaderErrorPrivate> d;
	};

	/**
	 * \brief A class that holds multiple reader errors
	 *			for the same reading session.
	 **/
	class JSON_LIBRARY JsonReaderErrors : public QObject
	{
		Q_OBJECT

		Q_PROPERTY(int numErrors
					READ numErrors
					STORED false)

		public:
			/**
			 * \brief The constant iterator type.
			 **/
			using const_iterator = QList<JsonReaderError>::const_iterator;

			/**
			 * \brief The constant iterator type.
			 **/
			using ConstIterator = const_iterator;

			// so that `JsonReader` can add errors
			friend class JsonReader;
			friend class JsonReaderPrivate;

			/**
			 * \brief Construct an empty grouping of errors.
			 **/
			JsonReaderErrors();

			/**
			 * \brief Destroy this object.
			 **/
			virtual ~JsonReaderErrors();

			/**
			 * \brief Make a copy of `other`.
			 *
			 * \param[in] other The errors to copy.
			 **/
			JsonReaderErrors(const JsonReaderErrors& other);

			/**
			 * \brief Assign the errors of `other` to this
			 *			collection.
			 *
			 * \param[in] other The errors to copy.
			 **/
			JsonReaderErrors& operator= (const JsonReaderErrors& other);

			/**
			 * \brief Get the number of errors.
			 *
			 * \returns The number of errors.
			 **/
			int numErrors() const;

			/**
			 * \brief Get `i`th error.
			 *
			 * If `i` is out of bounds, this
			 * returns a `JsonReaderError::NoError`
			 * type of error.
			 *
			 * \param[in] i The error to get.
			 *
			 * \returns The `i`th error.
			 **/
			JsonReaderError get(int i) const;

			/**
			 * \brief Get an iterator to the first error.
			 *
			 * \returns An iterator to the first error.
			 **/
			const_iterator begin() const;

			/**
			 * \brief Get an iterator to the last error.
			 *
			 * \returns An iterator to the last error.
			 **/
			const_iterator end() const;

		private:
			QSharedDataPointer<JsonReaderErrorsPrivate> d;

			/**
			 * \brief Used by `JsonReader` to add an error.
			 *
			 * \param[in] error The error to add.
			 **/
			void addError(JsonReaderError error);

			/**
			 * \brief Used by `JsonReader` to add an error.
			 *
			 * \param[in] type The type of error to add.
			 * \param[in] offset The offset of the error.
			 **/
			void addError(JsonReaderError::ErrorType type, int offset);
	};
}

#endif // JSON_READER_H