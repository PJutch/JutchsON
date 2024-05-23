#include "JutchsON.hpp"

JutchsON::ParseResult<int> testOkConstructorCompiles{12}; 
JutchsON::ParseResult testOkConstructorDeduces{12};

std::vector<JutchsON::ParseError> testErrors{{{1, 2}, "foo"}, {{3, 4}, "bar"}};
JutchsON::ParseResult<int> testErrorConstructorCompiles{testErrors};
