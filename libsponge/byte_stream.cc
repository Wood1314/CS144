#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : streamflow(""), streamCapacity(capacity), inputEnded(false), bytesReaden(0), bytesWriten(0), eofFlag(false) {}

size_t ByteStream::write(const string &data) {
    size_t dataSize = data.size();
    size_t writenSize = 0;
    while (this->streamflow.size() < this->streamCapacity && writenSize < dataSize) {
        this->streamflow.push_back(data[writenSize]);
        writenSize++;
    }
    this->bytesWriten += writenSize;
    return writenSize;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (len < this->streamflow.size()) {
        return this->streamflow.substr(0, len);
    } else {
        return this->streamflow;
    }
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (this->streamflow.empty()) {
        this->eofFlag = true;
        return;
    }
    if (len < this->streamflow.size()) {
        this->streamflow.erase(0, len);
        this->bytesReaden += len;
    } else {
        this->bytesReaden += this->streamflow.size();
        this->streamflow.clear();
        if (this->inputEnded)
            this->eofFlag = true;
    }
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string result = "";
    if (this->streamflow.empty()) {
        this->eofFlag = true;
        return result;
    }
    if (len < this->streamflow.size()) {
        result = this->streamflow.substr(0, len);
        this->streamflow.erase(0, len);
        this->bytesReaden += len;
    } else {
        result = this->streamflow;
        this->bytesReaden += this->streamflow.size();
        this->streamflow.clear();
        if (this->inputEnded)
            this->eofFlag = true;
    }
    return result;
}

void ByteStream::end_input() {
    this->inputEnded = true;
    this->eofFlag = this->streamflow.empty();
}

bool ByteStream::input_ended() const { return this->inputEnded; }

size_t ByteStream::buffer_size() const { return this->streamflow.size(); }

bool ByteStream::buffer_empty() const { return this->streamflow.empty(); }

bool ByteStream::eof() const { return this->eofFlag; }

size_t ByteStream::bytes_written() const { return this->bytesWriten; }

size_t ByteStream::bytes_read() const { return this->bytesReaden; }

size_t ByteStream::remaining_capacity() const { return this->streamCapacity - this->streamflow.size(); }
