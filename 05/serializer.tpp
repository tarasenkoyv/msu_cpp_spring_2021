Serializer::Serializer(std::ostream& out) : out_(out) {

}

template<class T>
Error Serializer::Save(T& object) {
    return object.Serialize(*this);
}

template<class... ArgsT>
Error Serializer::operator()(ArgsT... args)
{
    return process(args...);
}

Error Serializer::process(bool arg) {
    out_ << (arg ? "true" : "false") << kSeparator;
    return Error::NoError;
}

Error Serializer::process(uint64_t arg) {
    out_ << arg << kSeparator;
    return Error::NoError;
}

template<class T, class... ArgsT>
Error Serializer::process(T&& arg, ArgsT&&... args) {
    const Error err = process(arg);
    if (err == Error::NoError) {
        return process(std::forward<ArgsT>(args)...);
    }
    else {
        return err;
    }
}