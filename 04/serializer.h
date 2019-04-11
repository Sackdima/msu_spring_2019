#pragma once
#include <iostream>


enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
		: out_(out.rdbuf())
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}

private:
	std::ostream out_;
	Error process()
	{
		return Error::NoError;
	}
	template<class... ArgsT>
	Error process(uint64_t arg, ArgsT&... args)
	{
		out_ << arg << Separator;
		return process(args...);
	}
	template<class... ArgsT>
	Error process(bool arg, ArgsT&... args)
	{
		if (arg)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;

		return process(args...);
	}
};

class Deserializer
{
	static constexpr char Separator = ' ';
public:
	explicit Deserializer(std::istream& in)
		: in_(in.rdbuf())
	{
	}

	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&... args)
	{
		return process(args...);
	}

private:
	std::istream in_;
	Error process() { return Error::NoError; }
	template<class... ArgsT>
	Error process(uint64_t &arg, ArgsT&... args)
	{
		std::string text;
		in_ >> text;
		uint64_t res = 0;
		for(auto &i : text)
		{
			if (!isdigit(i))
				return Error::CorruptedArchive;
			res *= 10;
			res += i - '0';
		}

		arg = res;
		return process(args...);
	}
	template<class... ArgsT>
	Error process(bool &arg, ArgsT&... args)
	{
		std::string text;
		in_ >> text;
		if (text == "true")
			arg = true;
		else if (text == "false")
			arg = false;
		else
			return Error::CorruptedArchive;

		return process(args...);
	}
};
