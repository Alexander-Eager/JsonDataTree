#ifndef JSON_READER_H
#define JSON_READER_H

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
     */
    class JSON_LIBRARY JsonReader : public QObject
    {
        Q_OBJECT

        public:
            /**
             * \brief Construct a reader.
             */
            JsonReader();

            /**
             * \brief Destroy this object.
             */
            ~JsonReader();

            /**
             * \brief Make a copy of other.
             *
             * \param[in] other The reader to copy.
             */
            JsonReader(const JsonReader& other);

            /**
             * \brief Parse the value from the
             *          given string.
             *
             * \param[in] string The JSON string to
             *          parse.
             * \param[out] errors A list of all errors
             *                      that occured.
             */
            auto parse(QString string,
                       JsonReaderErrors* errors = nullptr) const -> JsonValue;

            /**
             * \brief Read the data from the given
             *          IO device.
             *
             * \param[in] io The IO device to read from.
             * \param[out] errors A list of all errors
             *                      that occured.
             */
            auto read(QIODevice* io,
                      JsonReaderErrors* errors = nullptr) const -> JsonValue;

            /**
             * \brief Read the data from the given
             *          text stream.
             *
             * \param[in] stream The text stream to read from.
             * \param[out] errors A list of all errors
             *                      that occured.
             */
            auto read(QTextStream& stream,
                      JsonReaderErrors* errors = nullptr) const -> JsonValue;

        private:
            QSharedDataPointer<JsonReaderPrivate> d;
    };

    /**
     * \brief A class that describes errors
     *          when reading the file.
     */
    class JSON_LIBRARY JsonReaderError
    {
        public:
            /**
             * \brief A type for describing the kind of error.
             */
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
             *          and offset.
             *
             * \param[in] type The type of error.
             * \param[in] offset The location of the error in the
             *                  file or string.
             */
            JsonReaderError(ErrorType type, int offset);

            /**
             * \brief Destroy this object.
             */
            ~JsonReaderError();

            /**
             * \brief Make a copy of `other`.
             *
             * \param[in] other The error to copy.
             */
            JsonReaderError(const JsonReaderError& other);

            /**
             * \brief Assign the error of `other` to
             *          this error.
             *
             * \param[in] other The error to copy.
             */
            auto operator= (const JsonReaderError& other) -> JsonReaderError&;

            /**
             * \brief Get the error type.
             *
             * \returns The error type.
             */
            auto type() const -> ErrorType;

            /**
             * \brief Get the error message.
             *
             * \returns The error message.
             */
            auto message() const -> QString;

            /**
             * \brief Get the error offset.
             *
             * \returns The error offset.
             */
            auto offset() const -> int;

        private:
            QSharedDataPointer<JsonReaderErrorPrivate> d;
    };

    /**
     * \brief A class that holds multiple reader errors
     *          for the same reading session.
     */
    class JSON_LIBRARY JsonReaderErrors
    {
        public:
            /**
             * \brief The constant iterator type.
             */
            using const_iterator = QList<JsonReaderError>::const_iterator;

            /**
             * \brief The constant iterator type.
             */
            using ConstIterator = const_iterator;

            // so that `JsonReader` can add errors
            friend class JsonReader;
            friend class JsonReaderPrivate;

            /**
             * \brief Construct an empty grouping of errors.
             */
            JsonReaderErrors();

            /**
             * \brief Destroy this object.
             */
            ~JsonReaderErrors();

            /**
             * \brief Make a copy of `other`.
             *
             * \param[in] other The errors to copy.
             */
            JsonReaderErrors(const JsonReaderErrors& other);

            /**
             * \brief Assign the errors of `other` to this
             *          collection.
             *
             * \param[in] other The errors to copy.
             */
            auto operator= (const JsonReaderErrors& other) -> JsonReaderErrors&;

            /**
             * \brief Get the number of errors.
             *
             * \returns The number of errors.
             */
            auto numErrors() const -> int;

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
             */
            auto get(int i) const -> JsonReaderError;

            /**
             * \brief Get an iterator to the first error.
             *
             * \returns An iterator to the first error.
             */
            auto begin() const -> const_iterator;

            /**
             * \brief Get an iterator to the last error.
             *
             * \returns An iterator to the last error.
             */
            auto end() const -> const_iterator;

        private:
            QSharedDataPointer<JsonReaderErrorsPrivate> d;

            /**
             * \brief Used by `JsonReader` to add an error.
             *
             * \param[in] error The error to add.
             */
            auto addError(JsonReaderError error) -> void;

            /**
             * \brief Used by `JsonReader` to add an error.
             *
             * \param[in] type The type of error to add.
             * \param[in] offset The offset of the error.
             */
            auto addError(JsonReaderError::ErrorType type, int offset) -> void;
    };
}

#endif // JSON_READER_H
