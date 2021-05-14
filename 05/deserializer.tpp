Deserializer::Deserializer(std::istream& in) : in_(in) {

}

template<class... ArgsT>
Error Deserializer::operator()(ArgsT&... args)
{
    Error err = process(args...);
    if(!IsEmptyStream()) return Error::CorruptedArchive;
    return err;
}

bool Deserializer::IsEmptyStream() {
    std::string object_str;
    std::getline(in_, object_str);
    return object_str.empty();
}

template<class T>
Error Deserializer::Load(T& object) {
    return object.Load(*this);
}

Error Deserializer::process(uint64_t& object) {
    std::string object_str;
    std::getline(in_, object_str, kSeparator);

    try {
        object = std::stoull(object_str);
        return Error::NoError;
    }
    catch (...) {
        return Error::CorruptedArchive;
    }
}

Error Deserializer::process(bool& object) {
    std::string object_str;
    std::getline(in_, object_str, kSeparator);
    if (object_str == "true" || object_str == "false") {
        object = (object_str == "true") ? true : false;
        return Error::NoError;
    }
    else {
        return Error::CorruptedArchive;
    }
}

template<class T, class... ArgsT>
Error Deserializer::process(T& arg, ArgsT&... args) {
    const Error err = process(arg);
    if (err == Error::NoError) {
        return process(args...);
    }
    else {
        return err;
    }
}