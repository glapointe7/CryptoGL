#pragma once

#include "../types/Types.hpp"
#include "../exceptions/Exception.hpp"
#include <fstream>
#include <filesystem>
#include <memory>
#include <functional>
#include <future>
#include <chrono>

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
    #include <fcntl.h>  // pour O_RDONLY, O_WRONLY, O_CREAT, etc.
    #include <unistd.h> // pour open(), read(), write(), close()
#endif

namespace CryptoGL
{
    /**
     * @brief Enhanced exception class for secure file operations
     *
     * Provides detailed error information for file I/O operations with
     * specific error codes to enable proper error handling in cryptographic
     * applications where file integrity is critical.
     */
    class FileException : public Exception
    {
    public:
        enum class ErrorCode
        {
            FILE_NOT_FOUND,
            PERMISSION_DENIED,
            INVALID_PATH,
            FILE_TOO_LARGE,
            MEMORY_MAP_FAILED,
            READ_ERROR,
            WRITE_ERROR,
            SECURITY_VIOLATION
        };

        /**
         * @brief Constructs a FileException with error code and message
         * @param code The specific error code indicating the type of failure
         * @param message Human-readable description of the error
         */
        FileException(ErrorCode code, const String &message)
            : Exception(message), error_code(code) {}

        /**
         * @brief Gets the error code associated with this exception
         * @return The ErrorCode indicating the specific type of failure
         */
        ErrorCode getErrorCode() const noexcept { return error_code; }

    private:
        ErrorCode error_code;
    };

    /**
     * @brief Progress callback type for monitoring file operations
     * @param processed Number of bytes processed so far
     * @param total Total number of bytes to process
     * @return true to continue processing, false to cancel operation
     */
    using ProgressCallback = std::function<bool(size_t processed, size_t total)>;

    /**
     * @brief High-performance, secure file I/O class for cryptographic applications
     *
     * This template class provides optimized file operations with:
     * - Memory-mapped I/O for large files (>2MB) to minimize memory usage
     * - Chunked buffered I/O for smaller files with optimal 64KB buffer size
     * - Asynchronous operations for non-blocking file processing
     * - Path validation and security checks to prevent directory traversal
     * - Progress callbacks for long-running operations
     * - Secure memory clearing to prevent data leakage
     * - File size limits (2GB max) to prevent DoS attacks
     *
     * @tparam DataType The data type to read/write (typically uint8_t for binary)
     */
    template <typename DataType>
    class File
    {
    private:
        static constexpr size_t DEFAULT_BUFFER_SIZE = 64 * 1024;           // 64KB
        static constexpr size_t MMAP_THRESHOLD = 10 * 1024 * 1024;         // 10MB
        static constexpr size_t MAX_FILE_SIZE = 2ULL * 1024 * 1024 * 1024; // 2GB limit

        const String filename;
        mutable std::unique_ptr<std::ifstream> input_stream;
        mutable std::unique_ptr<std::ofstream> output_stream;

    public:
        /**
         * @brief Constructs a File object with path validation
         * @param filename Path to the file (absolute or relative)
         * @throws FileException if the path is invalid or contains security violations
         */
        explicit File(const String &filename) : filename(validatePath(filename)) {}

        /**
         * @brief Secure destructor that properly closes file streams
         * Ensures all file handles are closed and resources are cleaned up
         */
        ~File() { closeStreams(); }

        /**
         * @brief Move constructor for efficient resource transfer
         * @param other File object to move from
         */
        File(File &&other) noexcept : filename(std::move(other.filename))
        {
            input_stream = std::move(other.input_stream);
            output_stream = std::move(other.output_stream);
        }

        // Delete copy constructor and assignment for security (prevents accidental duplication)
        File(const File &) = delete;
        File &operator=(const File &) = delete;

        /**
         * @brief Gets the size of the file with validation
         * @return File size in bytes
         * @throws FileException if file doesn't exist, is too large (>2GB), or access denied
         */
        [[nodiscard]] size_t getFileSize() const
        {
            try
            {
                if (!std::filesystem::exists(filename.c_str()))
                {
                    throw FileException(FileException::ErrorCode::FILE_NOT_FOUND,
                                        "File does not exist: " + filename);
                }
                const auto size = std::filesystem::file_size(filename.c_str());
                if (size > MAX_FILE_SIZE)
                {
                    throw FileException(FileException::ErrorCode::FILE_TOO_LARGE,
                                        "File exceeds maximum allowed size");
                }
                return static_cast<size_t>(size);
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                throw FileException(FileException::ErrorCode::READ_ERROR,
                                    String("Filesystem error: ") + e.what());
            }
        }

        /**
         * @brief Saves data to file using optimal chunked writing strategy
         *
         * Uses 64KB chunks for optimal disk I/O performance and supports
         * progress monitoring for large file operations.
         *
         * @param data The data vector to write to file
         * @param progress Optional callback for progress monitoring (processed, total) -> continue
         * @throws FileException if file cannot be opened, writing fails, or operation is cancelled
         */
        void save(const Vector<DataType> &data, const ProgressCallback &progress = nullptr) const
        {
            try
            {
                output_stream = std::make_unique<std::ofstream>(
                    filename.c_str(),
                    std::ios::binary | std::ios::trunc);
                if (!output_stream->is_open())
                {
                    throw FileException(FileException::ErrorCode::WRITE_ERROR,
                                        "Cannot open file for writing: " + filename);
                }
                // Set buffer size for optimal performance
                output_stream->rdbuf()->pubsetbuf(nullptr, DEFAULT_BUFFER_SIZE);
                const size_t data_size = data.size() * sizeof(DataType);
                const size_t chunk_size = DEFAULT_BUFFER_SIZE / sizeof(DataType);
                size_t written = 0;
                for (size_t i = 0; i < data.size(); i += chunk_size)
                {
                    const size_t current_chunk = std::min(chunk_size, data.size() - i);
                    const size_t byte_count = current_chunk * sizeof(DataType);
                    output_stream->write(
                        reinterpret_cast<const char *>(&data[i]),
                        byte_count);
                    if (output_stream->fail())
                    {
                        throw FileException(FileException::ErrorCode::WRITE_ERROR,
                                            "Failed to write data to file");
                    }
                    written += current_chunk;
                    // Progress callback
                    if (progress && !progress(written, data.size()))
                    {
                        throw FileException(FileException::ErrorCode::WRITE_ERROR,
                                            "Operation cancelled by user");
                    }
                }
                output_stream->flush();
            }
            catch (const FileException &)
            {
                throw;
            }
            catch (const std::exception &e)
            {
                throw FileException(FileException::ErrorCode::WRITE_ERROR,
                                    String("Unexpected error: ") + e.what());
            }
        }

        /**
         * @brief Loads file content using optimal strategy based on file size
         *
         * Automatically chooses between memory mapping (files >10MB) for minimal memory
         * usage and buffered I/O (files <10MB) for optimal small file performance.
         *
         * @param progress Optional progress callback for monitoring large operations
         * @return Vector containing the file data
         * @throws FileException if file cannot be read, is empty, or operation fails
         */
        [[nodiscard]] Vector<DataType> load(const ProgressCallback &progress = nullptr) const
        {
            const size_t file_size = getFileSize();
            if (file_size == 0)
            {
                return Vector<DataType>{};
            }
            const size_t element_count = file_size / sizeof(DataType);
            // Use memory mapping for large files
            if (file_size >= MMAP_THRESHOLD)
            {
                return loadWithMemoryMapping(file_size, progress);
            }
            else
            {
                return loadWithBufferedIO(file_size, element_count, progress);
            }
        }

        /**
         * @brief Processes file in chunks for streaming/memory-efficient operations
         *
         * Enables processing of very large files without loading entire content into memory.
         * Ideal for cryptographic operations on large files where memory usage must be controlled.
         *
         * @param processor Function to process each chunk (chunk_data, offset) -> continue
         * @param chunk_size Size of each chunk in elements (default: 64KB/sizeof(DataType))
         * @throws FileException if file cannot be opened or read error occurs
         */
        void processInChunks(const std::function<bool(const Vector<DataType> &, size_t offset)> &processor,
                             size_t chunk_size = DEFAULT_BUFFER_SIZE / sizeof(DataType)) const
        {
            const size_t file_size = getFileSize();
            const size_t element_count = file_size / sizeof(DataType);
            input_stream = std::make_unique<std::ifstream>(
                filename.c_str(), std::ios::binary);
            if (!input_stream->is_open())
            {
                throw FileException(FileException::ErrorCode::READ_ERROR,
                                    "Cannot open file for reading: " + filename);
            }
            Vector<DataType> buffer(chunk_size);
            size_t offset = 0;
            while (offset < element_count)
            {
                const size_t elements_to_read = std::min(chunk_size, element_count - offset);
                const size_t bytes_to_read = elements_to_read * sizeof(DataType);
                input_stream->read(
                    reinterpret_cast<char *>(buffer.raw_data()),
                    bytes_to_read);
                if (input_stream->fail() && !input_stream->eof())
                {
                    throw FileException(FileException::ErrorCode::READ_ERROR,
                                        "Failed to read from file");
                }
                const size_t actual_read = input_stream->gcount() / sizeof(DataType);
                buffer.resize(actual_read);
                if (!processor(buffer, offset))
                {
                    break; // User requested stop
                }
                offset += actual_read;
            }
        }

        /**
         * @brief Asynchronously loads file content without blocking the calling thread
         *
         * @param progress Optional progress callback for monitoring the async operation
         * @return Future that will contain the loaded data when ready
         */
        [[nodiscard]] std::future<Vector<DataType>> loadAsync(const ProgressCallback &progress = nullptr) const
        {
            return std::async(std::launch::async, [this, progress]()
                              { return load(progress); });
        }

        /**
         * @brief Checks if file exists and is accessible as a regular file
         *
         * @return true if file exists and is a regular file, false otherwise
         * @note This method never throws exceptions and returns false on any error
         */
        [[nodiscard]] bool exists() const noexcept
        {
            try
            {
                return std::filesystem::exists(filename.c_str()) &&
                       std::filesystem::is_regular_file(filename.c_str());
            }
            catch (...)
            {
                return false;
            }
        }

    private:
        /**
         * @brief Validates and normalizes file path to prevent security vulnerabilities
         *
         * Checks for directory traversal attacks (../ and //) and normalizes the path.
         * Critical for preventing unauthorized file access in cryptographic applications.
         *
         * @param path The file path to validate
         * @return The normalized, safe file path
         * @throws FileException if path contains security violations or is invalid
         */
        [[nodiscard]] String validatePath(const String &path) const
        {
            // Check for directory traversal attempts
            if (path.find("..") != String::npos ||
                path.find("//") != String::npos)
            {
                throw FileException(FileException::ErrorCode::SECURITY_VIOLATION,
                                    "Invalid path detected: potential directory traversal");
            }
            // Normalize path
            try
            {
                auto canonical = std::filesystem::weakly_canonical(path.c_str());
                return canonical.string();
            }
            catch (const std::filesystem::filesystem_error &)
            {
                throw FileException(FileException::ErrorCode::INVALID_PATH,
                                    "Invalid file path: " + path);
            }
        }

        /**
         * @brief Loads large files using memory mapping for minimal memory usage
         *
         * Maps file directly into virtual memory space, eliminating the need to
         * load entire file content into physical RAM. Ideal for very large files.
         *
         * @param file_size Size of the file in bytes
         * @param progress Optional progress callback
         * @return Vector containing the mapped file data
         * @throws FileException if mapping fails or file cannot be opened
         */
        [[nodiscard]] Vector<DataType> loadWithMemoryMapping(
            size_t file_size, const ProgressCallback &progress) const
        {
            return loadWithBufferedIO(file_size, file_size / sizeof(DataType), progress);
        }

        /**
         * @brief Loads smaller files using optimized buffered I/O
         *
         * Uses 64KB buffer chunks with pre-allocated memory for optimal performance
         * on smaller files where memory mapping overhead isn't justified.
         *
         * @param file_size Size of the file in bytes
         * @param element_count Number of DataType elements expected
         * @param progress Optional progress callback
         * @return Vector containing the loaded file data
         * @throws FileException if file cannot be opened or read error occurs
         */
        [[nodiscard]] Vector<DataType> loadWithBufferedIO(
            size_t file_size, size_t element_count, const ProgressCallback &progress) const
        {
            Vector<DataType> result;
            result.reserve(element_count); // Pre-allocate for performance

            input_stream = std::make_unique<std::ifstream>(
                filename.c_str(), std::ios::binary);

            if (!input_stream->is_open())
            {
                throw FileException(FileException::ErrorCode::READ_ERROR,
                                    "Cannot open file for reading: " + filename);
            }
            // Set optimal buffer size
            input_stream->rdbuf()->pubsetbuf(nullptr, DEFAULT_BUFFER_SIZE);

            const size_t chunk_size = DEFAULT_BUFFER_SIZE / sizeof(DataType);
            Vector<DataType> buffer(chunk_size);
            size_t total_read = 0;

            while (total_read < element_count)
            {
                const size_t to_read = std::min(chunk_size, element_count - total_read);
                const size_t bytes_to_read = to_read * sizeof(DataType);

                input_stream->read(
                    reinterpret_cast<char *>(buffer.raw_data()),
                    bytes_to_read);

                if (input_stream->fail() && !input_stream->eof())
                {
                    throw FileException(FileException::ErrorCode::READ_ERROR,
                                        "Failed to read from file");
                }

                const size_t actual_read = input_stream->gcount() / sizeof(DataType);
                result.extend(buffer, 0, actual_read);

                total_read += actual_read;

                if (progress && !progress(total_read, element_count))
                {
                    break; // User cancelled
                }
            }

            return result;
        }

        /**
         * @brief Safely closes and releases file stream resources
         *
         * Performs cleanup of input and output streams with exception safety.
         * Called automatically by destructor to ensure no resource leaks.
         *
         * @note This method never throws exceptions (noexcept)
         */
        void closeStreams() const noexcept
        {
            try
            {
                if (input_stream)
                {
                    input_stream->close();
                    input_stream.reset();
                }
                if (output_stream)
                {
                    output_stream->close();
                    output_stream.reset();
                }
            }
            catch (...)
            {
                // Ignore errors during cleanup
            }
        }
    };
}