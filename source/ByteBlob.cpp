module byte_blob;

ByteBlob::ByteBlob(Path path)
{
    std::ifstream fstream(path.c_str(), std::ios::binary);
    if (fstream.is_open())
    {
        std::stringstream sstream;
        sstream << fstream.rdbuf();
        this->str = sstream.str();
    }
}
