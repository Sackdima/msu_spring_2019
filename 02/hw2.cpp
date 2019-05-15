#include <iostream>


enum Token
{
	Invalid,
	Number,
	Minus,
	Plus,
	Mul,
	Div,
	End
};

struct Context
{
	int64_t result;
	Token token;
};

Token get_token(const char*& ptr)
{
	while (const char c = *ptr)
	{
		++ptr;

		switch (c)
		{
			case ' ':
				continue;
			case '-':
				return Token::Minus;
			case '+':
				return Token::Plus;
			case '*':
				return Token::Mul;
			case '/':
				return Token::Div;
		}
		if (c >= '0' && c <= '9')
		{
			//skip digits
			while (char c = *ptr && c >= '0' && c <= '9')
				ptr++;

			return Token::Number;
		}

		return Token::Invalid;
	}

	return Token::End;
}

//evaluate number and minus
Context prim(const char*& text)
{
	const char* prev = text;

	const Token token = get_token(text);
	if (token == Token::End)
		return { 0, Token::End };

	if (token == Token::Number)
	{
		return { std::atol(prev), get_token(text) };
	}
	else if (token == Token::Minus)//unary minus
	{
		const Context context = prim(text);
		return { -context.result, context.token };
	}
	else if (token == Token::Plus)//unary plus
	{
		const Context context = prim(text);
		return { context.result, context.token };
	}

	throw std::runtime_error("unexpected token");
}

//evaluate multiplication and division
Context term(const char*& text)
{
	Context context = prim(text);
	int64_t result = context.result;

	while (true)
	{
		if (context.token == Token::Mul)
		{
			context = prim(text);
			result *= context.result;
		}
		else if (context.token == Token::Div)
		{
			context = prim(text);

			if (context.result == 0)
				throw std::runtime_error("division by zero");

			result /= context.result;
		}
		else
		{
			return { result, context.token };
		}
	}
}

//Evaluate expression, first checking plus and minus
Context expr(const char*& text)
{
	Context context = term(text);
	int64_t result = context.result;

	while (true)
	{
		if (context.token == Token::Minus)
		{
			context = term(text);
			result -= context.result;
		}
		else
		if (context.token == Token::Plus)
		{
			context = term(text);
			result += context.result;
		}
		else
		{
			return { result, context.token };
		}
	}
}

int calc(const char* text)
{
	const Context context = expr(text);
	if (context.token == Token::Invalid)
		throw std::runtime_error("unexpected token");
	return context.result;
}

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::cout << "error\n";
		return 1;
	}

	try
	{
		const int64_t result = calc(argv[1]);
		std::cout << result << '\n';
	}
	catch (std::runtime_error)
	{
		std::cout << "error\n";
		return 1;
	}

	return 0;
}